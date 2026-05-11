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
                        if (x[1] <= 99625.5) {
                            if (x[0] <= 98668.5) {
                                if (x[0] <= 85930.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[3] <= 159.0) {
                                        if (x[1] <= 93613.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[2] <= 587.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            if (x[3] <= 284.0) {
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
                                if (x[2] <= 190.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[0] <= 98827.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[3] <= 108.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 101042.5) {
                                    if (x[3] <= 111.5) {
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

                        // tree #2
                        if (x[2] <= 598.0) {
                            if (x[2] <= 554.5) {
                                if (x[2] <= 453.0) {
                                    if (x[2] <= 119.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[0] <= 106742.0) {
                                            if (x[2] <= 357.5) {
                                                if (x[1] <= 93412.0) {
                                                    if (x[0] <= 85930.0) {
                                                        votes[0] += 1;
                                                    }

                                                    else {
                                                        if (x[1] <= 90827.5) {
                                                            votes[0] += 1;
                                                        }

                                                        else {
                                                            if (x[1] <= 91185.5) {
                                                                votes[1] += 1;
                                                            }

                                                            else {
                                                                votes[1] += 1;
                                                            }
                                                        }
                                                    }
                                                }

                                                else {
                                                    if (x[0] <= 95588.5) {
                                                        votes[0] += 1;
                                                    }

                                                    else {
                                                        votes[0] += 1;
                                                    }
                                                }
                                            }

                                            else {
                                                if (x[1] <= 93207.0) {
                                                    votes[1] += 1;
                                                }

                                                else {
                                                    if (x[1] <= 98375.5) {
                                                        if (x[2] <= 406.0) {
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
                                        }

                                        else {
                                            if (x[2] <= 277.0) {
                                                if (x[0] <= 110330.5) {
                                                    if (x[3] <= 113.0) {
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
                                                if (x[0] <= 110985.5) {
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
                            }

                            else {
                                if (x[0] <= 95966.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[3] <= 256.0) {
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

                        // tree #3
                        if (x[0] <= 92706.0) {
                            if (x[1] <= 82557.0) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[1] <= 99639.0) {
                                if (x[2] <= 147.0) {
                                    if (x[0] <= 97250.5) {
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
                                        if (x[2] <= 617.5) {
                                            votes[1] += 1;
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
                                }
                            }

                            else {
                                if (x[1] <= 101025.5) {
                                    if (x[2] <= 600.5) {
                                        if (x[2] <= 262.0) {
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
                                    if (x[1] <= 101187.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        // tree #4
                        if (x[2] <= 593.5) {
                            if (x[0] <= 91669.5) {
                                if (x[1] <= 77825.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 474.5) {
                                    if (x[0] <= 97629.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[0] <= 98668.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            if (x[2] <= 276.5) {
                                                if (x[0] <= 110712.0) {
                                                    if (x[3] <= 77.0) {
                                                        votes[1] += 1;
                                                    }

                                                    else {
                                                        if (x[3] <= 108.5) {
                                                            votes[0] += 1;
                                                        }

                                                        else {
                                                            votes[1] += 1;
                                                        }
                                                    }
                                                }

                                                else {
                                                    if (x[2] <= 228.0) {
                                                        votes[0] += 1;
                                                    }

                                                    else {
                                                        votes[0] += 1;
                                                    }
                                                }
                                            }

                                            else {
                                                if (x[0] <= 99218.0) {
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
                            votes[0] += 1;
                        }

                        // tree #5
                        if (x[1] <= 99990.0) {
                            if (x[2] <= 201.0) {
                                if (x[1] <= 91578.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[1] <= 97546.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[0] <= 91669.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[1] <= 97414.0) {
                                        if (x[3] <= 89.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[0] <= 100701.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            if (x[2] <= 267.5) {
                                                votes[0] += 1;
                                            }

                                            else {
                                                votes[0] += 1;
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        else {
                            if (x[3] <= 250.0) {
                                if (x[1] <= 101785.0) {
                                    if (x[0] <= 108977.0) {
                                        if (x[2] <= 335.5) {
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
                                    if (x[3] <= 142.5) {
                                        votes[0] += 1;
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

                        // tree #6
                        if (x[3] <= 204.5) {
                            if (x[0] <= 85178.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 93582.0) {
                                    if (x[0] <= 102555.0) {
                                        if (x[3] <= 46.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            if (x[3] <= 102.5) {
                                                if (x[1] <= 91949.5) {
                                                    votes[0] += 1;
                                                }

                                                else {
                                                    votes[1] += 1;
                                                }
                                            }

                                            else {
                                                if (x[0] <= 87504.0) {
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

                                else {
                                    if (x[0] <= 102349.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[1] <= 99595.0) {
                                            if (x[2] <= 201.0) {
                                                if (x[2] <= 166.5) {
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
                                            if (x[3] <= 108.5) {
                                                votes[0] += 1;
                                            }

                                            else {
                                                if (x[1] <= 101025.5) {
                                                    if (x[2] <= 262.0) {
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
                                }
                            }
                        }

                        else {
                            votes[0] += 1;
                        }

                        // tree #7
                        if (x[1] <= 100659.0) {
                            if (x[2] <= 399.0) {
                                if (x[1] <= 90873.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[2] <= 194.5) {
                                        if (x[1] <= 97629.0) {
                                            if (x[1] <= 92745.0) {
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
                                        if (x[3] <= 148.5) {
                                            if (x[0] <= 109663.5) {
                                                votes[0] += 1;
                                            }

                                            else {
                                                if (x[0] <= 110712.0) {
                                                    votes[1] += 1;
                                                }

                                                else {
                                                    votes[0] += 1;
                                                }
                                            }
                                        }

                                        else {
                                            if (x[0] <= 100652.5) {
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
                                if (x[3] <= 143.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 646.0) {
                                if (x[3] <= 204.5) {
                                    if (x[1] <= 101267.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    if (x[2] <= 512.0) {
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

                        // tree #8
                        if (x[1] <= 97030.0) {
                            if (x[3] <= 60.0) {
                                if (x[1] <= 77542.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[2] <= 114.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[0] <= 89789.5) {
                                    if (x[3] <= 119.0) {
                                        if (x[1] <= 73615.5) {
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
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 662.0) {
                                if (x[1] <= 101025.5) {
                                    if (x[0] <= 106500.5) {
                                        if (x[3] <= 195.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            if (x[2] <= 586.5) {
                                                if (x[2] <= 511.5) {
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
                                        if (x[2] <= 166.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            if (x[0] <= 111614.5) {
                                                if (x[2] <= 296.5) {
                                                    votes[1] += 1;
                                                }

                                                else {
                                                    if (x[3] <= 129.0) {
                                                        votes[0] += 1;
                                                    }

                                                    else {
                                                        votes[1] += 1;
                                                    }
                                                }
                                            }

                                            else {
                                                if (x[2] <= 352.0) {
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
                                    if (x[1] <= 101412.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
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

                        // tree #9
                        if (x[1] <= 97030.0) {
                            if (x[1] <= 93073.5) {
                                if (x[1] <= 90954.0) {
                                    if (x[2] <= 113.0) {
                                        if (x[1] <= 76364.5) {
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
                                    if (x[2] <= 538.5) {
                                        if (x[0] <= 88219.0) {
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
                                if (x[3] <= 111.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 111067.0) {
                                if (x[1] <= 101042.5) {
                                    if (x[2] <= 644.5) {
                                        if (x[3] <= 195.5) {
                                            if (x[1] <= 99625.5) {
                                                if (x[2] <= 218.5) {
                                                    if (x[0] <= 108921.5) {
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
                                                votes[1] += 1;
                                            }
                                        }

                                        else {
                                            if (x[2] <= 462.5) {
                                                votes[1] += 1;
                                            }

                                            else {
                                                votes[1] += 1;
                                            }
                                        }
                                    }

                                    else {
                                        if (x[2] <= 667.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[3] <= 125.5) {
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

                        // tree #10
                        if (x[3] <= 174.0) {
                            if (x[3] <= 57.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 149.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[2] <= 398.5) {
                                        if (x[2] <= 350.0) {
                                            if (x[1] <= 96834.5) {
                                                votes[0] += 1;
                                            }

                                            else {
                                                if (x[2] <= 263.5) {
                                                    if (x[0] <= 109890.5) {
                                                        if (x[1] <= 97272.5) {
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
                                                    if (x[0] <= 110918.5) {
                                                        if (x[1] <= 98337.0) {
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
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[3] <= 156.0) {
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
                            if (x[2] <= 640.0) {
                                if (x[1] <= 97218.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[2] <= 473.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[3] <= 255.5) {
                                            if (x[0] <= 106703.0) {
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
                                }
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #11
                        if (x[2] <= 276.5) {
                            if (x[3] <= 57.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 77825.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[1] <= 103826.5) {
                                        if (x[1] <= 96834.5) {
                                            if (x[3] <= 73.5) {
                                                votes[1] += 1;
                                            }

                                            else {
                                                if (x[2] <= 206.0) {
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
                            }
                        }

                        else {
                            if (x[3] <= 232.0) {
                                if (x[0] <= 93233.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[2] <= 446.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[0] <= 109240.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            if (x[2] <= 603.0) {
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
                                if (x[3] <= 233.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        // tree #12
                        if (x[3] <= 118.0) {
                            if (x[3] <= 110.0) {
                                if (x[0] <= 107011.0) {
                                    if (x[0] <= 97356.5) {
                                        if (x[0] <= 81241.5) {
                                            if (x[3] <= 39.0) {
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
                                        if (x[1] <= 98145.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[2] <= 219.5) {
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
                            if (x[2] <= 640.0) {
                                if (x[1] <= 97495.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[3] <= 153.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[2] <= 453.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            if (x[1] <= 101071.5) {
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
                                if (x[1] <= 91215.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        // tree #13
                        if (x[2] <= 593.5) {
                            if (x[1] <= 77825.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 90896.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[2] <= 190.5) {
                                        if (x[1] <= 91394.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[3] <= 198.0) {
                                            if (x[2] <= 446.0) {
                                                if (x[2] <= 261.5) {
                                                    if (x[3] <= 109.5) {
                                                        if (x[3] <= 89.0) {
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
                                            if (x[1] <= 96670.5) {
                                                votes[0] += 1;
                                            }

                                            else {
                                                if (x[3] <= 230.5) {
                                                    if (x[0] <= 102834.5) {
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
                                }
                            }
                        }

                        else {
                            votes[0] += 1;
                        }

                        // tree #14
                        if (x[3] <= 205.5) {
                            if (x[1] <= 91072.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 101025.5) {
                                    if (x[0] <= 108617.5) {
                                        if (x[0] <= 97629.0) {
                                            if (x[0] <= 92432.5) {
                                                votes[1] += 1;
                                            }

                                            else {
                                                votes[0] += 1;
                                            }
                                        }

                                        else {
                                            if (x[3] <= 133.0) {
                                                if (x[1] <= 98055.0) {
                                                    votes[1] += 1;
                                                }

                                                else {
                                                    if (x[3] <= 96.5) {
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
                                                    votes[1] += 1;
                                                }
                                            }
                                        }
                                    }

                                    else {
                                        if (x[2] <= 405.5) {
                                            if (x[1] <= 97303.0) {
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
                                    if (x[2] <= 276.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[0] <= 99275.5) {
                                if (x[0] <= 95942.0) {
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

                        // tree #15
                        if (x[2] <= 640.0) {
                            if (x[1] <= 101626.0) {
                                if (x[1] <= 99639.0) {
                                    if (x[0] <= 98668.5) {
                                        if (x[2] <= 398.5) {
                                            if (x[1] <= 77825.0) {
                                                votes[0] += 1;
                                            }

                                            else {
                                                if (x[1] <= 93613.0) {
                                                    votes[1] += 1;
                                                }

                                                else {
                                                    votes[0] += 1;
                                                }
                                            }
                                        }

                                        else {
                                            if (x[1] <= 96804.0) {
                                                votes[0] += 1;
                                            }

                                            else {
                                                votes[1] += 1;
                                            }
                                        }
                                    }

                                    else {
                                        if (x[3] <= 83.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[1] <= 101025.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[2] <= 512.0) {
                                            if (x[2] <= 430.5) {
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
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        else {
                            votes[0] += 1;
                        }

                        // tree #16
                        if (x[2] <= 598.0) {
                            if (x[0] <= 85538.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 101626.0) {
                                    if (x[2] <= 586.5) {
                                        if (x[2] <= 565.5) {
                                            if (x[0] <= 92432.5) {
                                                votes[1] += 1;
                                            }

                                            else {
                                                if (x[3] <= 192.5) {
                                                    if (x[2] <= 277.0) {
                                                        if (x[1] <= 91949.5) {
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
                                                        if (x[2] <= 446.0) {
                                                            if (x[0] <= 111010.5) {
                                                                votes[0] += 1;
                                                            }

                                                            else {
                                                                votes[1] += 1;
                                                            }
                                                        }

                                                        else {
                                                            if (x[0] <= 113345.5) {
                                                                votes[1] += 1;
                                                            }

                                                            else {
                                                                votes[0] += 1;
                                                            }
                                                        }
                                                    }
                                                }

                                                else {
                                                    if (x[1] <= 98790.0) {
                                                        votes[0] += 1;
                                                    }

                                                    else {
                                                        if (x[3] <= 204.5) {
                                                            votes[1] += 1;
                                                        }

                                                        else {
                                                            if (x[3] <= 219.5) {
                                                                votes[0] += 1;
                                                            }

                                                            else {
                                                                votes[1] += 1;
                                                            }
                                                        }
                                                    }
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
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            votes[0] += 1;
                        }

                        // tree #17
                        if (x[0] <= 92432.5) {
                            if (x[3] <= 39.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 77804.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 100003.5) {
                                if (x[3] <= 61.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[3] <= 295.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[3] <= 304.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[0] <= 109932.5) {
                                    if (x[2] <= 270.5) {
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

                        // tree #18
                        if (x[1] <= 97000.5) {
                            if (x[2] <= 107.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[2] <= 398.5) {
                                    if (x[3] <= 141.5) {
                                        if (x[2] <= 125.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[2] <= 350.0) {
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
                            if (x[2] <= 636.5) {
                                if (x[3] <= 230.5) {
                                    if (x[0] <= 106500.5) {
                                        if (x[3] <= 195.5) {
                                            if (x[3] <= 121.0) {
                                                votes[0] += 1;
                                            }

                                            else {
                                                votes[0] += 1;
                                            }
                                        }

                                        else {
                                            if (x[0] <= 102834.5) {
                                                votes[1] += 1;
                                            }

                                            else {
                                                votes[0] += 1;
                                            }
                                        }
                                    }

                                    else {
                                        if (x[3] <= 169.5) {
                                            if (x[2] <= 166.5) {
                                                votes[0] += 1;
                                            }

                                            else {
                                                if (x[2] <= 218.5) {
                                                    votes[1] += 1;
                                                }

                                                else {
                                                    if (x[1] <= 99505.0) {
                                                        votes[0] += 1;
                                                    }

                                                    else {
                                                        votes[1] += 1;
                                                    }
                                                }
                                            }
                                        }

                                        else {
                                            if (x[2] <= 473.0) {
                                                votes[0] += 1;
                                            }

                                            else {
                                                votes[1] += 1;
                                            }
                                        }
                                    }
                                }

                                else {
                                    votes[1] += 1;
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

                        // tree #19
                        if (x[0] <= 98785.5) {
                            if (x[2] <= 617.5) {
                                if (x[1] <= 91570.5) {
                                    if (x[3] <= 62.0) {
                                        if (x[0] <= 88598.0) {
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
                                    if (x[2] <= 389.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[3] <= 233.5) {
                                            if (x[1] <= 94090.5) {
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
                            }

                            else {
                                if (x[1] <= 92005.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[3] <= 96.0) {
                                if (x[0] <= 107011.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[1] <= 98329.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[1] <= 100517.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[3] <= 193.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[0] <= 109932.5) {
                                            if (x[2] <= 448.5) {
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
                        }

                        // tree #20
                        if (x[2] <= 201.0) {
                            if (x[0] <= 97356.5) {
                                if (x[1] <= 77825.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[2] <= 113.0) {
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
                            if (x[1] <= 97554.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 419.5) {
                                    if (x[1] <= 99625.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[3] <= 126.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            if (x[2] <= 276.0) {
                                                votes[0] += 1;
                                            }

                                            else {
                                                votes[0] += 1;
                                            }
                                        }
                                    }
                                }

                                else {
                                    if (x[1] <= 100736.0) {
                                        if (x[3] <= 304.0) {
                                            if (x[3] <= 227.0) {
                                                votes[1] += 1;
                                            }

                                            else {
                                                if (x[3] <= 277.5) {
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

                                    else {
                                        if (x[0] <= 110785.5) {
                                            if (x[0] <= 108385.5) {
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
                            }
                        }

                        // tree #21
                        if (x[1] <= 91026.5) {
                            votes[0] += 1;
                        }

                        else {
                            if (x[2] <= 640.0) {
                                if (x[1] <= 93482.0) {
                                    if (x[3] <= 102.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[1] <= 91156.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[1] <= 97030.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[0] <= 102759.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            if (x[1] <= 99595.0) {
                                                if (x[2] <= 229.0) {
                                                    if (x[2] <= 177.0) {
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
                                                if (x[0] <= 105951.0) {
                                                    votes[0] += 1;
                                                }

                                                else {
                                                    if (x[1] <= 101626.0) {
                                                        votes[1] += 1;
                                                    }

                                                    else {
                                                        if (x[0] <= 108692.0) {
                                                            votes[0] += 1;
                                                        }

                                                        else {
                                                            votes[0] += 1;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #22
                        if (x[2] <= 644.5) {
                            if (x[3] <= 267.0) {
                                if (x[2] <= 453.0) {
                                    if (x[3] <= 192.5) {
                                        if (x[2] <= 268.0) {
                                            if (x[3] <= 57.0) {
                                                if (x[0] <= 97421.5) {
                                                    if (x[0] <= 82414.5) {
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
                                                if (x[1] <= 77825.0) {
                                                    votes[0] += 1;
                                                }

                                                else {
                                                    if (x[1] <= 97972.5) {
                                                        if (x[0] <= 111387.5) {
                                                            if (x[2] <= 205.0) {
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
                                            }
                                        }

                                        else {
                                            if (x[0] <= 93233.0) {
                                                votes[1] += 1;
                                            }

                                            else {
                                                if (x[2] <= 446.0) {
                                                    if (x[1] <= 90785.5) {
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
                            votes[0] += 1;
                        }

                        // tree #23
                        if (x[2] <= 399.5) {
                            if (x[1] <= 94023.5) {
                                if (x[0] <= 84256.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[1] <= 90827.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[3] <= 108.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[3] <= 135.0) {
                                        if (x[1] <= 99255.5) {
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
                            if (x[1] <= 101510.0) {
                                if (x[0] <= 98723.0) {
                                    if (x[3] <= 284.0) {
                                        if (x[0] <= 96422.0) {
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

                            else {
                                if (x[2] <= 510.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #24
                        if (x[1] <= 93194.0) {
                            if (x[2] <= 398.5) {
                                if (x[3] <= 57.0) {
                                    if (x[2] <= 114.0) {
                                        if (x[1] <= 81987.5) {
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
                                    if (x[0] <= 85178.5) {
                                        if (x[0] <= 81241.5) {
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
                                if (x[2] <= 406.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[3] <= 113.0) {
                                if (x[1] <= 95687.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[3] <= 86.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[2] <= 277.0) {
                                            if (x[2] <= 227.0) {
                                                votes[1] += 1;
                                            }

                                            else {
                                                if (x[3] <= 103.5) {
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
                            }

                            else {
                                if (x[1] <= 101510.0) {
                                    if (x[2] <= 282.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    if (x[2] <= 418.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }
                        }

                        // tree #25
                        if (x[1] <= 99990.0) {
                            if (x[1] <= 93274.5) {
                                if (x[2] <= 404.5) {
                                    if (x[3] <= 121.0) {
                                        if (x[0] <= 97421.5) {
                                            if (x[0] <= 81078.0) {
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
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    if (x[1] <= 90667.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[0] <= 98483.5) {
                                    if (x[3] <= 233.0) {
                                        if (x[1] <= 94121.5) {
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
                                    if (x[2] <= 201.0) {
                                        if (x[0] <= 108921.5) {
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

                        else {
                            if (x[1] <= 101626.0) {
                                if (x[2] <= 639.5) {
                                    if (x[0] <= 105951.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    if (x[3] <= 274.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[2] <= 333.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        // tree #26
                        if (x[0] <= 92432.5) {
                            if (x[0] <= 85308.5) {
                                if (x[0] <= 81241.5) {
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
                            if (x[2] <= 205.0) {
                                if (x[1] <= 97629.0) {
                                    if (x[0] <= 97315.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[0] <= 102747.0) {
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
                                if (x[1] <= 100664.5) {
                                    if (x[0] <= 98459.0) {
                                        if (x[1] <= 96390.0) {
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
                                    if (x[1] <= 100977.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        // tree #27
                        if (x[3] <= 174.0) {
                            if (x[0] <= 85930.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 382.5) {
                                    if (x[3] <= 54.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[0] <= 92218.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            if (x[1] <= 99625.5) {
                                                if (x[2] <= 201.0) {
                                                    votes[1] += 1;
                                                }

                                                else {
                                                    votes[0] += 1;
                                                }
                                            }

                                            else {
                                                if (x[0] <= 105542.5) {
                                                    votes[0] += 1;
                                                }

                                                else {
                                                    if (x[3] <= 108.5) {
                                                        votes[0] += 1;
                                                    }

                                                    else {
                                                        votes[1] += 1;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }

                                else {
                                    if (x[1] <= 93720.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[0] <= 109932.5) {
                                if (x[1] <= 97414.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[0] <= 100701.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[0] <= 110312.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        // tree #28
                        if (x[2] <= 453.0) {
                            if (x[1] <= 77825.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 92432.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[2] <= 277.0) {
                                        if (x[0] <= 97315.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            if (x[3] <= 113.0) {
                                                if (x[0] <= 110330.5) {
                                                    if (x[1] <= 98055.0) {
                                                        votes[1] += 1;
                                                    }

                                                    else {
                                                        if (x[3] <= 97.0) {
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

                                            else {
                                                votes[0] += 1;
                                            }
                                        }
                                    }

                                    else {
                                        if (x[2] <= 435.5) {
                                            if (x[0] <= 94396.5) {
                                                votes[0] += 1;
                                            }

                                            else {
                                                votes[0] += 1;
                                            }
                                        }

                                        else {
                                            if (x[0] <= 101124.5) {
                                                votes[0] += 1;
                                            }

                                            else {
                                                votes[1] += 1;
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        else {
                            if (x[0] <= 99300.0) {
                                if (x[1] <= 96309.0) {
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

                        // tree #29
                        if (x[0] <= 98720.5) {
                            if (x[3] <= 267.0) {
                                if (x[0] <= 98077.5) {
                                    if (x[0] <= 91669.5) {
                                        if (x[2] <= 264.5) {
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
                                if (x[0] <= 95611.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 218.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 100659.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[2] <= 270.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[2] <= 447.0) {
                                            if (x[1] <= 100977.0) {
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
                        }

                        // tree #30
                        if (x[0] <= 98603.5) {
                            if (x[0] <= 97484.5) {
                                if (x[0] <= 90896.5) {
                                    if (x[1] <= 77804.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[2] <= 229.0) {
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
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[1] <= 97030.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 97448.5) {
                                    if (x[3] <= 141.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    if (x[0] <= 109932.5) {
                                        if (x[3] <= 105.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[3] <= 119.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            if (x[2] <= 641.5) {
                                                votes[1] += 1;
                                            }

                                            else {
                                                votes[0] += 1;
                                            }
                                        }
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

                protected:
                };
            }
        }
    }