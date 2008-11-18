#include "Task.h"
#include "ForgetCurve.h"

namespace freeRecite {

Task::~Task() {
 /*
   if(curve != 0)
   delete curve; 
 */
}

std::istream& 
operator>>(std::istream &istrm, Task &tsk) {

  istrm >> tsk.id;
  istrm.ignore(1,',');

  istrm >> tsk.reviewTime;
  istrm.ignore(1,',');

  istrm >> tsk.step;
  istrm.ignore(1,',');

  istrm >> tsk.name;
  istrm.ignore(1,'\n');
  return istrm;
}

std::ostream& 
operator<<(std::ostream &ostrm, const Task &tsk) {
  ostrm << tsk.id << ','
	<< tsk.reviewTime << ','
	<< tsk.step << ','
	<< tsk.name << '\n';
  return ostrm;
}

bool operator>(const Task &src, const Task &tar) {
  return src.reviewTime > tar.reviewTime;
}

bool operator<(const Task &src, const Task &tar) {
  return src.reviewTime < tar.reviewTime;
}

bool operator==(const Task &src, const Task &tar) {
  return src.id == tar.id;
}

bool Task::shouldReview() const {
  std::time_t current;
  std::time(&current);
  return reviewTime <= current;
}

bool Task::test(int mark) {
  if( forgetCurve.pass(step,mark) ) {
    if(step == 8){
      reviewTime = 0;
    } else {
      ++step;
      reviewTime = forgetCurve.getReviewTime(step);
    }
    return true;
  } else {
    reviewTime = forgetCurve.getReviewTime(step);
    return false;
  }
}

}//namespace freeRecite end
