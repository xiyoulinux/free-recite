#include "ForgetCurve.h"

namespace freeRecite {
  
ForgetCurve::ForgetCurve() {

  /**
   * This result came from Hermann Ebbinghaus' test.
   * For more information look the following link:
   * http://en.wikipedia.org/wiki/Forgetting_curve
   */

  interval[0] = 0;               // Remember form here
  interval[1] = 1200;            // 20 minutes
  interval[2] = 3600;            //  1 hour
  interval[3] = 28800;           //  8 hours
  interval[4] = 86400;           //  1 day
  interval[5] = 172800;          //  2 days
  interval[6] = 518400;          //  6 days
  interval[7] = 2678400;         // 31 days
    
  mark[0] = 90;
  mark[1] = 58;
  mark[2] = 76;
  mark[3] = 80;
  mark[4] = 85;
  mark[5] = 93;
  mark[6] = 95;
  mark[7] = 95;
}


time_t ForgetCurve::getReviewTime(int curOrder) const{
  time_t current;
  time(&current);
  return current + interval[curOrder];
}


ForgetCurve forgetCurve;

} //namespace freeRecite end
