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
                        float kernels[30] = { 0 };
                        float decisions[1] = { 0 };
                        int votes[2] = { 0 };
                        kernels[0] = compute_kernel(x,   0.491286760936  , 0.367181765145  , -0.858013569608  , -0.925512876693 );
                        kernels[1] = compute_kernel(x,   0.47916459713  , 0.36324471295  , -0.756837911566  , -0.798994028764 );
                        kernels[2] = compute_kernel(x,   0.575843821578  , 0.527922103026  , -0.86307235251  , -1.089987378999 );
                        kernels[3] = compute_kernel(x,   0.52715644236  , 0.552358978718  , -0.62025077321  , -0.836949683143 );
                        kernels[4] = compute_kernel(x,   -1.978455071399  , -2.826717887726  , -1.262716201773  , -1.305069420477 );
                        kernels[5] = compute_kernel(x,   0.727172145147  , 0.129193747994  , -0.706250082545  , -0.685127065629 );
                        kernels[6] = compute_kernel(x,   0.91128992819  , 0.472531851459  , 0.173978142415  , 0.213156754662 );
                        kernels[7] = compute_kernel(x,   -0.668168398443  , -0.1097445921  , 0.01715587245  , 0.213156754662 );
                        kernels[8] = compute_kernel(x,   -0.654456442663  , -0.100920164767  , 0.027273438255  , 0.225808639455 );
                        kernels[9] = compute_kernel(x,   -0.157547088644  , -0.014169256062  , 0.153743010806  , 0.364979372176 );
                        kernels[10] = compute_kernel(x,   -0.210904481787  , -0.059377476091  , 0.128449096296  , 0.364979372176 );
                        kernels[11] = compute_kernel(x,   -0.084516019817  , 0.128379185471  , 0.96820705804  , 0.946966072646 );
                        kernels[12] = compute_kernel(x,   -0.096240735628  , 0.094303319924  , 0.775973307761  , 0.83309910951 );
                        kernels[13] = compute_kernel(x,   -0.546350588399  , -0.134181467791  , 0.614092254895  , 0.83309910951 );
                        kernels[14] = compute_kernel(x,   1.39200327647  , 0.231964386319  , -0.761896694468  , -0.748386489593 );
                        kernels[15] = compute_kernel(x,   1.44555939361  , 0.21893138595  , -1.040129754082  , -1.026727955035 );
                        kernels[16] = compute_kernel(x,   0.374238326816  , 0.768353807746  , 1.150323242514  , 1.022877381403 );
                        kernels[17] = compute_kernel(x,   0.399277550414  , 0.779486162227  , 1.403262387618  , 1.275915077259 );
                        kernels[18] = compute_kernel(x,   -1.410899336514  , -0.497476353071  , -0.053667088178  , 0.061334137148 );
                        kernels[19] = compute_kernel(x,   -1.190911872047  , -0.422943882212  , -0.225665706849  , -0.06518471078 );
                        kernels[20] = compute_kernel(x,   -1.037298222514  , -0.340808827804  , -0.200371792338  , -0.15374790433 );
                        kernels[21] = compute_kernel(x,   -0.7019514779  , 0.201554052125  , 0.43197607042  , 0.820447224717 );
                        kernels[22] = compute_kernel(x,   -0.676614168307  , 0.151865738219  , 0.543269294266  , 1.060833035781 );
                        kernels[23] = compute_kernel(x,   0.836768429387  , 0.731562733788  , -0.680956168035  , -0.710430835215 );
                        kernels[24] = compute_kernel(x,   0.894199664465  , 0.876690623311  , 0.386447024302  , 0.377631256968 );
                        kernels[25] = compute_kernel(x,   0.983128653037  , 0.91388897853  , 0.796208439369  , 0.795143455132 );
                        kernels[26] = compute_kernel(x,   0.551996941961  , 0.337857514315  , -1.15648176083  , -1.216506226928 );
                        kernels[27] = compute_kernel(x,   0.556766317884  , 0.315457044931  , -1.050247319886  , -0.988772300657 );
                        kernels[28] = compute_kernel(x,   1.033008376235  , 0.735228265142  , 0.214448405631  , -0.179051673916 );
                        kernels[29] = compute_kernel(x,   0.956102189471  , 0.666126211103  , -0.03343195657  , -0.166399789123 );
                        float decision = -0.173788614699;
                        decision = decision - ( + kernels[0] * -0.821428571429  + kernels[1] * -0.821428571429  + kernels[2] * -0.821428571429  + kernels[3] * -0.821428571429  + kernels[4] * -0.047752843604  + kernels[5] * -0.821428571429  + kernels[6] * -0.821428571429  + kernels[7] * -0.821428571429  + kernels[8] * -0.821428571429  + kernels[9] * -0.821428571429  + kernels[10] * -0.821428571429  + kernels[11] * -0.821428571429  + kernels[12] * -0.821428571429  + kernels[13] * -0.821428571429  + kernels[14] * -0.821428571429  + kernels[15] * -0.821428571429  + kernels[16] * -0.821428571429  + kernels[17] * -0.821428571429 );
                        decision = decision - ( + kernels[18] * 0.166443587238  + kernels[19] * 1.277777777778  + kernels[20] * 1.277777777778  + kernels[21] * 1.277777777778  + kernels[22] * 1.277777777778  + kernels[23] * 1.277777777778  + kernels[24] * 1.277777777778  + kernels[25] * 1.277777777778  + kernels[26] * 1.067817192874  + kernels[27] * 1.277777777778  + kernels[28] * 1.277777777778  + kernels[29] * 1.277777777778 );

                        return decision > 0 ? 0 : 1;
                    }

                protected:
                    /**
                    * Compute kernel between feature vector and support vector.
                    * Kernel type: linear
                    */
                    float compute_kernel(float *x, ...) {
                        va_list w;
                        va_start(w, 4);
                        float kernel = 0.0;

                        for (uint16_t i = 0; i < 4; i++) {
                            kernel += x[i] * va_arg(w, double);
                        }

                        return kernel;
                    }
                };
            }
        }
    }
