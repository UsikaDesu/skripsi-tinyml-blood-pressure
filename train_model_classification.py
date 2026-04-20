"""
Blood Pressure Categorization Model Training
Trains a neural network for BP classification (Hypotension vs Normal vs Hypertension)
"""

import numpy as np
import pickle
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
import matplotlib.pyplot as plt
from sklearn.metrics import classification_report, accuracy_score
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
    print("\nCreating model architecture (Classification)...")
    
    model = keras.Sequential([
        # Input layer
        layers.Input(shape=(input_dim,)),
        
        # Hidden layers - kept small for microcontroller deployment
        layers.Dense(16, activation='relu', name='dense1'),
        layers.Dense(12, activation='relu', name='dense2'),
        layers.Dense(8, activation='relu', name='dense3'),
        
        # Output layer - 3 outputs (Categorical Probabilities)
        layers.Dense(3, activation='softmax', name='output')
    ])
    
    model.compile(
        optimizer=keras.optimizers.Adam(learning_rate=0.001),
        loss='sparse_categorical_crossentropy',
        metrics=['accuracy']
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
    
    # Get class probabilities
    y_pred_probs = model.predict(X_test, verbose=0)
    # Get class predictions (argmax)
    y_pred = np.argmax(y_pred_probs, axis=1)
    
    # Accuracy
    acc = accuracy_score(y_test, y_pred)
    print(f"\nOverall Test Accuracy: {acc * 100:.2f}%\n")
    
    class_names = {0: "Hypotension", 1: "Normal", 2: "Hypertension"}
    target_names = [class_names[i] for i in range(3) if i in np.unique(y_test) or i in np.unique(y_pred)]
    
    try:
        report = classification_report(y_test, y_pred, target_names=target_names)
        print("Classification Report:")
        print(report)
    except Exception as e:
        pass
    
    # Show some predictions
    print("\nSample predictions (first 5 test samples):")
    print(f"  {'Actual':<15} -> {'Predicted':<15} | Confidence")
    print("-" * 50)
    for i in range(min(5, len(y_test))):
        actual = class_names.get(y_test[i], "Unknown")
        pred = class_names.get(y_pred[i], "Unknown")
        confidence = y_pred_probs[i][y_pred[i]] * 100
        print(f"  {actual:<15} -> {pred:<15} | {confidence:.1f}%")
    
    return {'accuracy': acc, 'predictions': y_pred}

def plot_training_history(history):
    """Plot training curves"""
    print("\nGenerating training plots...")
    
    fig, axes = plt.subplots(1, 2, figsize=(12, 4))
    
    # Loss plot
    axes[0].plot(history.history['loss'], label='Training Loss')
    axes[0].plot(history.history['val_loss'], label='Validation Loss')
    axes[0].set_xlabel('Epoch')
    axes[0].set_ylabel('Loss (Crossentropy)')
    axes[0].set_title('Model Loss')
    axes[0].legend()
    axes[0].grid(True)
    
    # Accuracy plot
    if 'accuracy' in history.history:
        axes[1].plot(history.history['accuracy'], label='Training Accuracy')
        axes[1].plot(history.history['val_accuracy'], label='Validation Accuracy')
        axes[1].set_xlabel('Epoch')
        axes[1].set_ylabel('Accuracy')
        axes[1].set_title('Model Accuracy')
        axes[1].legend()
        axes[1].grid(True)
    
    plt.tight_layout()
    plt.savefig('training_history.png', dpi=150)
    print("  ✓ Saved training_history.png")
    
    plt.close()

def main():
    """Main training pipeline"""
    print("=" * 70)
    print("Blood Pressure Categorization Model Training")
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
    print("  1. Review training_history.png to check accuracy convergence")
    print("  2. Run convert_to_tflite.py to convert classification model for ESP32")
    print("=" * 70)
    
    return True

if __name__ == "__main__":
    success = main()
    if not success:
        exit(1)
