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
                        if (x[3] <= 0.5913843512535095) {
                            if (x[3] <= -1.067858636379242) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= -0.08313041552901268) {
                                    if (x[3] <= -0.36599932610988617) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[2] <= -1.0531631112098694) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[3] <= -0.24085424095392227) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[0] <= 1.382839560508728) {
                                            if (x[0] <= -0.49637050926685333) {
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
                                }
                            }
                        }

                        else {
                            if (x[0] <= -0.5091507136821747) {
                                if (x[0] <= -0.7947489023208618) {
                                    votes[1] += 1;
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
                        if (x[2] <= 0.13483577594161034) {
                            if (x[2] <= -0.2778024971485138) {
                                if (x[2] <= -1.7752801179885864) {
                                    if (x[2] <= -1.9799354076385498) {
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

                            else {
                                if (x[2] <= -0.13471020013093948) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[0] <= 0.3635539412498474) {
                                        if (x[2] <= 0.041659386828541756) {
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
                            }
                        }

                        else {
                            if (x[3] <= -1.206803023815155) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 1.0466333031654358) {
                                    if (x[3] <= 0.8892582058906555) {
                                        if (x[3] <= 0.6089903116226196) {
                                            if (x[1] <= -0.649372398853302) {
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

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    if (x[1] <= -0.45767827332019806) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }
                        }

                        // tree #3
                        if (x[2] <= -0.3010965883731842) {
                            if (x[3] <= 0.5147746503353119) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[1] <= 1.9790714383125305) {
                                if (x[3] <= -1.206803023815155) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[2] <= 0.509205162525177) {
                                        if (x[3] <= -0.10619240999221802) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            if (x[2] <= 0.07327280472964048) {
                                                votes[1] += 1;
                                            }

                                            else {
                                                votes[0] += 1;
                                            }
                                        }
                                    }

                                    else {
                                        if (x[0] <= 1.0865477323532104) {
                                            if (x[1] <= -0.29244503378868103) {
                                                votes[1] += 1;
                                            }

                                            else {
                                                votes[1] += 1;
                                            }
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #4
                        if (x[2] <= 0.5175244808197021) {
                            if (x[0] <= -1.321345031261444) {
                                if (x[1] <= -1.6842854022979736) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.01337369903922081) {
                                    if (x[0] <= 0.6282865107059479) {
                                        if (x[0] <= 0.24566318094730377) {
                                            if (x[3] <= 0.5062095820903778) {
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

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    if (x[2] <= 0.3844153732061386) {
                                        if (x[1] <= 1.0893990397453308) {
                                            if (x[3] <= -0.0076941996812820435) {
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
                                        if (x[2] <= 0.4592892527580261) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.5182203501462936) {
                                if (x[0] <= -0.5091507136821747) {
                                    if (x[1] <= -0.9645504355430603) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    if (x[2] <= 0.8153561353683472) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[2] <= 0.7022134065628052) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        // tree #5
                        if (x[3] <= 0.5913843512535095) {
                            if (x[2] <= 0.1498105488717556) {
                                if (x[2] <= -0.28612181544303894) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[1] <= 0.24912025034427643) {
                                        if (x[3] <= -0.8741930648684502) {
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
                            }

                            else {
                                if (x[0] <= -0.9801921248435974) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[1] <= -0.5400009155273438) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[0] <= 0.357294250279665) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }
                            }
                        }

                        else {
                            if (x[1] <= -0.5782221257686615) {
                                if (x[3] <= 1.0105966329574585) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
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

                protected:
                };
            }
        }
    }