"""
Blood Pressure Estimation Model Training
Trains a neural network for BP prediction from PPG sensor data
"""

import numpy as np
import pickle
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
import matplotlib.pyplot as plt
from sklearn.metrics import mean_absolute_error, mean_squared_error
import os

PROCESSED_DATA_FILE = 'processed_data.pkl'
MODEL_FILE = 'bp_model.h5'
TRAINING_HISTORY_FILE = 'training_history.pkl'

def load_processed_data():
    """Load preprocessed data"""
    if not os.path.exists(PROCESSED_DATA_FILE):
        print(f"ERROR: {PROCESSED_DATA_FILE} not found!")
        print("Please run preprocess_data.py first!")
        return None
    
    with open(PROCESSED_DATA_FILE, 'rb') as f:
        data = pickle.load(f)
    
    print("Loaded preprocessed data:")
    print(f"  Training samples: {len(data['X_train'])}")
    print(f"  Test samples: {len(data['X_test'])}")
    print(f"  Features: {data['feature_names']}")
    
    return data

def create_model(input_dim):
    """Create a small neural network optimized for TinyML deployment"""
    print("\nCreating model architecture...")
    
    model = keras.Sequential([
        # Input layer
        layers.Input(shape=(input_dim,)),
        
        # Hidden layers - kept small for microcontroller deployment
        layers.Dense(16, activation='relu', name='dense1'),
        layers.Dense(12, activation='relu', name='dense2'),
        layers.Dense(8, activation='relu', name='dense3'),
        
        # Output layer - 2 outputs (systolic, diastolic)
        layers.Dense(2, activation='linear', name='output')
    ])
    
    model.compile(
        optimizer=keras.optimizers.Adam(learning_rate=0.001),
        loss='mse',
        metrics=['mae']
    )
    
    print("\nModel summary:")
    model.summary()
    
    # Calculate model size
    total_params = model.count_params()
    print(f"\nTotal parameters: {total_params:,}")
    print(f"Estimated size: ~{total_params * 4 / 1024:.1f} KB (float32)")
    
    return model

def train_model(model, X_train, y_train, X_test, y_test):
    """Train the model"""
    print("\n" + "=" * 70)
    print("Training model...")
    print("=" * 70)
    
    # Callbacks
    early_stopping = keras.callbacks.EarlyStopping(
        monitor='val_loss',
        patience=20,
        restore_best_weights=True,
        verbose=1
    )
    
    reduce_lr = keras.callbacks.ReduceLROnPlateau(
        monitor='val_loss',
        factor=0.5,
        patience=10,
        min_lr=0.00001,
        verbose=1
    )
    
    # Train
    history = model.fit(
        X_train, y_train,
        validation_data=(X_test, y_test),
        epochs=200,
        batch_size=8,
        callbacks=[early_stopping, reduce_lr],
        verbose=1
    )
    
    return history

def evaluate_model(model, X_test, y_test):
    """Evaluate model performance"""
    print("\n" + "=" * 70)
    print("Evaluating model...")
    print("=" * 70)
    
    # Predictions
    y_pred = model.predict(X_test, verbose=0)
    
    # Systolic BP metrics
    systolic_mae = mean_absolute_error(y_test[:, 0], y_pred[:, 0])
    systolic_rmse = np.sqrt(mean_squared_error(y_test[:, 0], y_pred[:, 0]))
    
    # Diastolic BP metrics
    diastolic_mae = mean_absolute_error(y_test[:, 1], y_pred[:, 1])
    diastolic_rmse = np.sqrt(mean_squared_error(y_test[:, 1], y_pred[:, 1]))
    
    print("\nTest Set Performance:")
    print(f"  Systolic BP:")
    print(f"    MAE:  {systolic_mae:.2f} mmHg")
    print(f"    RMSE: {systolic_rmse:.2f} mmHg")
    print(f"  Diastolic BP:")
    print(f"    MAE:  {diastolic_mae:.2f} mmHg")
    print(f"    RMSE: {diastolic_rmse:.2f} mmHg")
    
    # Show some predictions
    print("\nSample predictions (first 5 test samples):")
    print("  Actual    ->  Predicted")
    for i in range(min(5, len(y_test))):
        actual_sys, actual_dia = y_test[i]
        pred_sys, pred_dia = y_pred[i]
        print(f"  {actual_sys:.0f}/{actual_dia:.0f}  ->  {pred_sys:.0f}/{pred_dia:.0f}")
    
    return {
        'systolic_mae': systolic_mae,
        'systolic_rmse': systolic_rmse,
        'diastolic_mae': diastolic_mae,
        'diastolic_rmse': diastolic_rmse,
        'predictions': y_pred
    }

def plot_training_history(history):
    """Plot training curves"""
    print("\nGenerating training plots...")
    
    fig, axes = plt.subplots(1, 2, figsize=(12, 4))
    
    # Loss plot
    axes[0].plot(history.history['loss'], label='Training Loss')
    axes[0].plot(history.history['val_loss'], label='Validation Loss')
    axes[0].set_xlabel('Epoch')
    axes[0].set_ylabel('Loss (MSE)')
    axes[0].set_title('Model Loss')
    axes[0].legend()
    axes[0].grid(True)
    
    # MAE plot
    axes[1].plot(history.history['mae'], label='Training MAE')
    axes[1].plot(history.history['val_mae'], label='Validation MAE')
    axes[1].set_xlabel('Epoch')
    axes[1].set_ylabel('MAE (mmHg)')
    axes[1].set_title('Mean Absolute Error')
    axes[1].legend()
    axes[1].grid(True)
    
    plt.tight_layout()
    plt.savefig('training_history.png', dpi=150)
    print("  ✓ Saved training_history.png")
    
    plt.close()

def main():
    """Main training pipeline"""
    print("=" * 70)
    print("Blood Pressure Estimation Model Training")
    print("=" * 70)
    
    # Load data
    data = load_processed_data()
    if data is None:
        return False
    
    X_train = data['X_train']
    X_test = data['X_test']
    y_train = data['y_train']
    y_test = data['y_test']
    
    # Create model
    model = create_model(input_dim=X_train.shape[1])
    
    # Train model
    history = train_model(model, X_train, y_train, X_test, y_test)
    
    # Evaluate model
    metrics = evaluate_model(model, X_test, y_test)
    
    # Plot results
    plot_training_history(history)
    
    # Save model
    model.save(MODEL_FILE)
    print(f"\n✓ Model saved to: {MODEL_FILE}")
    
    # Save training history
    with open(TRAINING_HISTORY_FILE, 'wb') as f:
        pickle.dump(history.history, f)
    
    print("\n" + "=" * 70)
    print("✓ Training complete!")
    print("=" * 70)
    print("\nNext steps:")
    print("  1. Review training_history.png to check model convergence")
    print("  2. Run convert_to_tflite.py to convert model for ESP32")
    print("=" * 70)
    
    return True

if __name__ == "__main__":
    success = main()
    if not success:
        exit(1)
