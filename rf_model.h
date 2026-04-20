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
                        votes[0] += 1;
                        // tree #2
                        if (x[2] <= 1787.5) {
                            votes[1] += 1;
                        }

                        else {
                            votes[0] += 1;
                        }

                        // tree #3
                        votes[1] += 1;
                        // tree #4
                        if (x[2] <= 1787.5) {
                            votes[1] += 1;
                        }

                        else {
                            votes[0] += 1;
                        }

                        // tree #5
                        if (x[1] <= 101486.0) {
                            votes[0] += 1;
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #6
                        if (x[1] <= 101486.0) {
                            votes[0] += 1;
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #7
                        votes[1] += 1;
                        // tree #8
                        if (x[0] <= 111676.5) {
                            votes[0] += 1;
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #9
                        votes[1] += 1;
                        // tree #10
                        if (x[3] <= 349.0) {
                            votes[1] += 1;
                        }

                        else {
                            votes[0] += 1;
                        }

                        // tree #11
                        if (x[0] <= 111676.5) {
                            votes[0] += 1;
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #12
                        if (x[2] <= 1787.5) {
                            votes[1] += 1;
                        }

                        else {
                            votes[0] += 1;
                        }

                        // tree #13
                        votes[0] += 1;
                        // tree #14
                        if (x[2] <= 1787.5) {
                            votes[1] += 1;
                        }

                        else {
                            votes[0] += 1;
                        }

                        // tree #15
                        if (x[1] <= 101486.0) {
                            votes[0] += 1;
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #16
                        votes[1] += 1;
                        // tree #17
                        votes[0] += 1;
                        // tree #18
                        votes[1] += 1;
                        // tree #19
                        if (x[4] <= 89.5) {
                            votes[0] += 1;
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #20
                        if (x[2] <= 1787.5) {
                            votes[1] += 1;
                        }

                        else {
                            votes[0] += 1;
                        }

                        // tree #21
                        votes[0] += 1;
                        // tree #22
                        votes[1] += 1;
                        // tree #23
                        votes[0] += 1;
                        // tree #24
                        votes[1] += 1;
                        // tree #25
                        votes[0] += 1;
                        // tree #26
                        if (x[0] <= 111676.5) {
                            votes[0] += 1;
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #27
                        votes[1] += 1;
                        // tree #28
                        if (x[4] <= 89.5) {
                            votes[0] += 1;
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #29
                        if (x[3] <= 349.0) {
                            votes[1] += 1;
                        }

                        else {
                            votes[0] += 1;
                        }

                        // tree #30
                        votes[0] += 1;
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