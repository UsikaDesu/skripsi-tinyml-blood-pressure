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
                        if (x[3] <= 82.60499954223633) {
                            if (x[0] <= 0.4113999903202057) {
                                if (x[2] <= 261.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 81.7599983215332) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 0.12249999865889549) {
                                if (x[2] <= 191.73500061035156) {
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
                        if (x[3] <= 84.1400032043457) {
                            if (x[2] <= 190.71499633789062) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 0.5936500132083893) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 191.73500061035156) {
                                if (x[0] <= 0.3376999944448471) {
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

                        // tree #3
                        if (x[0] <= 0.46194998919963837) {
                            if (x[3] <= 65.24499893188477) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 0.14694999903440475) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[3] <= 81.7599983215332) {
                                if (x[0] <= 0.48954999446868896) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[1] <= 0.25519999116659164) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #4
                        if (x[1] <= 0.2428000047802925) {
                            if (x[1] <= 0.1490500047802925) {
                                if (x[2] <= 261.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 82.45500183105469) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 0.25495000183582306) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 0.26114998757839203) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        // tree #5
                        if (x[3] <= 84.1400032043457) {
                            if (x[2] <= 213.1750030517578) {
                                if (x[2] <= 195.5749969482422) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 266.11000061035156) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.29294998943805695) {
                                if (x[0] <= 0.2646999955177307) {
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

                        // tree #6
                        if (x[3] <= 84.32500076293945) {
                            if (x[2] <= 209.61500549316406) {
                                if (x[0] <= 0.3189999908208847) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 65.70000076293945) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 0.08554999902844429) {
                                if (x[1] <= 0.0521499989554286) {
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

                        // tree #7
                        if (x[1] <= 0.1863499954342842) {
                            if (x[2] <= 250.2750015258789) {
                                if (x[3] <= 84.97999954223633) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        else {
                            if (x[3] <= 82.60499954223633) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #8
                        if (x[0] <= 0.4155000001192093) {
                            if (x[3] <= 65.24499893188477) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 0.124549999833107) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 199.70000457763672) {
                                if (x[2] <= 192.93000030517578) {
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

                        // tree #9
                        if (x[1] <= 0.23030000180006027) {
                            if (x[1] <= 0.10965000092983246) {
                                if (x[3] <= 68.17499923706055) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 186.51499938964844) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[3] <= 83.52499771118164) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #10
                        if (x[3] <= 84.32500076293945) {
                            if (x[1] <= 0.2418000027537346) {
                                if (x[2] <= 190.71499633789062) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 202.55500030517578) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 0.13954999670386314) {
                                if (x[2] <= 176.78500366210938) {
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