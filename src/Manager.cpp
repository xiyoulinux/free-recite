#include <fstream>
#include <set>
#include <map>
#include <ctime>

#include "Manager.h"
#include "ForgetCurve.h"
#include "Task.h"

namespace freeRecite {

static const std::string managerFile = "freeRecite.mgr";

bool Manager::load(const std::string &dir) {
  std::string mgrFile = dir+freeRecite::managerFile;
  std::ifstream ifs(mgrFile.c_str());
  if(!ifs.is_open())
    return false;
  Task tmpTask;
  unsigned amount;
  ifs >> amount;
  ifs.ignore(1,'\n');
  while(amount > 0 && ifs.good()) {    
    ifs >> tmpTask;
    allTasks[tmpTask.getID()] = tmpTask;
    if(tmpTask.shouldReview())
      activeID.push_back(tmpTask.getID());
    --amount;
  }
  mgrDir = dir;
  return true;
}

bool Manager::save() {
  std::string mgrFile = mgrDir + freeRecite::managerFile;
  std::ofstream ofs(mgrFile.c_str());
  if(!ofs.is_open())
    return false;
  ofs << static_cast<unsigned>(allTasks.size()) << '\n';   
  std::map<time_t,Task>::const_iterator itr = allTasks.begin();
  while(itr != allTasks.end()){
    if(!ofs.good())
      return false;
    else {
      if((itr->second).isAvailable())
	ofs << itr->second;
      ++itr;
    } 
  }
  return true;
}

bool Manager::refresh() {
  if(save()) {
    activeID.clear();
    allTasks.clear();
    return load(mgrDir);
  }else
    return false;
}

bool Manager::hasTask(time_t taskID) {
  if(allTasks.find(taskID) == allTasks.end())
    return true;
  else
    return false;
}

bool Manager::creatTask(const std::set<std::string> &words,
			const char *taskName) {
  time_t curTime;
  struct tm * timeinfo;
  char buffer[20];

  time(&curTime);
  timeinfo = localtime(&curTime);
  strftime(buffer,20,"%Y%m%d%H%M%S.tkwd",timeinfo);

  std::string desFileName = mgrDir + "tasks/" + buffer;

  if(words.size()==0)
    return false;

  /**
   * If there're some new words, then load them to this
   * soft ware's standard file. By the way, the desFile
   * coincide with the task's ID.
   **/
  std::ofstream desFile(desFileName.c_str());
  if(!desFile.is_open())
    return false;

  std::set<std::string>::const_iterator itr_words = words.begin();
  while(itr_words != words.end()) {
    if(!desFile.good())
      return false;
    if(!itr_words->empty())
      desFile << *itr_words << std::endl;
    ++itr_words;
  }

  Task *newTask;
  //Choose the task's name.
  if(taskName == 0)
    newTask = new Task(curTime,buffer);
  else
    newTask = new Task(curTime,taskName);

  allTasks[newTask->getID()] = *newTask;
  activeID.push_back(newTask->getID());
  delete newTask;
  return true;
}

const std::vector<time_t> &Manager::getActiveTasks() const{
  return activeID;
}

int Manager::getActiveTaskNum() const {
  return activeID.size();
}

//Get the task's name.
const std::string &Manager::getTaskName(time_t taskID)const {
  return (allTasks.find(taskID)->second).getName();
}


time_t Manager::getNextTime(time_t taskID) const {
  return (allTasks.find(taskID)->second).getReviewTime();
} 
//Get the step of the task.
int Manager::getTaskStep(time_t taskID)const {
  return (allTasks.find(taskID)->second).getStep();
}

bool Manager::test(time_t taskID,int mark) {
  bool result = (allTasks[taskID]).test(mark);
  return (refresh() && result);
}

} //namespace freeRecite end
