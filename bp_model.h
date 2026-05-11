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
    uint8_t votes[2] = {0};
    // tree #1
    if (x[0] <= 98668.5) {
      if (x[1] <= 96804.0) {
        if (x[2] <= 114.0) {
          if (x[2] <= 76.0) {
            votes[0] += 1;
          }

          else {
            votes[1] += 1;
          }
        }

        else {
          if (x[2] <= 384.0) {
            if (x[0] <= 86950.0) {
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
        votes[1] += 1;
      }
    }

    else {
      if (x[0] <= 109927.0) {
        if (x[0] <= 102153.5) {
          if (x[2] <= 377.5) {
            votes[0] += 1;
          }

          else {
            if (x[1] <= 97934.0) {
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
        if (x[1] <= 100223.0) {
          votes[0] += 1;
        }

        else {
          votes[1] += 1;
        }
      }
    }

    // tree #2
    if (x[2] <= 453.0) {
      if (x[2] <= 446.0) {
        if (x[2] <= 368.5) {
          if (x[2] <= 355.0) {
            if (x[1] <= 90970.0) {
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
          if (x[0] <= 110708.5) {
            if (x[2] <= 414.5) {
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

    else {
      if (x[2] <= 623.5) {
        votes[0] += 1;
      }

      else {
        if (x[2] <= 640.0) {
          votes[1] += 1;
        }

        else {
          votes[0] += 1;
        }
      }
    }

    // tree #3
    if (x[1] <= 99145.0) {
      if (x[1] <= 89109.0) {
        if (x[1] <= 78026.0) {
          votes[0] += 1;
        }

        else {
          votes[1] += 1;
        }
      }

      else {
        if (x[0] <= 91669.5) {
          votes[1] += 1;
        }

        else {
          if (x[2] <= 114.0) {
            votes[1] += 1;
          }

          else {
            if (x[2] <= 201.0) {
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
      if (x[2] <= 642.0) {
        if (x[1] <= 101002.0) {
          if (x[0] <= 108738.5) {
            if (x[1] <= 100411.0) {
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
          if (x[2] <= 462.0) {
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

    // tree #4
    if (x[2] <= 640.0) {
      if (x[1] <= 99114.5) {
        if (x[0] <= 99544.0) {
          if (x[0] <= 97933.0) {
            if (x[1] <= 80437.5) {
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
          if (x[0] <= 107011.0) {
            if (x[1] <= 97030.0) {
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
        if (x[2] <= 486.0) {
          if (x[0] <= 102971.5) {
            votes[1] += 1;
          }

          else {
            if (x[1] <= 100972.5) {
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
      votes[0] += 1;
    }

    // tree #5
    if (x[2] <= 640.0) {
      if (x[1] <= 99625.5) {
        if (x[0] <= 91669.5) {
          if (x[0] <= 82669.5) {
            votes[0] += 1;
          }

          else {
            votes[1] += 1;
          }
        }

        else {
          if (x[0] <= 97933.0) {
            votes[0] += 1;
          }

          else {
            if (x[0] <= 98600.5) {
              votes[1] += 1;
            }

            else {
              votes[0] += 1;
            }
          }
        }
      }

      else {
        if (x[2] <= 409.0) {
          if (x[2] <= 276.5) {
            if (x[2] <= 243.5) {
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
          if (x[0] <= 109240.5) {
            if (x[2] <= 437.5) {
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

    // tree #6
    if (x[1] <= 89500.5) {
      if (x[1] <= 78026.0) {
        votes[0] += 1;
      }

      else {
        votes[1] += 1;
      }
    }

    else {
      if (x[1] <= 97090.5) {
        if (x[1] <= 93179.5) {
          if (x[1] <= 92325.5) {
            if (x[2] <= 301.0) {
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
        if (x[1] <= 97463.0) {
          votes[1] += 1;
        }

        else {
          if (x[0] <= 102789.5) {
            if (x[2] <= 355.0) {
              votes[0] += 1;
            }

            else {
              votes[1] += 1;
            }
          }

          else {
            if (x[1] <= 100664.5) {
              votes[0] += 1;
            }

            else {
              votes[0] += 1;
            }
          }
        }
      }
    }

    // tree #7
    if (x[1] <= 99114.5) {
      if (x[2] <= 119.5) {
        votes[1] += 1;
      }

      else {
        if (x[2] <= 368.5) {
          if (x[1] <= 97463.0) {
            if (x[2] <= 349.0) {
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
          if (x[1] <= 97414.0) {
            votes[0] += 1;
          }

          else {
            if (x[1] <= 97721.5) {
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
      if (x[2] <= 600.0) {
        if (x[0] <= 109430.5) {
          if (x[0] <= 102378.0) {
            if (x[0] <= 101427.0) {
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
        votes[0] += 1;
      }
    }

    // tree #8
    if (x[0] <= 92706.0) {
      if (x[1] <= 90970.0) {
        if (x[2] <= 279.5) {
          if (x[1] <= 81057.0) {
            votes[0] += 1;
          }

          else {
            if (x[0] <= 87647.5) {
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
      }

      else {
        votes[1] += 1;
      }
    }

    else {
      if (x[2] <= 190.5) {
        if (x[2] <= 166.5) {
          if (x[1] <= 95377.0) {
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
        if (x[2] <= 565.5) {
          if (x[1] <= 99384.5) {
            votes[0] += 1;
          }

          else {
            if (x[0] <= 109297.5) {
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

    // tree #9
    if (x[2] <= 114.0) {
      if (x[1] <= 76364.5) {
        votes[0] += 1;
      }

      else {
        votes[1] += 1;
      }
    }

    else {
      if (x[0] <= 92706.0) {
        if (x[0] <= 86950.0) {
          if (x[1] <= 80215.5) {
            votes[0] += 1;
          }

          else {
            if (x[1] <= 88628.5) {
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
        if (x[1] <= 99749.0) {
          if (x[0] <= 98723.0) {
            if (x[1] <= 96804.0) {
              votes[0] += 1;
            }

            else {
              votes[1] += 1;
            }
          }

          else {
            if (x[0] <= 106742.0) {
              votes[0] += 1;
            }

            else {
              votes[0] += 1;
            }
          }
        }

        else {
          if (x[0] <= 109669.0) {
            if (x[0] <= 102529.0) {
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

    // tree #10
    if (x[0] <= 92432.5) {
      if (x[1] <= 77825.0) {
        votes[0] += 1;
      }

      else {
        if (x[0] <= 86950.0) {
          if (x[0] <= 83598.0) {
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
      if (x[2] <= 205.0) {
        if (x[0] <= 108921.5) {
          if (x[0] <= 97250.5) {
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
        if (x[0] <= 109927.0) {
          if (x[0] <= 102533.5) {
            if (x[2] <= 536.0) {
              votes[0] += 1;
            }

            else {
              votes[0] += 1;
            }
          }

          else {
            if (x[2] <= 261.5) {
              votes[0] += 1;
            }

            else {
              votes[0] += 1;
            }
          }
        }

        else {
          if (x[2] <= 464.5) {
            votes[0] += 1;
          }

          else {
            if (x[0] <= 112027.5) {
              votes[1] += 1;
            }

            else {
              votes[0] += 1;
            }
          }
        }
      }
    }

    // tree #11
    if (x[0] <= 102153.5) {
      if (x[2] <= 114.0) {
        if (x[0] <= 81028.5) {
          votes[0] += 1;
        }

        else {
          votes[1] += 1;
        }
      }

      else {
        if (x[2] <= 139.5) {
          votes[0] += 1;
        }

        else {
          if (x[2] <= 155.5) {
            votes[1] += 1;
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
      }
    }

    else {
      if (x[1] <= 99749.0) {
        votes[0] += 1;
      }

      else {
        if (x[2] <= 267.5) {
          if (x[0] <= 108212.5) {
            votes[1] += 1;
          }

          else {
            votes[0] += 1;
          }
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
    }

    // tree #12
    if (x[2] <= 359.5) {
      if (x[1] <= 99145.0) {
        if (x[0] <= 86175.5) {
          votes[0] += 1;
        }

        else {
          if (x[1] <= 93518.0) {
            if (x[2] <= 114.0) {
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

      else {
        if (x[2] <= 200.0) {
          votes[1] += 1;
        }

        else {
          votes[0] += 1;
        }
      }
    }

    else {
      if (x[2] <= 368.5) {
        votes[1] += 1;
      }

      else {
        if (x[2] <= 598.0) {
          if (x[0] <= 97957.5) {
            votes[0] += 1;
          }

          else {
            if (x[2] <= 471.5) {
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
    }

    // tree #13
    if (x[2] <= 276.5) {
      if (x[0] <= 85930.0) {
        if (x[0] <= 81241.5) {
          votes[1] += 1;
        }

        else {
          votes[0] += 1;
        }
      }

      else {
        if (x[1] <= 93518.0) {
          if (x[1] <= 91424.5) {
            votes[1] += 1;
          }

          else {
            if (x[1] <= 92294.5) {
              votes[0] += 1;
            }

            else {
              votes[1] += 1;
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
            votes[1] += 1;
          }
        }
      }
    }

    else {
      if (x[0] <= 111617.5) {
        if (x[2] <= 473.0) {
          votes[0] += 1;
        }

        else {
          if (x[0] <= 102595.5) {
            if (x[2] <= 555.5) {
              votes[1] += 1;
            }

            else {
              votes[0] += 1;
            }
          }

          else {
            if (x[1] <= 101043.0) {
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
          votes[0] += 1;
        }

        else {
          votes[1] += 1;
        }
      }
    }

    // tree #14
    if (x[2] <= 266.0) {
      if (x[0] <= 84648.0) {
        if (x[1] <= 77825.0) {
          votes[0] += 1;
        }

        else {
          votes[1] += 1;
        }
      }

      else {
        if (x[0] <= 109509.0) {
          if (x[1] <= 93518.0) {
            votes[1] += 1;
          }

          else {
            if (x[2] <= 223.5) {
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
      if (x[0] <= 92721.5) {
        votes[1] += 1;
      }

      else {
        if (x[1] <= 99749.0) {
          if (x[2] <= 623.5) {
            votes[0] += 1;
          }

          else {
            if (x[0] <= 101112.0) {
              votes[1] += 1;
            }

            else {
              votes[0] += 1;
            }
          }
        }

        else {
          if (x[0] <= 110269.5) {
            votes[0] += 1;
          }

          else {
            votes[1] += 1;
          }
        }
      }
    }

    // tree #15
    if (x[1] <= 89422.0) {
      if (x[1] <= 81240.0) {
        votes[0] += 1;
      }

      else {
        votes[1] += 1;
      }
    }

    else {
      if (x[0] <= 106742.0) {
        if (x[1] <= 93281.0) {
          if (x[1] <= 92325.5) {
            if (x[2] <= 301.0) {
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
          if (x[2] <= 203.0) {
            votes[1] += 1;
          }

          else {
            if (x[1] <= 99607.0) {
              votes[0] += 1;
            }

            else {
              votes[0] += 1;
            }
          }
        }
      }

      else {
        if (x[1] <= 97000.5) {
          votes[0] += 1;
        }

        else {
          if (x[0] <= 107011.0) {
            votes[1] += 1;
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
      }
    }

    // tree #16
    if (x[1] <= 93179.5) {
      if (x[2] <= 384.0) {
        if (x[0] <= 87197.5) {
          if (x[2] <= 279.5) {
            if (x[0] <= 81296.0) {
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
          if (x[2] <= 350.5) {
            if (x[2] <= 197.5) {
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

    else {
      if (x[0] <= 109952.5) {
        if (x[1] <= 97090.5) {
          votes[0] += 1;
        }

        else {
          if (x[1] <= 97569.0) {
            votes[1] += 1;
          }

          else {
            if (x[0] <= 102170.5) {
              votes[1] += 1;
            }

            else {
              votes[0] += 1;
            }
          }
        }
      }

      else {
        if (x[1] <= 99749.0) {
          votes[0] += 1;
        }

        else {
          votes[1] += 1;
        }
      }
    }

    // tree #17
    if (x[0] <= 98668.5) {
      if (x[2] <= 587.5) {
        if (x[2] <= 384.5) {
          if (x[2] <= 314.5) {
            if (x[1] <= 81240.0) {
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
        if (x[2] <= 643.0) {
          votes[1] += 1;
        }

        else {
          votes[0] += 1;
        }
      }
    }

    else {
      if (x[1] <= 100203.0) {
        votes[0] += 1;
      }

      else {
        if (x[1] <= 100399.0) {
          votes[1] += 1;
        }

        else {
          if (x[0] <= 109405.0) {
            votes[0] += 1;
          }

          else {
            votes[1] += 1;
          }
        }
      }
    }

    // tree #18
    if (x[0] <= 91943.0) {
      if (x[1] <= 77825.0) {
        votes[0] += 1;
      }

      else {
        votes[1] += 1;
      }
    }

    else {
      if (x[1] <= 97090.5) {
        votes[0] += 1;
      }

      else {
        if (x[0] <= 102153.5) {
          votes[1] += 1;
        }

        else {
          if (x[2] <= 216.0) {
            if (x[2] <= 166.5) {
              votes[0] += 1;
            }

            else {
              votes[1] += 1;
            }
          }

          else {
            if (x[0] <= 110708.5) {
              votes[0] += 1;
            }

            else {
              votes[1] += 1;
            }
          }
        }
      }
    }

    // tree #19
    if (x[1] <= 97030.0) {
      if (x[0] <= 91258.0) {
        if (x[0] <= 85930.0) {
          votes[0] += 1;
        }

        else {
          votes[1] += 1;
        }
      }

      else {
        if (x[0] <= 98668.5) {
          if (x[2] <= 114.0) {
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

    else {
      if (x[2] <= 218.5) {
        votes[1] += 1;
      }

      else {
        if (x[2] <= 503.5) {
          if (x[1] <= 99354.0) {
            votes[0] += 1;
          }

          else {
            if (x[2] <= 401.5) {
              votes[1] += 1;
            }

            else {
              votes[0] += 1;
            }
          }
        }

        else {
          if (x[2] <= 662.0) {
            if (x[2] <= 586.5) {
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

    // tree #20
    if (x[2] <= 123.5) {
      votes[1] += 1;
    }

    else {
      if (x[2] <= 640.0) {
        if (x[1] <= 99145.0) {
          if (x[0] <= 92432.5) {
            if (x[1] <= 90970.0) {
              votes[0] += 1;
            }

            else {
              votes[1] += 1;
            }
          }

          else {
            if (x[2] <= 623.5) {
              votes[0] += 1;
            }

            else {
              votes[1] += 1;
            }
          }
        }

        else {
          if (x[1] <= 101373.0) {
            if (x[2] <= 590.0) {
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

      else {
        votes[0] += 1;
      }
    }

    // tree #21
    if (x[1] <= 89422.0) {
      if (x[2] <= 314.5) {
        if (x[2] <= 159.5) {
          if (x[0] <= 81028.5) {
            votes[0] += 1;
          }

          else {
            if (x[0] <= 86177.5) {
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
      if (x[2] <= 484.5) {
        if (x[2] <= 354.0) {
          if (x[1] <= 97463.0) {
            if (x[0] <= 97356.5) {
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

        else {
          if (x[2] <= 384.0) {
            votes[1] += 1;
          }

          else {
            if (x[1] <= 99270.0) {
              votes[0] += 1;
            }

            else {
              votes[1] += 1;
            }
          }
        }
      }

      else {
        if (x[1] <= 97218.5) {
          votes[0] += 1;
        }

        else {
          if (x[2] <= 611.5) {
            if (x[0] <= 100701.5) {
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

    // tree #22
    if (x[0] <= 109864.5) {
      if (x[0] <= 98785.5) {
        if (x[0] <= 97484.5) {
          if (x[1] <= 81240.0) {
            votes[0] += 1;
          }

          else {
            if (x[2] <= 175.0) {
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
      if (x[0] <= 110372.0) {
        votes[1] += 1;
      }

      else {
        if (x[2] <= 385.0) {
          votes[0] += 1;
        }

        else {
          if (x[2] <= 421.5) {
            votes[1] += 1;
          }

          else {
            if (x[2] <= 603.0) {
              votes[0] += 1;
            }

            else {
              votes[0] += 1;
            }
          }
        }
      }
    }

    // tree #23
    if (x[1] <= 93194.0) {
      if (x[2] <= 159.5) {
        if (x[2] <= 76.0) {
          votes[0] += 1;
        }

        else {
          if (x[2] <= 114.0) {
            votes[1] += 1;
          }

          else {
            if (x[1] <= 81039.0) {
              votes[0] += 1;
            }

            else {
              votes[1] += 1;
            }
          }
        }
      }

      else {
        if (x[1] <= 92217.0) {
          if (x[1] <= 89619.5) {
            if (x[0] <= 83592.0) {
              votes[0] += 1;
            }

            else {
              votes[1] += 1;
            }
          }

          else {
            if (x[2] <= 350.5) {
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
      if (x[1] <= 97308.0) {
        votes[0] += 1;
      }

      else {
        if (x[2] <= 677.0) {
          if (x[2] <= 637.0) {
            if (x[0] <= 110931.0) {
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
    }

    // tree #24
    if (x[1] <= 99114.5) {
      if (x[2] <= 368.5) {
        if (x[2] <= 353.0) {
          if (x[0] <= 86873.0) {
            votes[0] += 1;
          }

          else {
            if (x[2] <= 201.0) {
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
        if (x[0] <= 98723.0) {
          if (x[2] <= 577.0) {
            votes[0] += 1;
          }

          else {
            if (x[2] <= 634.0) {
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
      if (x[1] <= 101612.5) {
        if (x[2] <= 600.0) {
          if (x[1] <= 100425.0) {
            votes[1] += 1;
          }

          else {
            if (x[0] <= 109266.0) {
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

    // tree #25
    if (x[1] <= 89109.0) {
      if (x[2] <= 159.5) {
        if (x[0] <= 81028.5) {
          votes[0] += 1;
        }

        else {
          if (x[0] <= 86177.5) {
            if (x[0] <= 82578.0) {
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

    else {
      if (x[1] <= 97090.5) {
        if (x[2] <= 114.0) {
          votes[1] += 1;
        }

        else {
          if (x[0] <= 92432.5) {
            votes[1] += 1;
          }

          else {
            if (x[2] <= 188.5) {
              votes[0] += 1;
            }

            else {
              votes[0] += 1;
            }
          }
        }
      }

      else {
        if (x[2] <= 611.5) {
          if (x[2] <= 515.0) {
            if (x[2] <= 218.5) {
              votes[1] += 1;
            }

            else {
              votes[0] += 1;
            }
          }

          else {
            if (x[1] <= 97763.0) {
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

    // tree #26
    if (x[0] <= 98785.5) {
      if (x[1] <= 92358.5) {
        if (x[2] <= 383.5) {
          if (x[0] <= 92218.5) {
            if (x[0] <= 86950.0) {
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
        if (x[0] <= 97933.0) {
          if (x[0] <= 94105.5) {
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
      if (x[0] <= 109669.0) {
        if (x[1] <= 97090.5) {
          votes[0] += 1;
        }

        else {
          if (x[2] <= 216.0) {
            votes[1] += 1;
          }

          else {
            if (x[2] <= 503.5) {
              votes[0] += 1;
            }

            else {
              votes[0] += 1;
            }
          }
        }
      }

      else {
        if (x[2] <= 471.5) {
          if (x[1] <= 99089.5) {
            votes[0] += 1;
          }

          else {
            votes[1] += 1;
          }
        }

        else {
          if (x[0] <= 112166.5) {
            votes[1] += 1;
          }

          else {
            votes[0] += 1;
          }
        }
      }
    }

    // tree #27
    if (x[2] <= 205.0) {
      if (x[1] <= 77825.0) {
        votes[0] += 1;
      }

      else {
        if (x[2] <= 156.0) {
          if (x[0] <= 104712.5) {
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
      if (x[0] <= 91669.5) {
        if (x[0] <= 86175.5) {
          votes[0] += 1;
        }

        else {
          votes[1] += 1;
        }
      }

      else {
        if (x[2] <= 614.5) {
          if (x[1] <= 99384.5) {
            if (x[2] <= 586.5) {
              votes[0] += 1;
            }

            else {
              votes[1] += 1;
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

        else {
          votes[0] += 1;
        }
      }
    }

    // tree #28
    if (x[1] <= 99555.5) {
      if (x[1] <= 91759.0) {
        if (x[0] <= 91589.0) {
          if (x[1] <= 80437.5) {
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
        if (x[1] <= 93194.0) {
          if (x[1] <= 92573.0) {
            votes[0] += 1;
          }

          else {
            votes[1] += 1;
          }
        }

        else {
          if (x[1] <= 97218.5) {
            votes[0] += 1;
          }

          else {
            if (x[2] <= 586.5) {
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
      if (x[1] <= 100741.5) {
        votes[1] += 1;
      }

      else {
        if (x[1] <= 103880.0) {
          if (x[1] <= 100768.5) {
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

        else {
          votes[0] += 1;
        }
      }
    }

    // tree #29
    if (x[0] <= 92706.0) {
      if (x[2] <= 212.5) {
        if (x[2] <= 148.5) {
          votes[0] += 1;
        }

        else {
          if (x[0] <= 86268.0) {
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
    }

    else {
      if (x[2] <= 201.0) {
        if (x[2] <= 161.0) {
          votes[0] += 1;
        }

        else {
          votes[1] += 1;
        }
      }

      else {
        if (x[1] <= 99749.0) {
          if (x[1] <= 97414.0) {
            votes[0] += 1;
          }

          else {
            if (x[2] <= 523.0) {
              votes[0] += 1;
            }

            else {
              votes[0] += 1;
            }
          }
        }

        else {
          if (x[2] <= 646.0) {
            if (x[1] <= 100945.5) {
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

    // tree #30
    if (x[1] <= 99145.0) {
      if (x[1] <= 93274.5) {
        if (x[1] <= 92573.0) {
          if (x[1] <= 91185.5) {
            if (x[2] <= 228.0) {
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

        else {
          votes[1] += 1;
        }
      }

      else {
        if (x[1] <= 97090.5) {
          votes[0] += 1;
        }

        else {
          if (x[0] <= 106766.0) {
            votes[0] += 1;
          }

          else {
            if (x[1] <= 97392.5) {
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
      if (x[1] <= 100404.0) {
        if (x[0] <= 112417.5) {
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
  const char *predictLabel(float *x) { return idxToLabel(predict(x)); }

  /**
   * Convert class idx to readable name
   */
  const char *idxToLabel(uint8_t classIdx) {
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
} // namespace Port
} // namespace ML
} // namespace Eloquent