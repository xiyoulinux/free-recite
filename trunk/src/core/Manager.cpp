#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "ConfigHolder.h"
#include "Manager.h"
#include "ForgetCurve.h"
#include "Task.h"

namespace freeRecite {

bool Manager::load() { 
  std::ifstream ifs(configHolder.mgrFile().c_str());
  if(!ifs.is_open())
    return false;
  Task tmpTask;
  unsigned amount;
  ifs >> amount;
  ifs.ignore(1,',');
  ifs >> maxTaskID;
  ifs.ignore(1,'\n');
  bool firstFlag = true;
  while(amount > 0 && ifs.good()) {    
    ifs >> tmpTask;
    allTasks[tmpTask.getID()] = tmpTask;

    if(tmpTask.shouldReview())
      activeID.push_back(tmpTask.getID());
    else if(firstFlag){
      firstReviewTime = tmpTask.getReviewTime();
      firstFlag = false;
    }else if(firstReviewTime > tmpTask.getReviewTime())
      firstReviewTime = tmpTask.getReviewTime();
    --amount;
  }
  
  if(!activeID.empty())
    firstReviewTime = 0;
  return true;
}

bool Manager::save() {
  std::ofstream ofs(configHolder.mgrFile().c_str());
  if(!ofs.is_open())
    return false;
  ofs << static_cast<unsigned>(allTasks.size()) 
      << ',' << maxTaskID << '\n';   
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
    return load();
  }else
    return false;
}

bool Manager::hasTask(time_t taskID) {
  if(allTasks.find(taskID) == allTasks.end())
    return false;
  else
    return true;
}

  
bool Manager::createTask(const std::set<std::string> &words,
			 const char *taskName, unsigned maxLimit) {
  if(words.size()==0)
    return false;
  unsigned tasksNum = words.size()/maxLimit + 1;
  maxLimit = words.size()/tasksNum;
  std::string baseName;
  std::string realName;
  std::ofstream desFile;
  std::ostringstream os;

  time_t curTime;
  struct tm * timeinfo;
  char buffer[20];
  std::set<std::string>::const_iterator itr = words.begin();
  time(&curTime);

  
  if(taskName != 0)
    baseName = taskName;

  if(baseName.empty()){
    timeinfo = localtime(&curTime);
    strftime(buffer,20,"%Y%m%d%H%M%S",timeinfo);
    baseName = buffer;
  }

  for (unsigned i = 1; i <= tasksNum; ++i) { //[0]
    memset(buffer,0,20); 
    os.str("");
    os << "_" << i;

    if(tasksNum > 1)
      realName = baseName + os.str();
    else
      realName = baseName;
    if(curTime > maxTaskID)
      maxTaskID = curTime;
    else
      ++maxTaskID;
    timeinfo = localtime(&maxTaskID);
    strftime(buffer,20,"%Y%m%d%H%M%S.tkwd",timeinfo);
    std::string desFileName = configHolder.tasksDir() + buffer;
    
    desFile.open(desFileName.c_str());
    if(!desFile.is_open()) {
      return false;
    }

    //Write words into file.
    unsigned j = 0;
    while(j++ < maxLimit && itr != words.end()) {
      if(!desFile.good()){
	return false;
      }
      if(itr->empty()) {
	++itr;
	continue;
      }else {
	desFile << *itr << std::endl;
	++itr;
      }
    }
    
    Task newTask(maxTaskID,realName.c_str());
    //Pass a copy of newTask here.
    allTasks[newTask.getID()] = newTask;
    activeID.push_back(newTask.getID());
    desFile.close();
  } //[!0]
  return true;
}

void Manager::removeTask(time_t taskID) {
  allTasks.erase(allTasks.find(taskID));
  if(save()) {
    struct tm * timeinfo;
    char buffer[20];
    timeinfo = localtime(&taskID);
    strftime(buffer,20,"%Y%m%d%H%M%S.tkwd",timeinfo);
    std::string taskFileName = configHolder.tasksDir() + buffer;
    remove(taskFileName.c_str());
  }
}

const std::vector<time_t> &Manager::getActiveTasks() const{
  return activeID;
}

//Get the number of the active tasks.
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

//This is a global variable.
Manager manager;

} //namespace freeRecite end
