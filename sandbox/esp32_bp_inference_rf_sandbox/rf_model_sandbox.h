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
                        if (x[3] <= 83.5) {
                            if (x[0] <= 0.46024999022483826) {
                                if (x[2] <= 0.2542000040411949) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 81.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 0.27605000138282776) {
                                votes[1] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #2
                        if (x[2] <= 0.25349999964237213) {
                            if (x[3] <= 65.0) {
                                votes[1] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        else {
                            if (x[0] <= 0.6556499898433685) {
                                if (x[3] <= 61.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.2899499982595444) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #3
                        if (x[2] <= 0.2672000080347061) {
                            if (x[1] <= 0.06299999915063381) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[1] <= 0.11000000312924385) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 0.29225000739097595) {
                                if (x[2] <= 0.2833999991416931) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[1] <= 0.0560000017285347) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #4
                        if (x[2] <= 0.2776000052690506) {
                            if (x[0] <= 0.5936500132083893) {
                                if (x[2] <= 0.2526500001549721) {
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
                            if (x[3] <= 74.0) {
                                if (x[0] <= 0.28824999183416367) {
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

                        // tree #5
                        if (x[3] <= 83.5) {
                            if (x[2] <= 0.25495000183582306) {
                                if (x[2] <= 0.21370000392198563) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[1] <= 0.26429998874664307) {
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
                        if (x[2] <= 0.2639999985694885) {
                            if (x[0] <= 0.25804999470710754) {
                                if (x[0] <= 0.23560000211000443) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.5257000029087067) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[3] <= 84.5) {
                                if (x[3] <= 65.5) {
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

                        // tree #7
                        if (x[1] <= 0.22955000400543213) {
                            if (x[2] <= 0.2875500023365021) {
                                if (x[3] <= 63.5) {
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
                            if (x[3] <= 80.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 0.28005000203847885) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #8
                        if (x[0] <= 0.7502500116825104) {
                            if (x[3] <= 81.5) {
                                if (x[0] <= 0.19139999896287918) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[1] <= 0.12370000034570694) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 0.24719999730587006) {
                                votes[0] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #9
                        if (x[0] <= 0.46194998919963837) {
                            if (x[2] <= 0.251350000500679) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[3] <= 63.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 0.2672000080347061) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[3] <= 80.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #10
                        if (x[3] <= 83.5) {
                            if (x[1] <= 0.26134999096393585) {
                                if (x[2] <= 0.2524999976158142) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.2415499985218048) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 0.1359499990940094) {
                                if (x[1] <= 0.12075000256299973) {
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

                        // tree #11
                        if (x[2] <= 0.26635000109672546) {
                            if (x[3] <= 70.0) {
                                if (x[1] <= 0.10690000280737877) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[1] <= 0.09360000118613243) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 0.2667999863624573) {
                                if (x[0] <= 0.2932000011205673) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.27854999899864197) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #12
                        if (x[3] <= 83.5) {
                            if (x[3] <= 72.0) {
                                if (x[3] <= 63.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.650299996137619) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.29294998943805695) {
                                if (x[3] <= 88.0) {
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

                        // tree #13
                        if (x[2] <= 0.2778500020503998) {
                            if (x[2] <= 0.25495000183582306) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 0.43479999899864197) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 0.2875500023365021) {
                                if (x[2] <= 0.287200003862381) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.11934999749064445) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #14
                        if (x[3] <= 83.5) {
                            if (x[1] <= 0.08425000309944153) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[2] <= 0.2778500020503998) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.2932000011205673) {
                                if (x[0] <= 0.2587999925017357) {
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

                        // tree #15
                        if (x[2] <= 0.25349999964237213) {
                            if (x[0] <= 0.43745000660419464) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 0.214199997484684) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.20429999381303787) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[2] <= 0.2875500023365021) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #16
                        if (x[2] <= 0.2875500023365021) {
                            if (x[3] <= 79.5) {
                                if (x[0] <= 0.37539999186992645) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.24695000797510147) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[3] <= 76.5) {
                                votes[1] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #17
                        if (x[0] <= 0.5485499799251556) {
                            if (x[0] <= 0.28965000808238983) {
                                if (x[0] <= 0.1648000031709671) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[1] <= 0.124549999833107) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[3] <= 89.0) {
                                if (x[2] <= 0.275299996137619) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[1] <= 0.24194999039173126) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #18
                        if (x[2] <= 0.26590000092983246) {
                            if (x[3] <= 65.5) {
                                if (x[3] <= 59.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[0] <= 0.14070000126957893) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[2] <= 0.2892000079154968) {
                                if (x[3] <= 87.0) {
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

                        // tree #19
                        if (x[2] <= 0.28485000133514404) {
                            if (x[0] <= 0.5145000219345093) {
                                if (x[1] <= 0.10965000092983246) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 81.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.11934999749064445) {
                                votes[1] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #20
                        if (x[2] <= 0.26680000126361847) {
                            if (x[3] <= 72.5) {
                                if (x[1] <= 0.10935000330209732) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[2] <= 0.2589000016450882) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[3] <= 64.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 0.28485000133514404) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #21
                        if (x[3] <= 83.5) {
                            if (x[2] <= 0.26635000109672546) {
                                if (x[1] <= 0.06314999982714653) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 65.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 0.12950000166893005) {
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

                        // tree #22
                        if (x[2] <= 0.26635000109672546) {
                            if (x[1] <= 0.06314999982714653) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[2] <= 0.26215000450611115) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[3] <= 61.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[2] <= 0.2778500020503998) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #23
                        if (x[2] <= 0.26635000109672546) {
                            if (x[1] <= 0.06590000167489052) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[0] <= 0.5297999978065491) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 0.12370000034570694) {
                                if (x[3] <= 77.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 76.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #24
                        if (x[3] <= 82.5) {
                            if (x[2] <= 0.2635499984025955) {
                                if (x[2] <= 0.219650000333786) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 71.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            if (x[1] <= 0.1246500015258789) {
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

                        // tree #25
                        if (x[3] <= 83.5) {
                            if (x[1] <= 0.1928500011563301) {
                                if (x[2] <= 0.250450000166893) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[3] <= 67.5) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[0] <= 0.2961000055074692) {
                                votes[0] += 1;
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