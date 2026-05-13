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
                                        if (x[1] <= 92946.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[2] <= 587.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[3] <= 84.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 270.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 100962.5) {
                                    votes[1] += 1;
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
                                votes[1] += 1;
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
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[0] <= 98483.5) {
                                        if (x[1] <= 95546.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
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
                            }

                            else {
                                if (x[1] <= 101025.5) {
                                    if (x[3] <= 177.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    if (x[2] <= 332.0) {
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
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[3] <= 232.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[2] <= 576.5) {
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
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[1] <= 92932.0) {
                                    if (x[0] <= 91958.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    if (x[0] <= 98483.5) {
                                        if (x[3] <= 219.5) {
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
                            if (x[0] <= 106721.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 453.0) {
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

                        // tree #6
                        if (x[3] <= 204.5) {
                            if (x[0] <= 85178.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 93582.0) {
                                    if (x[0] <= 93754.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[1] <= 91949.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[1] <= 99595.0) {
                                        if (x[2] <= 201.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[0] <= 105951.0) {
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
                                        if (x[1] <= 95103.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[3] <= 148.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[3] <= 165.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[3] <= 204.5) {
                                if (x[1] <= 100832.5) {
                                    if (x[2] <= 382.0) {
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
                                votes[1] += 1;
                            }
                        }

                        // tree #8
                        if (x[1] <= 97030.0) {
                            if (x[3] <= 60.0) {
                                if (x[1] <= 85877.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[1] <= 91231.0) {
                                    if (x[0] <= 90590.0) {
                                        votes[0] += 1;
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
                            if (x[2] <= 662.0) {
                                if (x[1] <= 101025.5) {
                                    if (x[0] <= 106500.5) {
                                        if (x[3] <= 195.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[3] <= 129.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[1] <= 101857.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[2] <= 716.0) {
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
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    if (x[3] <= 240.0) {
                                        if (x[2] <= 365.5) {
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
                                if (x[0] <= 96834.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 101626.0) {
                                if (x[0] <= 109601.0) {
                                    if (x[1] <= 97581.0) {
                                        if (x[1] <= 97308.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[2] <= 275.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[0] <= 111067.0) {
                                        if (x[2] <= 380.0) {
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
                            if (x[1] <= 97218.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[3] <= 278.0) {
                                    if (x[1] <= 100523.0) {
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
                                    if (x[2] <= 201.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[2] <= 251.0) {
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
                            if (x[0] <= 92721.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 100003.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[2] <= 417.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[0] <= 109932.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }
                            }
                        }

                        // tree #12
                        if (x[3] <= 118.0) {
                            if (x[3] <= 110.0) {
                                if (x[0] <= 107011.0) {
                                    if (x[0] <= 97356.5) {
                                        if (x[0] <= 81241.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[3] <= 81.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[1] <= 97951.5) {
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
                                if (x[2] <= 435.5) {
                                    if (x[3] <= 128.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    if (x[1] <= 97495.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[2] <= 453.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[1] <= 92021.0) {
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
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[1] <= 99639.0) {
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

                        // tree #14
                        if (x[3] <= 205.5) {
                            if (x[1] <= 91072.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 101025.5) {
                                    if (x[0] <= 108617.5) {
                                        if (x[0] <= 97629.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[0] <= 109927.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
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
                        }

                        else {
                            if (x[2] <= 640.0) {
                                if (x[3] <= 243.5) {
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
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[2] <= 261.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[0] <= 109240.5) {
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

                        // tree #16
                        if (x[2] <= 598.0) {
                            if (x[0] <= 85538.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 101626.0) {
                                    if (x[2] <= 201.0) {
                                        if (x[3] <= 54.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[3] <= 109.5) {
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
                            votes[0] += 1;
                        }

                        // tree #17
                        if (x[0] <= 92432.5) {
                            if (x[3] <= 90.0) {
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
                            if (x[1] <= 100003.5) {
                                if (x[3] <= 61.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[3] <= 295.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[3] <= 328.5) {
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
                                    if (x[2] <= 276.0) {
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
                            if (x[2] <= 398.5) {
                                if (x[3] <= 57.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[0] <= 92432.5) {
                                        if (x[1] <= 77825.0) {
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
                                if (x[2] <= 406.5) {
                                    votes[0] += 1;
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
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[1] <= 101151.5) {
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
                                votes[0] += 1;
                            }
                        }

                        // tree #19
                        if (x[0] <= 98785.5) {
                            if (x[2] <= 617.5) {
                                if (x[1] <= 91570.5) {
                                    if (x[3] <= 62.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    if (x[1] <= 93549.5) {
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
                            if (x[2] <= 218.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[3] <= 198.0) {
                                    if (x[1] <= 92529.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    if (x[1] <= 100517.5) {
                                        if (x[3] <= 226.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[0] <= 109932.5) {
                                            votes[0] += 1;
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
                                    votes[1] += 1;
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[3] <= 280.0) {
                                if (x[1] <= 99625.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[2] <= 453.0) {
                                        if (x[1] <= 101214.5) {
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
                            }

                            else {
                                if (x[2] <= 643.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
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
                                    if (x[3] <= 132.5) {
                                        votes[1] += 1;
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
                                        if (x[1] <= 101626.0) {
                                            votes[1] += 1;
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
                        }

                        // tree #22
                        if (x[2] <= 644.5) {
                            if (x[3] <= 267.0) {
                                if (x[2] <= 453.0) {
                                    if (x[3] <= 119.5) {
                                        if (x[3] <= 57.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[3] <= 149.0) {
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
                                    if (x[3] <= 257.0) {
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

                        // tree #24
                        if (x[1] <= 93194.0) {
                            if (x[2] <= 398.5) {
                                if (x[3] <= 57.0) {
                                    if (x[2] <= 114.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    if (x[1] <= 77825.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[3] <= 234.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 106239.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 277.0) {
                                    if (x[3] <= 86.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[2] <= 268.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[1] <= 101313.0) {
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
                                        if (x[0] <= 96110.0) {
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
                                    if (x[0] <= 93360.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[1] <= 97000.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[0] <= 100216.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[3] <= 96.0) {
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
                            if (x[1] <= 101626.0) {
                                if (x[2] <= 639.5) {
                                    if (x[0] <= 108911.5) {
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

                        // tree #26
                        if (x[0] <= 92432.5) {
                            if (x[0] <= 85308.5) {
                                votes[1] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[2] <= 205.0) {
                                if (x[2] <= 166.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[1] <= 100664.5) {
                                    if (x[0] <= 98459.0) {
                                        if (x[3] <= 278.0) {
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
                            if (x[1] <= 97414.0) {
                                if (x[2] <= 423.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 653.0) {
                                    if (x[2] <= 473.0) {
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
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[1] <= 99608.5) {
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
                            if (x[0] <= 99300.0) {
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

                        // tree #29
                        if (x[0] <= 98720.5) {
                            if (x[3] <= 267.0) {
                                if (x[2] <= 389.5) {
                                    if (x[0] <= 87058.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[2] <= 135.5) {
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
                                votes[1] += 1;
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
                                    if (x[1] <= 101025.5) {
                                        if (x[3] <= 166.0) {
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

                        // tree #30
                        if (x[0] <= 98603.5) {
                            if (x[0] <= 97484.5) {
                                if (x[0] <= 90896.5) {
                                    if (x[1] <= 77804.0) {
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
                            if (x[2] <= 201.0) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 100664.5) {
                                    if (x[1] <= 91215.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    if (x[0] <= 109932.5) {
                                        votes[0] += 1;
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
                            }
                        }

                        // tree #31
                        if (x[0] <= 91669.5) {
                            if (x[2] <= 212.5) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[3] <= 204.5) {
                                if (x[2] <= 435.5) {
                                    if (x[2] <= 190.5) {
                                        if (x[2] <= 135.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[1] <= 99595.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[0] <= 109405.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[1] <= 97662.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[2] <= 676.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        // tree #32
                        if (x[3] <= 81.5) {
                            if (x[1] <= 77825.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 97356.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 99990.0) {
                                if (x[2] <= 398.5) {
                                    if (x[2] <= 357.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
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
                                if (x[1] <= 101626.0) {
                                    if (x[3] <= 253.0) {
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

                        // tree #33
                        if (x[0] <= 98668.5) {
                            if (x[0] <= 97212.0) {
                                if (x[1] <= 91026.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[2] <= 387.0) {
                                        if (x[2] <= 255.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[2] <= 489.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
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
                                if (x[1] <= 97475.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[1] <= 100659.0) {
                                        votes[0] += 1;
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
                        }

                        // tree #34
                        if (x[1] <= 97030.0) {
                            if (x[2] <= 404.0) {
                                if (x[3] <= 155.0) {
                                    if (x[1] <= 92055.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[1] <= 95039.0) {
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
                                votes[0] += 1;
                            }
                        }

                        else {
                            if (x[1] <= 101626.0) {
                                if (x[3] <= 243.0) {
                                    if (x[0] <= 106500.5) {
                                        if (x[3] <= 148.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[1] <= 99505.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[0] <= 106482.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[1] <= 100243.5) {
                                            votes[0] += 1;
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
                        }

                        // tree #35
                        if (x[2] <= 614.5) {
                            if (x[0] <= 91669.5) {
                                if (x[3] <= 57.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[1] <= 77825.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[1] <= 96834.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[3] <= 194.5) {
                                        if (x[1] <= 97448.5) {
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
                            votes[0] += 1;
                        }

                        // tree #36
                        if (x[2] <= 276.5) {
                            if (x[1] <= 77825.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 93518.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[1] <= 96834.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[3] <= 108.0) {
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
                            if (x[1] <= 100003.5) {
                                if (x[2] <= 366.0) {
                                    if (x[3] <= 131.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    if (x[1] <= 90824.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[1] <= 100977.0) {
                                    if (x[2] <= 444.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    if (x[0] <= 106016.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        // tree #37
                        if (x[0] <= 90896.5) {
                            if (x[2] <= 212.5) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[1] <= 100659.0) {
                                if (x[3] <= 90.5) {
                                    if (x[0] <= 97356.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[3] <= 61.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[1] <= 91101.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[1] <= 100741.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[0] <= 110332.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }
                        }

                        // tree #38
                        if (x[3] <= 179.0) {
                            if (x[1] <= 77825.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[3] <= 149.0) {
                                    if (x[2] <= 276.5) {
                                        if (x[1] <= 99505.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[2] <= 325.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[1] <= 94056.5) {
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
                                if (x[1] <= 97414.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[3] <= 219.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[3] <= 250.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        // tree #39
                        if (x[0] <= 98668.5) {
                            if (x[0] <= 97212.0) {
                                if (x[3] <= 129.5) {
                                    if (x[1] <= 77825.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[1] <= 90227.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[2] <= 405.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[0] <= 98377.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 109663.5) {
                                if (x[3] <= 198.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[2] <= 501.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[2] <= 545.5) {
                                    if (x[1] <= 99079.0) {
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

                        // tree #40
                        if (x[0] <= 92432.5) {
                            if (x[0] <= 85538.0) {
                                votes[1] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[2] <= 270.5) {
                                if (x[0] <= 108371.5) {
                                    if (x[0] <= 97250.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[0] <= 107598.5) {
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
                                if (x[0] <= 109927.0) {
                                    if (x[0] <= 102574.0) {
                                        if (x[3] <= 267.0) {
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
                                    if (x[2] <= 564.0) {
                                        if (x[2] <= 446.0) {
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

                        // tree #41
                        if (x[2] <= 384.0) {
                            if (x[1] <= 91147.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 101471.0) {
                                    if (x[1] <= 99209.5) {
                                        if (x[1] <= 97448.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[0] <= 108923.0) {
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

                        else {
                            if (x[1] <= 100659.0) {
                                if (x[0] <= 98991.5) {
                                    if (x[3] <= 267.0) {
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
                                if (x[1] <= 100945.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        // tree #42
                        if (x[2] <= 565.5) {
                            if (x[0] <= 106742.0) {
                                if (x[0] <= 92432.5) {
                                    if (x[3] <= 57.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[3] <= 108.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[3] <= 69.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[1] <= 99505.0) {
                                    if (x[2] <= 201.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    if (x[1] <= 101785.0) {
                                        if (x[1] <= 100501.5) {
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

                        // tree #43
                        if (x[0] <= 92432.5) {
                            if (x[0] <= 85538.0) {
                                if (x[2] <= 117.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 88611.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[3] <= 90.5) {
                                if (x[1] <= 97546.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 617.5) {
                                    if (x[2] <= 444.0) {
                                        if (x[1] <= 99855.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[0] <= 108733.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[1] <= 92793.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        // tree #44
                        if (x[0] <= 98785.5) {
                            if (x[1] <= 77825.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 384.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[1] <= 96804.0) {
                                        if (x[1] <= 92995.5) {
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
                            if (x[1] <= 99990.0) {
                                if (x[3] <= 84.5) {
                                    votes[1] += 1;
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
                                    if (x[0] <= 110785.5) {
                                        if (x[3] <= 146.0) {
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

                        // tree #45
                        if (x[2] <= 640.0) {
                            if (x[1] <= 91072.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 93482.0) {
                                    if (x[1] <= 91873.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    if (x[2] <= 446.0) {
                                        if (x[2] <= 329.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
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
                        }

                        else {
                            votes[0] += 1;
                        }

                        // tree #46
                        if (x[2] <= 640.0) {
                            if (x[0] <= 93037.0) {
                                if (x[2] <= 260.0) {
                                    if (x[2] <= 117.0) {
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
                                if (x[3] <= 267.0) {
                                    if (x[0] <= 106742.0) {
                                        if (x[3] <= 75.5) {
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
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            votes[0] += 1;
                        }

                        // tree #47
                        if (x[2] <= 368.5) {
                            if (x[1] <= 93613.0) {
                                if (x[1] <= 77804.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[1] <= 91808.0) {
                                        if (x[1] <= 90227.5) {
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
                                if (x[1] <= 96917.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[2] <= 276.5) {
                                        if (x[1] <= 99209.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[3] <= 135.0) {
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
                            if (x[1] <= 96804.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 97900.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[1] <= 100659.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }
                        }

                        // tree #48
                        if (x[2] <= 398.5) {
                            if (x[3] <= 149.0) {
                                if (x[0] <= 96804.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[3] <= 77.0) {
                                        if (x[3] <= 61.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[1] <= 99625.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[1] <= 92053.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[3] <= 230.5) {
                                if (x[3] <= 154.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 582.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        // tree #49
                        if (x[0] <= 98668.5) {
                            if (x[1] <= 91026.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 399.0) {
                                    if (x[2] <= 255.5) {
                                        if (x[0] <= 96804.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[2] <= 362.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[3] <= 267.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[1] <= 99625.5) {
                                if (x[0] <= 100034.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 108032.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[2] <= 391.0) {
                                        votes[1] += 1;
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
                        }

                        // tree #50
                        if (x[0] <= 102574.0) {
                            if (x[3] <= 216.0) {
                                if (x[3] <= 155.0) {
                                    if (x[1] <= 77825.0) {
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
                                    if (x[3] <= 180.0) {
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
                            if (x[3] <= 204.0) {
                                if (x[3] <= 186.5) {
                                    if (x[2] <= 201.0) {
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
                                if (x[2] <= 546.5) {
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