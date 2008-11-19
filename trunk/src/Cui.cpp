#include <set>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <string>
#include "Cui.h"
#include "Manager.h"
#include "Dict.h"
#include "Task.h"
#include "Reciter.h"

namespace freeRecite {

CUI::CUI(const std::string &dir)
{
  manager = new Manager;
  if(!manager->load(dir)) {
    std::cerr << "Can't load the configure file from " << dir << std::endl
	      << "The program exit!\n";
    exit(EXIT_FAILURE);  
  }
  dict = new Dict(dir + "freeRecite.dict");
  if(!dict->loadDict()) {
    std::cerr << "Can't load the dic from " << dir << std::endl
	      << "The program exit!\n";
    exit(EXIT_FAILURE);  
  }
  showActive();
}

CUI::~CUI()
{
  delete manager;
  delete dict;
}


void CUI::run() {
  std::cout << "Welcome to Free Recite!"<< std::endl;
  std::cout << "Type \"help\" for help;p" << std::endl;
  while(true) {
    switch(getCmd()) {
    case 'e':{ //eixt
    exit(EXIT_SUCCESS);
    }
    case 'n':{ //creat new task
      creatNew();
      break;
    }
    case 't':{ //test
      test();
      break;
    }
    case 'r': { //recite for the new works.
      recite();
      break;
    }
    case 'm':{
      modify();
      break;
    }
    case 's':{
      showActive();
      break;
    }
    case'h':{
      help();
      break;
    }
    default:{
      std::cerr<< "Bad Command!"<<std::endl;
      break;
    }
    } //End of swithch().
  } //End of while().
} //End of run().

char CUI::getCmd() {
  std::string cmd;
  std::cout << "~_~Free Recite:> ";
  std::cout.flush();
  getLine(cmd);

  if(cmd == "exit")
    return 'e';
  else if(cmd == "new")
    return 'n';
  else if(cmd == "test")
    return 't';
  else if(cmd == "recite")
    return 'r';
  else if(cmd == "modify")
    return 'm';
  else if(cmd == "showall")
    return 's';
  else if(cmd == "help")
    return 'h';
  else
    return '*';
}

void CUI::creatNew() {
  std::string fileName,taskName;
  std::cout<<"Input the filename of the words. File Name: ";
  getLine(fileName);
  std::ifstream ifs(fileName.c_str());
  if(!ifs.is_open()) {
    std::cout << "Can't find the file.";
    return;
  }
  std::set<std::string> wordSet;
  std::string word, cmd;
  while(ifs.good()) {
    std::getline(ifs,word);
    if(!dict->lookUp(word) && !word.empty()) {
      std::cout << '\"' << word << '\"' 
		<< " can't be fond in your dict"<<std::endl
		<< "Remove(R) or Modify(M) it? (R/m)" << std::endl;
      getLine(cmd);

      if(cmd == "M" || cmd == "m")
	while(!modify(word))
		;
      else
	continue;
    }
    wordSet.insert(word);
  }
  std::cout << "Give a task name for it?" << std::endl;
  std::cout << "(Press Enter will give a default name): ";
  cmd.clear();
  getLine(cmd);
  if(manager->creatTask(wordSet,cmd.c_str())) {
    std::cout << "Creat a task SUCCESS!" << std::endl;
    manager->refresh();
  }else
    std::cerr << "Can't creat a task :}" << std::endl;
}

void CUI::testProcess(Reciter &tester) {
  clear();
  time_t starTime = 0;
  time_t endTime = 0;
  time(&starTime);
  char c;
  std::cin.get(c);
  std::string inputStr;
  bool testResult;
  std::cout << "Step 1: interpretation To key!" << std::endl;
  while(tester.isValid()) {
    if(dict->lookUp(tester.getWord())){
      std::cout<<"[W]: "<<dict->word()<<std::endl;
      std::cout<<"Press Enter to show the interpretation..."<<std::endl;
      getLine(inputStr);
      if(inputStr == "\\modify")
	modify(tester.getWord());
      else if(inputStr == "\\stop")
	return;
      else if(inputStr == "\\exit")
	exit(EXIT_FAILURE);

      std::cout<<std::endl
	       <<"********************ANSWER********************"<<std::endl
	       <<"*"<<std::endl
	       <<"* " <<dict->word()<< std::endl
	       <<"*" << std::endl
	       <<"* ["<<dict->phonetics() << "]"<<std::endl
	       <<"* " <<dict->translation() << std::endl
	       <<"*" << std::endl
	       <<"* Amount: "<< tester.capability()<<"  Unrecited Num: "
	       <<tester.size() << std::endl
	       <<"**********************************************"<<std::endl
	       <<std::endl;
      std::cout<<std::endl<<std::endl
	       <<"Have you remembered it(y/N)?";
      getLine(inputStr);
      if(inputStr == "Y" || inputStr == "y") {
	std::cout<<"   **     *  *        "<<std::endl;
	std::cout<<"  *  *    * *         "<<std::endl;
	std::cout<<"  *  *    **          "<<std::endl;
	std::cout<<"  *  *    * *         "<<std::endl;
	std::cout<<"   **     *  *        "<<std::endl;
	tester.test(true);
      }
      else {
	std::cout<<"##### ####  ####     "<<std::endl;
	std::cout<<"#     #   # #   #    "<<std::endl;
	std::cout<<"##### ####  ####     "<<std::endl;
	std::cout<<"#     #  #  #  #     "<<std::endl;
	std::cout<<"##### #   # #   #    "<<std::endl;
	tester.test(false);
      }
      clear();
    }else {
      std::cout <<"**" << tester.getWord()<<"** can't find in the Dictionary."
	        <<"modify(M) or remove(R) it? M/r"<<std::endl;
      modify();
      tester.test(true);
      clear();
    }
  }
  tester.redo();
  clear();
  std::cout << "Step 2: interpretation To key!" << std::endl;
  while(tester.isValid()) {
    if(dict->lookUp(tester.getWord())){
      std::cout<<"[M]: "<<dict->translation()<<std::endl;
      std::cout<<"Input: ";
      getLine(inputStr);
      if(inputStr == "\\modify")
	modify(tester.getWord());
      else if(inputStr == "\\stop")
	return;
      else if(inputStr == "\\exit")
	exit(EXIT_FAILURE);
      testResult = ( inputStr == tester.getWord() ? true : false );
     
      std::cout<<std::endl
	       <<"********************ANSWER********************"<<std::endl
	       <<"*"<<std::endl
	       <<"* " <<dict->word()<< std::endl
	       <<"*" << std::endl
	       <<"* ["<<dict->phonetics() << "]"<<std::endl
	       <<"* " <<dict->translation() << std::endl
	       <<"*" << std::endl
	       <<"* Amount: "<< tester.capability()<<"  Unrecited Num: "
	       <<tester.size() << std::endl
	       <<"**********************************************"<<std::endl
	       <<std::endl;
      if(testResult) {
	std::cout<<"   **     *  *        "<<std::endl;
	std::cout<<"  *  *    * *         "<<std::endl;
	std::cout<<"  *  *    **          "<<std::endl;
	std::cout<<"  *  *    * *         "<<std::endl;
	std::cout<<"   **     *  *        "<<std::endl;
      }
      else {
	std::cout<<"##### ####  ####     "<<std::endl;
	std::cout<<"#     #   # #   #    "<<std::endl;
	std::cout<<"##### ####  ####     "<<std::endl;
	std::cout<<"#     #  #  #  #     "<<std::endl;
	std::cout<<"##### #   # #   #    "<<std::endl;
      }
      tester.test(testResult);
      clear();
    }else {
      std::cout <<"**" << tester.getWord()<<"** can't find in the Dictionary."
	        <<"modify(M) or remove(R) it? M/r"<<std::endl;
      modify();
      tester.test(true);
      clear();
    }
  }
  time(&endTime);
  starTime = endTime - starTime;
  std::cout << std::endl << "Used Time: " 
	    << starTime/60 << "minutes" << std::endl;
}

void CUI::test() {
  showActive();
  if((manager->getActiveTasks()).size() == 0){
    std::cout << "There's no tasks to review!" << std::endl;
    return;
  }
  unsigned i;
  std::cout<< "Which do you want to test [ 0 by default]: ";
  std::cin >> i;
  if(i >= (manager->getActiveTasks()).size()) {
    std::cerr<< "Error task!" << std::endl;
    return;
  }

  Reciter tester;
  time_t taskID = (manager->getActiveTasks())[i];

  if(!tester.loadWords(taskID,manager->getDir().c_str())) {
    std::cerr << "error when load words!" << std::endl;
    return;
  }
  testProcess(tester);

  if(manager->test(taskID,tester.getScore())) {
    std::cout<<"Your score is " << tester.getScore() << std::endl;
    std::cout<<"You passed it!"<<std::endl;
  }
  else {
    std::cout<<"Your score is " << tester.getScore() << std::endl
	     <<"You haven't passed it :} "<<std::endl;
  }
  
  time_t nextTime = manager->getNextTime(taskID);
  if(manager->getTaskStep(taskID) == 8)
    std::cout << "Congratulations! You have complish this task!" << std::endl;
  else {
    struct tm * timeinfo;
    char buffer[30];
    timeinfo = localtime(&nextTime);
    strftime(buffer,30,"%Y.%m.%d %H:%M:%S",timeinfo);
    std::cout << "Then next reviewing time is: " << buffer
	      << std::endl << std::endl;
  }
  showActive();
  clear();
}



void CUI::recite() {
  showActive();
  if((manager->getActiveTasks()).size() == 0)
    return;
  unsigned i;
  std::cout<< "Which do you want to test: ";
  std::cin >> i;
  if(i >= (manager->getActiveTasks()).size()) {
    std::cerr<< "Error task!" << std::endl;
    return;
  }

  Reciter tester;
  if(!tester.loadWords((manager->getActiveTasks())[i],manager->getDir().c_str())) {
    std::cerr << "error when load words!" << std::endl;
    return;
  }
  testProcess(tester);

  std::cout << "Do you want to do it again?(y/N)";
  std::string YN;
  while(std::cin>>YN,YN=="Y"||YN=="y") {
    tester.redo();
    testProcess(tester);
  }
}

bool CUI::modify(const char *word) {
  std::string newItem,inputStr;
  if(word == 0) {
    std::cout<<"[W]: ";
    getLine(inputStr);
    newItem.append("[W]"+inputStr);
  }else {
    newItem.append("[W]");
    newItem.append(word);
  }
  
  std::cout<<"θ_ɑ_ʌ_ә_є_æ_ɔ_ʃ_ð_ŋ_ʒ"<<std::endl;
  std::cout<<"0_1_2_3_4_5_6_7_8_9_="<<std::endl;
  std::cout<<"[T]: ";
  getLine(inputStr);
  newItem.append("[T]"+inputStr);
  std::cout<<"[M]: ";
  getLine(inputStr);
  newItem.append("[M]"+inputStr);
  
  if(!dict->modify(newItem)) {
    std::cout<<"ERROR!" << std::endl;
    return false;
  }
  std::cout<<"SUCCESS!"<<std::endl;
  return true;

}

bool CUI::modify(const std::string &word) {
  std::string newItem,inputStr;
  if(word.empty()) {
    std::cout<<"[W]: ";
    getLine(inputStr);
    newItem.append("[W]"+inputStr);
  }else
    newItem.append("[W]"+word);
  
  std::cout<<"θ_ɑ_ʌ_ә_є_æ_ɔ_ʃ_ð_ŋ_ʒ"<<std::endl;
  std::cout<<"0_1_2_3_4_5_6_7_8_9_="<<std::endl;
  std::cout<<"[T]: ";
  getLine(inputStr);
  newItem.append("[T]"+inputStr);
  std::cout<<"[M]: ";
  getLine(inputStr);
  newItem.append("[M]"+inputStr);
  
  if(!dict->modify(newItem)) {
    std::cout<<"ERROR!" << std::endl;
    return false;
  }
  std::cout<<"SUCCESS!"<<std::endl;
  return true;
}

void CUI::showActive() {
  if(!manager->refresh()) {
    std::cout << "The configure file is broken!" << std::endl;
    exit(EXIT_FAILURE);
  }
  if(manager->getActiveTaskNum() == 0)
    std::cout << "There's no new task should be review,type \"new\" to creat?"
	      << std::endl;
  else {
    std::cout << "There's " << manager->getActiveTaskNum()
	      << " tasks should review." << std::endl;
    std::cout << "They are:" << std::endl;

    for(int i = 0; i < manager->getActiveTaskNum(); ++i) {
      
      std::cout << i << ": "
		<< manager->getTaskName(manager->getActiveTasks()[i])
		<< std::endl;
    }
  }
}


void CUI::clear() {
  for(int i = 0; i < 10; ++i)
    std::cerr << std::endl;
}

void CUI::getLine(std::string &input) {

  input.clear();
  char t;
  while(std::cin.get(t),t != '\n')
    input += t;
}

void CUI::help() {
  std::cout << "Welcome to use Free Recite!" << std::endl
	    << "There's server command here:" << std::endl
	    << " exit:     Exit Free Recite normally." << std::endl
	    << " new:      Creat a new tasks."<<std::endl
	    << " test:     Test the tasks." << std::endl
	    << " recite:   Recite the task without any mark set."<<std::endl
	    << " help:     Get help like this;p"<<std::endl;
}

} // namespace freeRecite End.
