#pragma once
#include <cstdarg>
namespace Eloquent {
    namespace ML {
        namespace Port {
            class RandomForest {
                public:
                    /**
                    * Predict class for features vector
                    */
                    int predict(float *x) {
                        uint8_t votes[2] = { 0 };
                        // tree #1
                        if (x[3] <= 0.26680000126361847) {
                            if (x[0] <= 0.13439999520778656) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[0] <= 0.46024999022483826) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[3] <= 0.2875500023365021) {
                                if (x[2] <= 64.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #2
                        if (x[3] <= 0.29375000298023224) {
                            if (x[3] <= 0.25349999964237213) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 0.6168999969959259) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[3] <= 0.2961000055074692) {
                                votes[1] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #3
                        if (x[2] <= 83.0) {
                            if (x[3] <= 0.26270000636577606) {
                                if (x[1] <= 0.05090000107884407) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 0.2733500003814697) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.3149999976158142) {
                                if (x[1] <= 0.11955000087618828) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #4
                        if (x[2] <= 84.5) {
                            if (x[1] <= 0.1928500011563301) {
                                if (x[2] <= 66.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 80.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.29294998943805695) {
                                if (x[0] <= 0.2587999925017357) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 0.2797500044107437) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #5
                        if (x[3] <= 0.263949990272522) {
                            if (x[2] <= 86.5) {
                                if (x[1] <= 0.18834999948740005) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[1] <= 0.1959500014781952) {
                                if (x[3] <= 0.2875500023365021) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.6556499898433685) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        // tree #6
                        if (x[3] <= 0.2778500020503998) {
                            if (x[2] <= 82.5) {
                                if (x[0] <= 0.4167499989271164) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 0.2698500007390976) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 0.22744999825954437) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.2548999935388565) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #7
                        if (x[1] <= 0.1863499954342842) {
                            if (x[2] <= 65.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[3] <= 0.24710000306367874) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 81.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[3] <= 0.25015000253915787) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #8
                        if (x[0] <= 0.4155000001192093) {
                            if (x[3] <= 0.24904999881982803) {
                                if (x[1] <= 0.09519999846816063) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 64.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 82.5) {
                                if (x[3] <= 0.2089499980211258) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #9
                        if (x[1] <= 0.23030000180006027) {
                            if (x[2] <= 83.0) {
                                if (x[3] <= 0.25064999610185623) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[1] <= 0.12370000034570694) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[3] <= 0.2777999937534332) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 77.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #10
                        if (x[3] <= 0.24729999899864197) {
                            if (x[3] <= 0.2089499980211258) {
                                if (x[0] <= 0.5151500105857849) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        else {
                            if (x[0] <= 0.6556499898433685) {
                                if (x[2] <= 64.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #11
                        if (x[2] <= 82.5) {
                            if (x[3] <= 0.26365000009536743) {
                                if (x[1] <= 0.05739999935030937) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 65.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 84.5) {
                                if (x[0] <= 0.3458999991416931) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 89.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #12
                        if (x[3] <= 0.2778500020503998) {
                            if (x[0] <= 0.41269999742507935) {
                                if (x[3] <= 0.25200000405311584) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 0.2089499980211258) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 0.22744999825954437) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[2] <= 78.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #13
                        if (x[2] <= 65.0) {
                            votes[0] += 1;
                        }

                        else {
                            if (x[1] <= 0.1959500014781952) {
                                if (x[0] <= 0.298799991607666) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 0.2672000080347061) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #14
                        if (x[3] <= 0.26635000109672546) {
                            if (x[0] <= 0.25270000100135803) {
                                if (x[2] <= 85.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 0.2089499980211258) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.08374999836087227) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 64.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #15
                        if (x[2] <= 83.5) {
                            if (x[1] <= 0.05225000157952309) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[3] <= 0.2672000080347061) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 0.12344999983906746) {
                                if (x[3] <= 0.26475000381469727) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // return argmax of votes
                        uint8_t classIdx = 0;
                        float maxVotes = votes[0];

                        for (uint8_t i = 1; i < 2; i++) {
                            if (votes[i] > maxVotes) {
                                classIdx = i;
                                maxVotes = votes[i];
                            }
                        }

                        return classIdx;
                    }

                    /**
                    * Predict readable class name
                    */
                    const char* predictLabel(float *x) {
                        return idxToLabel(predict(x));
                    }

                    /**
                    * Convert class idx to readable name
                    */
                    const char* idxToLabel(uint8_t classIdx) {
                        switch (classIdx) {
                            case 0:
                            return "Normal";
                            case 1:
                            return "Hipertensi";
                            default:
                            return "Houston we have a problem";
                        }
                    }

                protected:
                };
            }
        }
    }