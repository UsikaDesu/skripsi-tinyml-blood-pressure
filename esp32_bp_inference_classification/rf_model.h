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
                        if (x[1] <= 100203.0) {
                            if (x[0] <= 91958.5) {
                                if (x[2] <= 224.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 71.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 103805.5) {
                                if (x[3] <= 240.0) {
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

                        // tree #2
                        if (x[2] <= 593.5) {
                            if (x[2] <= 471.5) {
                                if (x[2] <= 150.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[3] <= 111.5) {
                                        if (x[2] <= 205.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[0] <= 92470.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[1] <= 97218.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[3] <= 247.0) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #3
                        if (x[2] <= 277.0) {
                            if (x[3] <= 58.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 97972.5) {
                                    if (x[2] <= 205.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    if (x[0] <= 107598.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[1] <= 100003.5) {
                                if (x[0] <= 93029.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 110785.5) {
                                    if (x[1] <= 100736.0) {
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

                        // tree #4
                        if (x[2] <= 593.5) {
                            if (x[1] <= 101626.0) {
                                if (x[1] <= 99651.5) {
                                    if (x[2] <= 162.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[0] <= 91697.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[1] <= 101373.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        else {
                            if (x[3] <= 244.0) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #5
                        if (x[3] <= 232.5) {
                            if (x[1] <= 101785.0) {
                                if (x[1] <= 99518.5) {
                                    if (x[1] <= 93348.5) {
                                        if (x[3] <= 105.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[0] <= 107011.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[3] <= 204.5) {
                                        if (x[0] <= 110285.5) {
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

                            else {
                                votes[0] += 1;
                            }
                        }

                        else {
                            if (x[2] <= 623.0) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #6
                        if (x[3] <= 191.5) {
                            if (x[2] <= 283.5) {
                                if (x[0] <= 107598.5) {
                                    if (x[3] <= 69.0) {
                                        if (x[1] <= 83059.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[3] <= 88.5) {
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
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        else {
                            if (x[2] <= 593.5) {
                                if (x[0] <= 109240.5) {
                                    if (x[0] <= 102444.5) {
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
                                if (x[1] <= 91828.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        // tree #7
                        if (x[1] <= 99796.5) {
                            if (x[0] <= 90455.0) {
                                if (x[3] <= 96.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 205.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[3] <= 277.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[1] <= 94942.5) {
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
                            if (x[3] <= 250.0) {
                                if (x[0] <= 101634.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[1] <= 101626.0) {
                                        if (x[0] <= 108977.0) {
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
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #8
                        if (x[1] <= 100216.5) {
                            if (x[1] <= 93073.5) {
                                if (x[1] <= 90908.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[2] <= 507.5) {
                                        if (x[2] <= 250.5) {
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
                            }

                            else {
                                if (x[2] <= 218.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[3] <= 240.0) {
                                if (x[0] <= 102327.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[1] <= 101373.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #9
                        if (x[0] <= 98603.5) {
                            if (x[2] <= 384.0) {
                                if (x[1] <= 82299.5) {
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
                            if (x[1] <= 100203.0) {
                                if (x[3] <= 92.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[2] <= 256.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[3] <= 225.0) {
                                    if (x[1] <= 101648.0) {
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

                        // tree #10
                        if (x[1] <= 99625.5) {
                            if (x[1] <= 91231.0) {
                                if (x[2] <= 201.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 205.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[2] <= 587.5) {
                                        if (x[2] <= 237.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[2] <= 624.5) {
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
                            if (x[0] <= 109669.0) {
                                if (x[1] <= 101098.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 110785.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #11
                        if (x[2] <= 593.5) {
                            if (x[2] <= 414.5) {
                                if (x[1] <= 99625.5) {
                                    if (x[1] <= 91185.5) {
                                        if (x[1] <= 90785.5) {
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
                                    if (x[3] <= 143.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[1] <= 97414.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[3] <= 203.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[3] <= 230.5) {
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
                            if (x[1] <= 92005.5) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #12
                        if (x[3] <= 232.5) {
                            if (x[0] <= 92470.0) {
                                if (x[3] <= 135.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 106766.0) {
                                    if (x[1] <= 99621.0) {
                                        if (x[3] <= 93.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[1] <= 106087.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[2] <= 325.0) {
                                        if (x[0] <= 108114.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[1] <= 99103.0) {
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
                            if (x[3] <= 243.5) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #13
                        if (x[2] <= 508.0) {
                            if (x[1] <= 99625.5) {
                                if (x[0] <= 92470.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[2] <= 218.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[1] <= 92898.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[0] <= 109283.5) {
                                    if (x[2] <= 436.0) {
                                        if (x[1] <= 101214.5) {
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
                                    if (x[3] <= 165.5) {
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

                        // tree #14
                        if (x[0] <= 109663.5) {
                            if (x[0] <= 102153.5) {
                                if (x[1] <= 96689.5) {
                                    if (x[0] <= 90455.0) {
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
                            if (x[0] <= 112278.5) {
                                if (x[0] <= 110372.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[2] <= 433.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #15
                        if (x[2] <= 575.0) {
                            if (x[1] <= 99858.5) {
                                if (x[1] <= 93258.5) {
                                    if (x[1] <= 90840.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[3] <= 105.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[2] <= 237.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[1] <= 101626.0) {
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

                        // tree #16
                        if (x[2] <= 593.5) {
                            if (x[0] <= 92470.0) {
                                if (x[0] <= 88219.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 193.5) {
                                    if (x[2] <= 277.0) {
                                        if (x[0] <= 107598.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[3] <= 135.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[0] <= 97957.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[1] <= 97218.5) {
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
                            if (x[0] <= 102238.5) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #17
                        if (x[0] <= 102640.5) {
                            if (x[3] <= 102.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 103361.5) {
                                    if (x[0] <= 92470.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[1] <= 95845.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 575.0) {
                                if (x[1] <= 99858.5) {
                                    if (x[2] <= 218.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    if (x[0] <= 109547.0) {
                                        if (x[0] <= 108155.5) {
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
                                votes[0] += 1;
                            }
                        }

                        // tree #18
                        if (x[1] <= 100203.0) {
                            if (x[2] <= 205.0) {
                                if (x[3] <= 58.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 93029.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[2] <= 586.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[2] <= 645.5) {
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
                            if (x[2] <= 625.0) {
                                if (x[3] <= 191.5) {
                                    if (x[3] <= 142.5) {
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

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #19
                        if (x[2] <= 497.5) {
                            if (x[0] <= 102578.5) {
                                if (x[1] <= 90996.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[1] <= 103361.5) {
                                        if (x[0] <= 91956.5) {
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
                            }

                            else {
                                if (x[1] <= 99639.0) {
                                    if (x[3] <= 92.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[1] <= 97002.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[1] <= 101098.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[3] <= 236.5) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #20
                        if (x[2] <= 582.0) {
                            if (x[0] <= 92470.0) {
                                if (x[0] <= 85538.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 190.5) {
                                    if (x[3] <= 92.5) {
                                        if (x[2] <= 180.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[1] <= 99589.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[3] <= 203.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[2] <= 518.5) {
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
                            if (x[3] <= 243.5) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #21
                        if (x[1] <= 99990.0) {
                            if (x[0] <= 98603.5) {
                                if (x[1] <= 82329.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[3] <= 174.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[1] <= 94587.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[3] <= 92.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[0] <= 101566.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[1] <= 101626.0) {
                                if (x[0] <= 109669.0) {
                                    if (x[2] <= 547.0) {
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
                                if (x[0] <= 105772.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        // tree #22
                        if (x[2] <= 593.5) {
                            if (x[1] <= 99639.0) {
                                if (x[2] <= 205.0) {
                                    if (x[3] <= 52.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    if (x[3] <= 232.0) {
                                        if (x[2] <= 366.0) {
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
                                if (x[0] <= 109547.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            votes[0] += 1;
                        }

                        // tree #23
                        if (x[2] <= 593.5) {
                            if (x[1] <= 99625.5) {
                                if (x[0] <= 107060.5) {
                                    if (x[1] <= 97569.0) {
                                        if (x[3] <= 58.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[3] <= 119.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[1] <= 96777.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[1] <= 103473.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 102238.5) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #24
                        if (x[1] <= 100216.5) {
                            if (x[0] <= 91943.0) {
                                if (x[2] <= 224.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 277.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[2] <= 663.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[0] <= 109547.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 100994.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #25
                        if (x[3] <= 237.5) {
                            if (x[3] <= 153.0) {
                                if (x[0] <= 106766.0) {
                                    if (x[3] <= 69.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[0] <= 96910.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[3] <= 137.0) {
                                        if (x[1] <= 99505.0) {
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
                                if (x[2] <= 453.0) {
                                    if (x[3] <= 163.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[1] <= 96023.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[2] <= 473.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[0] <= 110285.5) {
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
                            if (x[0] <= 99606.5) {
                                if (x[1] <= 93564.5) {
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

                        // tree #26
                        if (x[0] <= 98483.5) {
                            if (x[2] <= 617.5) {
                                if (x[2] <= 237.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[1] <= 93117.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        else {
                            if (x[1] <= 99990.0) {
                                if (x[1] <= 97032.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[3] <= 100.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[1] <= 101373.0) {
                                    if (x[2] <= 561.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    if (x[1] <= 101813.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        // tree #27
                        if (x[3] <= 204.5) {
                            if (x[0] <= 92470.0) {
                                if (x[2] <= 267.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 445.0) {
                                    if (x[3] <= 136.5) {
                                        if (x[2] <= 335.0) {
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
                                    if (x[2] <= 459.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[0] <= 99359.0) {
                                if (x[2] <= 617.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[1] <= 93609.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        // tree #28
                        if (x[3] <= 204.0) {
                            if (x[1] <= 99651.5) {
                                if (x[0] <= 92729.5) {
                                    if (x[0] <= 85553.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    if (x[2] <= 205.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[1] <= 91499.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[3] <= 171.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[3] <= 213.0) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #29
                        if (x[3] <= 216.5) {
                            if (x[3] <= 153.0) {
                                if (x[2] <= 274.5) {
                                    if (x[2] <= 246.0) {
                                        if (x[2] <= 205.0) {
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
                                    if (x[0] <= 111385.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[2] <= 398.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[1] <= 98372.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[3] <= 191.5) {
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
                            if (x[3] <= 243.5) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #30
                        if (x[0] <= 102578.5) {
                            if (x[2] <= 450.0) {
                                if (x[1] <= 90996.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[1] <= 93348.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[2] <= 414.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[3] <= 239.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 100650.5) {
                                if (x[3] <= 92.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 110785.5) {
                                    if (x[1] <= 101373.0) {
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

                        // tree #31
                        if (x[0] <= 91943.0) {
                            if (x[2] <= 224.5) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[0] <= 109958.0) {
                                if (x[0] <= 102153.5) {
                                    if (x[2] <= 414.5) {
                                        votes[0] += 1;
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

                                else {
                                    if (x[3] <= 116.5) {
                                        if (x[0] <= 106585.5) {
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
                                if (x[1] <= 99103.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[2] <= 524.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }
                        }

                        // tree #32
                        if (x[1] <= 100216.5) {
                            if (x[3] <= 179.5) {
                                if (x[0] <= 91697.0) {
                                    if (x[1] <= 82375.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    if (x[2] <= 205.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[2] <= 237.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[2] <= 485.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 101785.0) {
                                if (x[3] <= 240.0) {
                                    if (x[1] <= 101373.0) {
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
                                votes[0] += 1;
                            }
                        }

                        // tree #33
                        if (x[0] <= 91943.0) {
                            if (x[0] <= 85538.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 91476.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 99608.5) {
                                if (x[2] <= 205.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[0] <= 98483.5) {
                                        if (x[0] <= 95641.5) {
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
                                if (x[3] <= 204.5) {
                                    if (x[1] <= 101648.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    if (x[1] <= 100983.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        // tree #34
                        if (x[0] <= 91943.0) {
                            if (x[2] <= 224.5) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[3] <= 113.5) {
                                if (x[0] <= 108779.5) {
                                    if (x[2] <= 270.0) {
                                        if (x[1] <= 97380.5) {
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
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 203.0) {
                                    if (x[2] <= 414.5) {
                                        if (x[3] <= 128.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[1] <= 98771.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[0] <= 94429.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        // tree #35
                        if (x[2] <= 575.0) {
                            if (x[3] <= 153.0) {
                                if (x[2] <= 271.5) {
                                    if (x[3] <= 104.0) {
                                        if (x[2] <= 205.0) {
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
                                    if (x[3] <= 111.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[1] <= 99103.0) {
                                    if (x[0] <= 91956.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    if (x[3] <= 203.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[1] <= 92005.5) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #36
                        if (x[2] <= 575.0) {
                            if (x[1] <= 99595.0) {
                                if (x[1] <= 93117.0) {
                                    if (x[2] <= 250.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[1] <= 91081.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[1] <= 95210.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[1] <= 101626.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[3] <= 147.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[1] <= 93624.0) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #37
                        if (x[2] <= 611.5) {
                            if (x[2] <= 445.0) {
                                if (x[0] <= 92470.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[1] <= 99853.0) {
                                        if (x[0] <= 106766.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[3] <= 136.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[1] <= 97414.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[1] <= 101373.0) {
                                        if (x[3] <= 215.5) {
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
                            }
                        }

                        else {
                            votes[0] += 1;
                        }

                        // tree #38
                        if (x[0] <= 102759.0) {
                            if (x[1] <= 90996.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 598.0) {
                                    if (x[0] <= 92729.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[3] <= 195.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 298.5) {
                                if (x[3] <= 103.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 111617.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[1] <= 98786.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }
                        }

                        // tree #39
                        if (x[3] <= 282.0) {
                            if (x[3] <= 277.0) {
                                if (x[3] <= 202.5) {
                                    if (x[1] <= 99651.5) {
                                        if (x[1] <= 97225.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[0] <= 105542.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[0] <= 100708.5) {
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
                        }

                        else {
                            votes[0] += 1;
                        }

                        // tree #40
                        if (x[0] <= 91943.0) {
                            if (x[0] <= 87127.0) {
                                votes[1] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[2] <= 593.5) {
                                if (x[2] <= 414.5) {
                                    if (x[1] <= 97462.0) {
                                        if (x[2] <= 205.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[2] <= 277.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[0] <= 109405.0) {
                                        if (x[2] <= 447.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[3] <= 176.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[3] <= 243.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        // tree #41
                        if (x[2] <= 279.0) {
                            if (x[1] <= 82299.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 106585.5) {
                                    if (x[0] <= 102315.5) {
                                        if (x[2] <= 250.5) {
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
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[3] <= 162.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 100618.0) {
                                    if (x[2] <= 611.5) {
                                        if (x[3] <= 185.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[2] <= 644.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[2] <= 470.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }
                        }

                        // tree #42
                        if (x[0] <= 109663.5) {
                            if (x[0] <= 102578.5) {
                                if (x[2] <= 250.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[3] <= 123.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[1] <= 97414.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[3] <= 92.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 99796.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[3] <= 216.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #43
                        if (x[1] <= 100203.0) {
                            if (x[0] <= 98603.5) {
                                if (x[3] <= 179.5) {
                                    if (x[0] <= 85538.0) {
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
                                if (x[3] <= 92.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[1] <= 92913.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[3] <= 204.5) {
                                if (x[3] <= 191.5) {
                                    if (x[1] <= 101219.0) {
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

                        // tree #44
                        if (x[0] <= 99479.0) {
                            if (x[1] <= 82299.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 93171.0) {
                                    if (x[1] <= 91156.5) {
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

                        else {
                            if (x[1] <= 100203.0) {
                                if (x[2] <= 218.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 253.0) {
                                    if (x[2] <= 366.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[0] <= 109878.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        // tree #45
                        if (x[2] <= 598.0) {
                            if (x[1] <= 99639.0) {
                                if (x[2] <= 586.0) {
                                    if (x[1] <= 93258.5) {
                                        if (x[0] <= 85930.0) {
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
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[1] <= 101626.0) {
                                    if (x[1] <= 101373.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    if (x[1] <= 103992.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[2] <= 638.0) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #46
                        if (x[3] <= 232.5) {
                            if (x[3] <= 108.5) {
                                if (x[2] <= 205.0) {
                                    if (x[3] <= 58.0) {
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
                                if (x[0] <= 90982.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[0] <= 109675.5) {
                                        if (x[1] <= 99853.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[1] <= 99103.0) {
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
                            votes[0] += 1;
                        }

                        // tree #47
                        if (x[2] <= 614.5) {
                            if (x[1] <= 99595.0) {
                                if (x[1] <= 97569.0) {
                                    if (x[1] <= 91072.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[1] <= 93258.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[1] <= 97791.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[1] <= 101626.0) {
                                    if (x[1] <= 101373.0) {
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
                        }

                        else {
                            votes[0] += 1;
                        }

                        // tree #48
                        if (x[2] <= 497.5) {
                            if (x[3] <= 108.5) {
                                if (x[0] <= 106766.0) {
                                    if (x[3] <= 50.0) {
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
                                if (x[1] <= 101373.0) {
                                    if (x[1] <= 90996.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[0] <= 110372.0) {
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
                        }

                        else {
                            if (x[3] <= 214.0) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #49
                        if (x[1] <= 99990.0) {
                            if (x[2] <= 276.0) {
                                if (x[1] <= 82299.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[2] <= 261.0) {
                                        if (x[0] <= 105837.0) {
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

                            else {
                                if (x[1] <= 91842.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[3] <= 277.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[3] <= 296.0) {
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
                            if (x[0] <= 109669.0) {
                                if (x[0] <= 108155.5) {
                                    if (x[2] <= 283.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[2] <= 356.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[3] <= 198.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[3] <= 214.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #50
                        if (x[0] <= 91943.0) {
                            votes[1] += 1;
                        }

                        else {
                            if (x[2] <= 593.5) {
                                if (x[3] <= 108.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[1] <= 97220.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[2] <= 408.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[3] <= 246.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
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

                protected:
                };
            }
        }
    }