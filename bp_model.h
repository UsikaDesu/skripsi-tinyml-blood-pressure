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
                        if (x[0] <= 115748.5) {
                            votes[0] += 1;
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #2
                        votes[0] += 1;
                        // tree #3
                        if (x[1] <= 101486.0) {
                            if (x[1] <= 95120.0) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[1] <= 97587.5) {
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

                        // tree #4
                        if (x[2] <= 871.5) {
                            votes[0] += 1;
                        }

                        else {
                            if (x[1] <= 97587.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[0] <= 111676.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #5
                        if (x[2] <= 679.0) {
                            votes[0] += 1;
                        }

                        else {
                            if (x[1] <= 101486.0) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #6
                        votes[0] += 1;
                        // tree #7
                        if (x[1] <= 96703.5) {
                            votes[0] += 1;
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #8
                        if (x[0] <= 113900.0) {
                            votes[0] += 1;
                        }

                        else {
                            if (x[1] <= 100602.0) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #9
                        if (x[2] <= 871.5) {
                            votes[0] += 1;
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #10
                        if (x[0] <= 111676.5) {
                            votes[0] += 1;
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #11
                        if (x[0] <= 113900.0) {
                            votes[0] += 1;
                        }

                        else {
                            if (x[2] <= 679.0) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #12
                        if (x[2] <= 871.5) {
                            votes[0] += 1;
                        }

                        else {
                            if (x[1] <= 97587.5) {
                                votes[1] += 1;
                            }

                            else {
                                if (x[0] <= 111676.5) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #13
                        if (x[2] <= 916.0) {
                            votes[0] += 1;
                        }

                        else {
                            if (x[0] <= 112026.0) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #14
                        if (x[2] <= 1832.0) {
                            votes[1] += 1;
                        }

                        else {
                            votes[0] += 1;
                        }

                        // tree #15
                        if (x[1] <= 99018.5) {
                            votes[0] += 1;
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #16
                        if (x[1] <= 101486.0) {
                            if (x[2] <= 723.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[0] <= 112026.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #17
                        if (x[0] <= 113900.0) {
                            votes[0] += 1;
                        }

                        else {
                            if (x[2] <= 679.0) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #18
                        if (x[0] <= 113900.0) {
                            votes[0] += 1;
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #19
                        if (x[1] <= 95120.0) {
                            votes[0] += 1;
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #20
                        if (x[2] <= 679.0) {
                            votes[0] += 1;
                        }

                        else {
                            if (x[2] <= 1832.0) {
                                votes[1] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #21
                        if (x[1] <= 96703.5) {
                            votes[0] += 1;
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #22
                        if (x[0] <= 113900.0) {
                            votes[0] += 1;
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #23
                        if (x[1] <= 101486.0) {
                            votes[0] += 1;
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #24
                        if (x[1] <= 101486.0) {
                            if (x[2] <= 1832.0) {
                                votes[1] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #25
                        if (x[1] <= 96703.5) {
                            votes[0] += 1;
                        }

                        else {
                            if (x[2] <= 1832.0) {
                                votes[1] += 1;
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        // tree #26
                        if (x[0] <= 115748.5) {
                            votes[0] += 1;
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #27
                        if (x[2] <= 871.5) {
                            votes[0] += 1;
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #28
                        if (x[1] <= 96703.5) {
                            votes[0] += 1;
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #29
                        if (x[0] <= 115748.5) {
                            if (x[2] <= 871.5) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #30
                        if (x[1] <= 100602.0) {
                            votes[0] += 1;
                        }

                        else {
                            votes[1] += 1;
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
                            return "Hypotension";
                            case 1:
                            return "Normal";
                            case 2:
                            return "Hypertension";
                            default:
                            return "Houston we have a problem";
                        }
                    }

                protected:
                };
            }
        }
    }