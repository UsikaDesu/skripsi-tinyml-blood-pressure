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
                        if (x[3] <= 114.5) {
                            if (x[0] <= 106742.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 108371.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 94544.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 97662.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[2] <= 662.0) {
                                        if (x[3] <= 240.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[2] <= 722.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }
                            }
                        }

                        // tree #2
                        if (x[2] <= 594.0) {
                            if (x[3] <= 88.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 106766.0) {
                                    if (x[3] <= 108.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[0] <= 94544.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[2] <= 421.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[2] <= 640.0) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #3
                        if (x[2] <= 511.0) {
                            if (x[1] <= 99354.0) {
                                if (x[1] <= 97463.0) {
                                    if (x[2] <= 169.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[2] <= 404.0) {
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

                        else {
                            if (x[3] <= 293.0) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #4
                        if (x[1] <= 100691.5) {
                            if (x[0] <= 94417.0) {
                                if (x[1] <= 82578.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 201.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[1] <= 97554.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[0] <= 103841.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #5
                        if (x[1] <= 100659.0) {
                            if (x[2] <= 529.5) {
                                if (x[1] <= 90840.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[1] <= 93922.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[0] <= 107011.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[0] <= 102769.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #6
                        if (x[2] <= 594.0) {
                            if (x[2] <= 243.0) {
                                if (x[1] <= 96834.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 110.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[1] <= 95901.5) {
                                        if (x[1] <= 93890.0) {
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
                            }
                        }

                        else {
                            if (x[2] <= 645.5) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #7
                        if (x[1] <= 101043.0) {
                            if (x[2] <= 640.0) {
                                if (x[1] <= 96834.5) {
                                    if (x[2] <= 404.5) {
                                        if (x[3] <= 100.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[1] <= 95287.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[3] <= 190.0) {
                                        if (x[3] <= 97.5) {
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
                            }

                            else {
                                if (x[1] <= 99744.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #8
                        if (x[1] <= 100179.5) {
                            if (x[1] <= 93800.0) {
                                if (x[1] <= 82548.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[1] <= 93453.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[2] <= 247.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[3] <= 269.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[3] <= 223.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[0] <= 106561.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        // tree #9
                        if (x[1] <= 100659.0) {
                            if (x[2] <= 611.5) {
                                if (x[1] <= 99595.0) {
                                    if (x[1] <= 93207.0) {
                                        if (x[1] <= 90870.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[2] <= 201.0) {
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

                            else {
                                if (x[0] <= 106652.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[3] <= 274.5) {
                                if (x[3] <= 120.0) {
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

                        // tree #10
                        if (x[1] <= 101264.0) {
                            if (x[1] <= 100316.0) {
                                if (x[0] <= 94544.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[2] <= 414.5) {
                                        if (x[1] <= 98253.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[1] <= 96689.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[3] <= 271.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #11
                        if (x[0] <= 94544.5) {
                            if (x[2] <= 267.0) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[3] <= 142.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 100821.5) {
                                if (x[2] <= 201.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[3] <= 280.0) {
                                        if (x[1] <= 99354.0) {
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
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #12
                        if (x[1] <= 100659.0) {
                            if (x[3] <= 196.5) {
                                if (x[3] <= 111.5) {
                                    if (x[0] <= 106500.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[0] <= 111118.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[2] <= 406.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[1] <= 97165.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[2] <= 640.0) {
                                    if (x[0] <= 98991.5) {
                                        if (x[3] <= 257.5) {
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
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #13
                        if (x[0] <= 94544.5) {
                            if (x[1] <= 91081.0) {
                                votes[1] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[0] <= 109669.0) {
                                if (x[3] <= 280.0) {
                                    if (x[1] <= 99571.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[3] <= 236.5) {
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

                            else {
                                if (x[2] <= 265.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[1] <= 99555.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }
                        }

                        // tree #14
                        if (x[3] <= 174.0) {
                            if (x[1] <= 90840.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 94593.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[3] <= 118.0) {
                                        if (x[2] <= 261.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[2] <= 388.5) {
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
                            if (x[2] <= 640.0) {
                                if (x[1] <= 95879.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[3] <= 197.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[3] <= 232.0) {
                                            votes[1] += 1;
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

                        // tree #15
                        if (x[2] <= 640.5) {
                            if (x[1] <= 99354.0) {
                                if (x[3] <= 247.5) {
                                    if (x[1] <= 93800.0) {
                                        if (x[3] <= 90.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[2] <= 220.0) {
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

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[1] <= 98466.5) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
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
