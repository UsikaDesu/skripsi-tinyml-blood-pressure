#pragma once
#include <cstdarg>
namespace Eloquent {
    namespace ML {
        namespace Port {
            class SVM {
                public:
                    /**
                    * Predict class for features vector
                    */
                    int predict(float *x) {
                        float kernels[31] = { 0 };
                        float decisions[1] = { 0 };
                        int votes[2] = { 0 };
                        kernels[0] = compute_kernel(x,   -1.428016594547  , -1.514506364508  , -0.006778266174  , 0.397871390242 );
                        kernels[1] = compute_kernel(x,   -0.605919044474  , -0.599973695406  , 1.644861014831  , -1.29294221168 );
                        kernels[2] = compute_kernel(x,   -0.501728842366  , -0.464153120513  , 1.615081670315  , -1.550137072615 );
                        kernels[3] = compute_kernel(x,   -0.610455440703  , -0.656918045012  , 1.576385355397  , -1.245625309829 );
                        kernels[4] = compute_kernel(x,   -0.78585606092  , -0.653477228414  , -0.804939242811  , 0.624778372812 );
                        kernels[5] = compute_kernel(x,   -0.780847411044  , -0.72753056027  , -0.75785488667  , 0.752007733864 );
                        kernels[6] = compute_kernel(x,   -0.744796766089  , -0.758711107443  , -0.792950551442  , 0.718319077111 );
                        kernels[7] = compute_kernel(x,   -0.739059221857  , -0.621126323627  , -0.784955354939  , 0.601330942538 );
                        kernels[8] = compute_kernel(x,   -0.738116200641  , -0.73047855326  , -0.771052889196  , 0.741475599493 );
                        kernels[9] = compute_kernel(x,   -0.477380045134  , -0.311675305303  , -0.774115140216  , 0.120575446162 );
                        kernels[10] = compute_kernel(x,   0.99770406623  , 0.964525568221  , -0.407531409222  , 0.225983176053 );
                        kernels[11] = compute_kernel(x,   0.387285382141  , 0.569877105324  , -0.754460772835  , 0.824699081835 );
                        kernels[12] = compute_kernel(x,   0.582075759995  , 0.682633809009  , -0.613902266719  , 0.208063861971 );
                        kernels[13] = compute_kernel(x,   0.592268279767  , 0.6890404399  , -0.728254949661  , 0.02570848926 );
                        kernels[14] = compute_kernel(x,   0.608689561621  , 0.821017036259  , -0.613902266719  , 0.445231254227 );
                        kernels[15] = compute_kernel(x,   0.595665786357  , 0.832548971863  , -0.839629698151  , 0.496881041873 );
                        kernels[16] = compute_kernel(x,   1.339153478573  , 1.145192559353  , -0.846776740835  , -0.179836584028 );
                        kernels[17] = compute_kernel(x,   1.877658273163  , 1.70641342542  , -0.109737964061  , 0.540098211129 );
                        kernels[18] = compute_kernel(x,   1.867465753391  , 1.653879052113  , -0.041245471674  , 0.398851853075 );
                        kernels[19] = compute_kernel(x,   1.900874568198  , 1.778167691402  , -0.063877773506  , 0.155359997026 );
                        kernels[20] = compute_kernel(x,   -0.173869456385  , -0.242483691678  , -0.459645262125  , 1.452929151987 );
                        kernels[21] = compute_kernel(x,   1.088870493074  , 1.251542632146  , -0.456667327674  , 0.815212386145 );
                        kernels[22] = compute_kernel(x,   1.212879483627  , 1.516777151041  , -0.244042807829  , 0.959620976096 );
                        kernels[23] = compute_kernel(x,   -0.467753776461  , -0.414181399562  , -0.453391599777  , 0.398851853075 );
                        kernels[24] = compute_kernel(x,   0.609822063817  , 0.658288611623  , -0.722596874203  , -0.41067951249 );
                        kernels[25] = compute_kernel(x,   -0.468497014876  , -0.379683769605  , -0.490023656714  , 0.337362673175 );
                        kernels[26] = compute_kernel(x,   1.113313672578  , 1.136619389936  , -0.446827129419  , -1.161159741087 );
                        kernels[27] = compute_kernel(x,   -0.423901277675  , -0.408642707553  , -0.459755687558  , 0.420200163026 );
                        kernels[28] = compute_kernel(x,   -0.470721160996  , -0.403458160647  , -0.431150875079  , 0.390448400948 );
                        kernels[29] = compute_kernel(x,   -0.92811591947  , -0.893397390222  , 1.287508880637  , -0.735335320555 );
                        kernels[30] = compute_kernel(x,   -0.815998201985  , -0.652508068714  , 1.237777375295  , -0.802796267686 );
                        float decision = -2.360654100256;
                        decision = decision - ( + kernels[0] * -8.0  + kernels[1] * -8.0  + kernels[2] * -4.484723527864  + kernels[3] * -8.0  + kernels[4] * -8.0  + kernels[5] * -8.0  + kernels[6] * -0.164627817259  + kernels[7] * -8.0  + kernels[8] * -8.0  + kernels[9] * -8.0  + kernels[10] * -8.0  + kernels[11] * -8.0  + kernels[12] * -8.0  + kernels[13] * -8.0  + kernels[14] * -8.0  + kernels[15] * -8.0  + kernels[16] * -7.463286757522  + kernels[17] * -0.835532891401  + kernels[18] * -8.0  + kernels[19] * -0.18532994416 );
                        decision = decision - ( + kernels[20] * 11.026743571984  + kernels[21] * 13.333333333333  + kernels[22] * 13.333333333333  + kernels[23] * 13.333333333333  + kernels[24] * 13.333333333333  + kernels[25] * 13.333333333333  + kernels[26] * 13.333333333333  + kernels[27] * 13.333333333333  + kernels[28] * 10.113162216812  + kernels[29] * 13.333333333333  + kernels[30] * 5.326928482745 );

                        return decision > 0 ? 0 : 1;
                    }

                protected:
                    /**
                    * Compute kernel between feature vector and support vector.
                    * Kernel type: rbf
                    */
                    float compute_kernel(float *x, ...) {
                        va_list w;
                        va_start(w, 4);
                        float kernel = 0.0;

                        for (uint16_t i = 0; i < 4; i++) {
                            kernel += pow(x[i] - va_arg(w, double), 2);
                        }

                        return exp(-0.1 * kernel);
                    }
                };
            }
        }
    }
