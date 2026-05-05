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
                        if (x[1] <= 93239.5) {
                            if (x[3] <= 111.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 398.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 111532.5) {
                                if (x[2] <= 195.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 391.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        // tree #2
                        if (x[2] <= 399.0) {
                            if (x[2] <= 255.5) {
                                if (x[0] <= 81078.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 354.0) {
                                    if (x[2] <= 298.5) {
                                        if (x[2] <= 269.5) {
                                            if (x[1] <= 93464.5) {
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
                                        if (x[3] <= 118.5) {
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
                        }

                        else {
                            if (x[1] <= 101247.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 101373.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        // tree #3
                        if (x[0] <= 91669.5) {
                            if (x[3] <= 111.5) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[1] <= 100773.0) {
                                if (x[2] <= 178.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 109770.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #4
                        if (x[1] <= 93549.5) {
                            if (x[0] <= 85178.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 250.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[2] <= 538.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[1] <= 101303.5) {
                                if (x[1] <= 100203.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[3] <= 124.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[2] <= 470.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #5
                        if (x[1] <= 92946.5) {
                            if (x[2] <= 314.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 384.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[1] <= 91166.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[0] <= 109427.5) {
                                if (x[0] <= 97378.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[1] <= 99089.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #6
                        if (x[2] <= 399.0) {
                            if (x[2] <= 269.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 90797.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[3] <= 109.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[3] <= 118.5) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            if (x[3] <= 129.0) {
                                                votes[0] += 1;
                                            }

                                            else {
                                                if (x[1] <= 101033.5) {
                                                    if (x[0] <= 99031.0) {
                                                        votes[1] += 1;
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
                            if (x[1] <= 101170.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 470.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #7
                        if (x[0] <= 92432.5) {
                            if (x[0] <= 86558.0) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[1] <= 100696.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 470.0) {
                                    if (x[1] <= 100900.5) {
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

                        // tree #8
                        if (x[0] <= 109669.0) {
                            if (x[1] <= 93117.0) {
                                if (x[0] <= 91943.0) {
                                    if (x[1] <= 81571.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    if (x[2] <= 287.5) {
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

                        else {
                            if (x[3] <= 99.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 99301.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #9
                        if (x[0] <= 91669.5) {
                            if (x[1] <= 90940.0) {
                                if (x[3] <= 61.0) {
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
                            if (x[1] <= 100632.0) {
                                votes[0] += 1;
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

                        // tree #10
                        if (x[3] <= 232.0) {
                            if (x[3] <= 110.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 93233.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[0] <= 109663.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[1] <= 99301.0) {
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

                        // tree #11
                        if (x[0] <= 92706.0) {
                            if (x[2] <= 231.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[3] <= 133.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 100203.0) {
                                if (x[2] <= 178.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 109669.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[2] <= 378.5) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }
                        }

                        // tree #12
                        if (x[1] <= 91185.5) {
                            if (x[3] <= 90.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 91785.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 109932.5) {
                                if (x[0] <= 91943.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[3] <= 86.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[1] <= 100223.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #13
                        if (x[2] <= 398.5) {
                            if (x[2] <= 255.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 97754.0) {
                                    if (x[3] <= 111.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    if (x[1] <= 99625.5) {
                                        if (x[0] <= 107147.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        if (x[2] <= 342.5) {
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
                            if (x[1] <= 91354.5) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #14
                        if (x[0] <= 92706.0) {
                            if (x[0] <= 86573.0) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[1] <= 100203.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 109669.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #15
                        if (x[2] <= 398.5) {
                            if (x[1] <= 90797.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[3] <= 110.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[1] <= 101033.5) {
                                        if (x[3] <= 155.0) {
                                            if (x[2] <= 332.5) {
                                                if (x[1] <= 99255.5) {
                                                    if (x[2] <= 283.5) {
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
                            if (x[3] <= 204.0) {
                                if (x[3] <= 196.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 483.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        // tree #16
                        if (x[1] <= 93289.0) {
                            if (x[3] <= 111.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 94268.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[2] <= 396.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[3] <= 202.5) {
                                if (x[0] <= 109716.5) {
                                    if (x[2] <= 234.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    if (x[0] <= 110372.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[1] <= 99565.5) {
                                            votes[0] += 1;
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

                        // tree #17
                        if (x[0] <= 92706.0) {
                            if (x[3] <= 114.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 88219.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 109932.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 100029.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #18
                        if (x[2] <= 255.5) {
                            votes[0] += 1;
                        }

                        else {
                            if (x[2] <= 398.5) {
                                if (x[3] <= 146.0) {
                                    if (x[0] <= 109663.5) {
                                        if (x[2] <= 266.0) {
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
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 565.5) {
                                    if (x[2] <= 563.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    if (x[2] <= 575.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        // tree #19
                        if (x[0] <= 109927.0) {
                            if (x[0] <= 91669.5) {
                                if (x[1] <= 82548.0) {
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
                            if (x[1] <= 100223.0) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #20
                        if (x[2] <= 576.0) {
                            if (x[3] <= 110.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 92044.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[3] <= 113.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[1] <= 99744.0) {
                                            if (x[2] <= 296.0) {
                                                votes[0] += 1;
                                            }

                                            else {
                                                votes[0] += 1;
                                            }
                                        }

                                        else {
                                            if (x[0] <= 109405.0) {
                                                votes[0] += 1;
                                            }

                                            else {
                                                if (x[2] <= 405.0) {
                                                    votes[1] += 1;
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
                            if (x[2] <= 597.5) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #21
                        if (x[3] <= 204.0) {
                            if (x[2] <= 255.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 93582.0) {
                                    if (x[2] <= 384.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[1] <= 91354.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[1] <= 99858.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[0] <= 108022.0) {
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
                            if (x[1] <= 91734.0) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #22
                        if (x[3] <= 232.0) {
                            if (x[3] <= 108.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[3] <= 118.0) {
                                    if (x[2] <= 266.0) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[2] <= 270.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[2] <= 360.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[0] <= 93233.0) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            if (x[1] <= 99744.0) {
                                                votes[0] += 1;
                                            }

                                            else {
                                                if (x[1] <= 101785.0) {
                                                    if (x[1] <= 101159.5) {
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
                            if (x[3] <= 233.5) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #23
                        if (x[2] <= 399.0) {
                            if (x[2] <= 255.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 95554.0) {
                                    if (x[1] <= 81571.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    if (x[0] <= 109601.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        if (x[3] <= 148.5) {
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
                            votes[0] += 1;
                        }

                        // tree #24
                        if (x[1] <= 93289.0) {
                            if (x[2] <= 237.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 398.5) {
                                    if (x[3] <= 111.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    if (x[1] <= 91166.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[0] <= 109669.0) {
                                if (x[0] <= 93527.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 357.5) {
                                    if (x[2] <= 237.5) {
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

                        // tree #25
                        if (x[1] <= 91231.0) {
                            if (x[3] <= 111.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 97992.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 109927.0) {
                                if (x[0] <= 93037.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    if (x[0] <= 94200.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[1] <= 100223.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #26
                        if (x[0] <= 92706.0) {
                            if (x[0] <= 86573.0) {
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
                            if (x[1] <= 100623.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 109932.5) {
                                    if (x[0] <= 106428.5) {
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

                        // tree #27
                        if (x[1] <= 93289.0) {
                            if (x[2] <= 250.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 92706.0) {
                                    if (x[3] <= 111.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    if (x[0] <= 93029.5) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[1] <= 101170.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 471.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #28
                        if (x[2] <= 384.0) {
                            if (x[1] <= 99595.0) {
                                if (x[3] <= 155.0) {
                                    if (x[0] <= 81345.5) {
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
                                if (x[2] <= 342.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 101170.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 109430.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #29
                        if (x[0] <= 91669.5) {
                            if (x[3] <= 111.0) {
                                votes[0] += 1;
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
                                if (x[0] <= 112332.5) {
                                    if (x[2] <= 463.0) {
                                        if (x[0] <= 111010.5) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }

                                    else {
                                        if (x[1] <= 101022.5) {
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

                        // tree #30
                        if (x[0] <= 91669.5) {
                            if (x[0] <= 85798.5) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[0] <= 109932.5) {
                                if (x[1] <= 90743.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 603.0) {
                                    if (x[1] <= 99534.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    if (x[1] <= 96631.0) {
                                        votes[0] += 1;
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

                protected:
                };
            }
        }
    }