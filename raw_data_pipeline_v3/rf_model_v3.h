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
                        if (x[3] <= 84.09500122070312) {
                            if (x[3] <= 70.03554916381836) {
                                if (x[2] <= 254.85028076171875) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.24758797883987427) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[1] <= 0.19335000216960907) {
                                        if (x[2] <= 214.68685150146484) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[3] <= 81.19999694824219) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }
                            }
                        }

                        else {
                            if (x[2] <= 191.73500061035156) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[0] <= 0.3056495934724808) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #2
                        if (x[2] <= 188.6149444580078) {
                            votes[0] += 1;
                        }

                        else {
                            if (x[3] <= 81.45999908447266) {
                                if (x[2] <= 254.0152816772461) {
                                    if (x[3] <= 69.5686264038086) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[2] <= 199.24759674072266) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #3
                        if (x[2] <= 189.1231460571289) {
                            votes[0] += 1;
                        }

                        else {
                            if (x[3] <= 79.79216384887695) {
                                if (x[3] <= 69.8436279296875) {
                                    if (x[2] <= 254.85028076171875) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[0] <= 0.22929523885250092) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[3] <= 79.43805313110352) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[2] <= 198.6785125732422) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[0] <= 0.6485990285873413) {
                                    if (x[3] <= 81.38923263549805) {
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

                        // tree #4
                        if (x[2] <= 188.16314697265625) {
                            votes[0] += 1;
                        }

                        else {
                            if (x[1] <= 0.10900000110268593) {
                                if (x[2] <= 255.5) {
                                    if (x[2] <= 223.0128402709961) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    if (x[0] <= 0.23849523812532425) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[0] <= 0.650299996137619) {
                                    if (x[3] <= 78.7901382446289) {
                                        if (x[0] <= 0.5612528026103973) {
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
                        }

                        // tree #5
                        if (x[1] <= 0.10885779559612274) {
                            if (x[2] <= 225.97661590576172) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 254.85028076171875) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 188.22994232177734) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 0.5762566030025482) {
                                    if (x[0] <= 0.5066301822662354) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    if (x[3] <= 81.28999710083008) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }
                        }

                        // tree #6
                        if (x[2] <= 188.6149444580078) {
                            if (x[2] <= 181.96499633789062) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 0.19614999741315842) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[3] <= 79.79216384887695) {
                                if (x[3] <= 69.5686264038086) {
                                    if (x[3] <= 62.71865272521973) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    if (x[2] <= 197.48485565185547) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #7
                        if (x[0] <= 0.5397583246231079) {
                            if (x[2] <= 181.96499633789062) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[3] <= 63.736806869506836) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[3] <= 72.20221328735352) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[3] <= 79.64535140991211) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }
                            }
                        }

                        else {
                            if (x[3] <= 83.1247787475586) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #8
                        if (x[1] <= 0.10900000110268593) {
                            if (x[1] <= 0.0927279144525528) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 0.22391277551651) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[1] <= 0.10507982596755028) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[2] <= 188.6149444580078) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 202.84821319580078) {
                                    if (x[1] <= 0.2085999995470047) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[2] <= 193.62671661376953) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[3] <= 69.8436279296875) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[3] <= 80.31999969482422) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }
                            }
                        }

                        // tree #9
                        if (x[1] <= 0.10385215282440186) {
                            if (x[2] <= 227.43370819091797) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[3] <= 65.92499923706055) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 188.6149444580078) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 0.24449478089809418) {
                                    if (x[2] <= 250.5) {
                                        if (x[3] <= 78.53832626342773) {
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
                                    if (x[1] <= 0.28539998829364777) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }
                        }

                        // tree #10
                        if (x[1] <= 0.10900000110268593) {
                            if (x[3] <= 74.09791564941406) {
                                if (x[2] <= 254.85028076171875) {
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
                            if (x[2] <= 188.22994232177734) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 204.09500122070312) {
                                    if (x[2] <= 193.62671661376953) {
                                        if (x[2] <= 191.85871124267578) {
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
                                    if (x[1] <= 0.21692931652069092) {
                                        if (x[3] <= 70.73721313476562) {
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
