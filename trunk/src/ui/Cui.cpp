#include <set>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <cstring>
#include <Manager.h>
#include <Dict.h>
#include <Task.h>
#include <Reciter.h>
#include <Tester.h>
#include <ConfigHolder.h>
#include <iomanip>
#include "Cui.h"

namespace freeRecite {

CUI::CUI()
{
  if(!manager.load()) {
    std::cerr << "Can't load " << configHolder.mgrFile() << ".\n";
    std::cerr << "You should use command \'frt-init\' to initialize.\n";
    exit(EXIT_FAILURE);  
  }
  if(!dictionary.load()) {
    std::cerr << "Can't load " << configHolder.dictFile() << ".\n";
    std::cerr << "You should use command \'frt-init\' to initialize.\n";
    exit(EXIT_FAILURE);  
  }
}

CUI::~CUI()
{
}

void CUI::run(int argc, char *argv[]) {
  if(argc == 1) {
    help();
    return;
  }else if(argc == 2){
    if(!strcmp(argv[1],"--help")) {
      help();
      return;
    }else if(!strcmp(argv[1],"--version")) {
      std::cout << "Free Recite version 1.0" << std::endl;
      return;      
    }else if(!strcmp(argv[1],"cls")) {
      cleanStress();
      return;
    }else if(!strcmp(argv[1],"ls")) {
      showActive();
      return;
    }else if(!strcmp(argv[1],"stress")) {
      exportStress();
      return;
    }
  }else if(argc == 3) {
    time_t taskID;
    if(!strcmp(argv[1],"export")) {
      if( ( taskID = atoid(argv[2]) ) != 0 ) {
	exportTask(taskID);
	return;
      }
    }else if(!strcmp(argv[1],"new")) {
      createNew(argv[2]);
      return;
    }else if(!strcmp(argv[1],"recite")) {
      if( ( taskID = atoid(argv[2]) ) != 0 ) {
	recite(taskID);
	return;
      }
    }else if(!strcmp(argv[1],"remove")) {
      if( ( taskID = atoid(argv[2]) ) != 0 ){
	remove(taskID);
	return;
      }
    }else if(!strcmp(argv[1],"test")) {
      if( ( taskID = atoid(argv[2]) ) != 0 ) {
	test(taskID);
	return;
      }
    }
  } //else if(argc == 3)

  std::cout << "frt: \'" << argv[argc-1] << "\' is not a frt-command. "
	    << "See \'frt --help\'." << std::endl;
} //End of run().

time_t CUI::atoid(const char *argv) {
  std::istringstream ssm(argv);
  unsigned tid;
  if((ssm >> tid))
    return static_cast<time_t>(tid);
  else 
    return static_cast<time_t>(0);
}

void CUI::cleanStress() {
  ::remove(configHolder.keyFile().c_str());
}

void CUI::clear() {
  for(int i = 0; i < 10; ++i)
    std::cerr << std::endl;
}

void CUI::createNew(const char *fileName) {
  std::string taskName;
  std::ifstream ifs(fileName);
  if(!ifs.is_open()) {
    std::cout << "Can't find the file." << std::endl;
    return;
  }
  std::set<std::string> wordSet;
  std::string word, name, cmd;
  std::cout << "Give a task name for it?" << std::endl;
  std::cout << "(Press Enter will give a default name): ";
  getLine(name);
  while(ifs.good()) {
    std::getline(ifs,word);
    if(!dictionary.lookUp(word) && !word.empty()) {
      std::cout << '\"' << word << '\"' 
		<< " can't be fond in your dictionary"<<std::endl
		<< "Remove(R) or Modify(M) it? (R/m)" << std::endl;
      getLine(cmd);

      if(cmd == "M" || cmd == "m")
	while(!modify(word))
	  /* An empty sentance. */;
      else
	continue;
    }
    wordSet.insert(word);
  }
  if(manager.createTask(wordSet,name.c_str(),30)) {
    std::cout << "Creat a task SUCCESS!" << std::endl;
    manager.refresh();
  }else
    std::cerr << "Can't creat a task :}" << std::endl;
}


void CUI::exportFromFile(const char *fileName) {
  std::ifstream ifs(fileName);
  std::string tmpWord;
  std::set<std::string> wdSet;
  if(!ifs.is_open()) {
    std::cerr << "There's no words should be remind;p!" << std::endl;
    return;
  }
  while(ifs.good()) {
    std::getline(ifs, tmpWord);
    if(!tmpWord.empty())
      wdSet.insert(tmpWord);
  }
  ifs.close();
  std::set<std::string>::const_iterator itr = wdSet.begin();
  while(itr != wdSet.end()) {
    if(dictionary.lookUp(*itr)) {
      std::cout << std::setw(12) << std::setfill(' ') << std::left
	  <<  dictionary.word() << ' ';
      if(!dictionary.phonetics().empty())
	std::cout << '[' << dictionary.phonetics() << ']';
      std::cout << ' ' << dictionary.translation() << std::endl;
    }else {
      std::cout << *itr << " *** " << std::endl;
    }
    ++itr;
  }
}

void CUI::exportTask(time_t taskID) {
  if(!manager.hasTask(taskID)) {
    std::cout << "There's no this task!" << std::endl;
    return;
  }
  std::string fileName = Scanner::getTaskFileName(taskID);
  exportFromFile(fileName.c_str());
}

void CUI::exportStress(){
  exportFromFile(configHolder.keyFile().c_str());
}


void CUI::test(time_t taskID) {
  Tester tester;
  if(!manager.hasTask(taskID)) {
    std::cout << "There's no this task!" << std::endl;
    return;
  }
  std::string inputStr;
  if(!tester.loadWords(taskID, true)) {
    std::cerr << "error when load words!" << std::endl;
    return;
  }
  time_t startTime = 0;
  time_t endTime = 0;
  time(&startTime);
  bool result;
  while(tester.isValid()) {
    if(dictionary.lookUp(tester.getWord())){
      clear();
      std::cout <<" Amount: "<< tester.capability()
		<<" R_Num: " << tester.size()
		<<" R_Times: "<< tester.times()<<std::endl;
      std::cout<<"[M]: "<<dictionary.translation()<<std::endl;
      std::cout <<"**********************************************" << std::endl;
      std::cout<<"*Input : ";
      getLine(inputStr);
      if(inputStr == "\\modify")
	modify(tester.getWord());
      else if(inputStr == "\\add"){
	std::cout << "Input new word: ";
	getLine(inputStr);
	tester.add(inputStr);
	std::cout << "SUCCESS!" << std::endl;
	continue;
      } else if(inputStr == "\\rm" || inputStr == "remove"){
	inputStr = tester.getWord();
	tester.remove(inputStr);
	std::cout << "SUCCESS!" << std::endl;
	continue;
      } else if(inputStr == "\\stop") {
	return;
      }
      result = ( inputStr == tester.getWord() ? true : false );
      std::cout << "*Answer: " << dictionary.word();
      if( !dictionary.phonetics().empty() )
	std::cout << "  /" << dictionary.phonetics() << "/" << std::endl;
      else
	std::cout << std::endl;
      std::cout <<"**********************************************" << std::endl;
      showResult(result);
      tester.test(result);
    } else { //If the dictionary can't look up the current word
      std::cout << '\"'	<< tester.getWord() << '\"'
		<< " can't be found in your dictionary."
		<< "\n Modify Or Remove it from task(M/r) ";
      std::string m_r;
      getLine(m_r);
      if(m_r == "R" || m_r == "r") {
	inputStr = tester.getWord();
	tester.remove(inputStr);
	std::cout << "SUCCESS!" << std::endl;
      } else {
	modify(tester.getWord());
      }
    } 
  }  //End of while()

  time(&endTime);
  startTime = endTime - startTime;
  std::cout << std::endl << "Used Time: " 
	    << startTime/60 << "minutes" << std::endl;

  if(manager.test(taskID,tester.getScore())) {
    std::cout<<"Your score is " << tester.getScore() << std::endl;
    std::cout<<"You passed it!"<<std::endl;
  }
  else {
    std::cout<<"Your score is " << tester.getScore() << std::endl
	     <<"You haven't passed it :} "<<std::endl;
  }
  
  time_t nextTime = manager.getNextTime(taskID);
  if(manager.getTaskStep(taskID) == 8) {
    manager.removeTask(taskID);
    std::cout << "Congratulations! You have complish this task!" 
	      << std::endl;
  }else {
    struct tm * timeinfo;
    char buffer[30];
    timeinfo = localtime(&nextTime);
    strftime(buffer,30,"%Y.%m.%d %H:%M:%S",timeinfo);
    std::cout << "Then next reviewing time is: " << buffer
	      << std::endl << std::endl;
  }
}

void CUI::recite(time_t taskID) {

  if(!manager.hasTask(taskID)) {
    std::cerr<< "There's no this task!" << std::endl;
    return;
  }

  Reciter reciter;
  std::string inputStr;
  if(!reciter.loadWords(taskID,false)) {
    std::cerr << "error when load words!" << std::endl;
    return;
  }
  time_t startTime = 0;
  time_t endTime = 0;
  time(&startTime);
  while(reciter.isValid()) {
    if(dictionary.lookUp(reciter.getWord())){
      clear();
      std::cout <<" Amount: "<< reciter.capability()
		<<" R_Num: " << reciter.size()
		<<" R_Times: "<< reciter.times()<<std::endl;
      std::cout<<"[M]: "<<dictionary.translation()<<std::endl;
      std::cout <<"**********************************************" << std::endl;
      std::cout<<"*Input : ";
      getLine(inputStr);
      if(inputStr == "\\modify"){
	modify(reciter.getWord());
      } else if(inputStr == "\\add"){
	std::cout << "Input new word: ";
	getLine(inputStr);
	reciter.add(inputStr);
	std::cout << "SUCCESS!" << std::endl;
	continue;
      } else if(inputStr == "\\rm" || inputStr == "remove"){
	inputStr = reciter.getWord();
	reciter.remove(inputStr);
	std::cout << "SUCCESS!" << std::endl;
	continue;
      } else if(inputStr == "\\stop"){
	return;
      }
      bool result = ( inputStr == reciter.getWord() ? true : false );
      std::cout << "*Answer: " << dictionary.word();
      if( !dictionary.phonetics().empty() )
	std::cout << "  /" << dictionary.phonetics() << "/" << std::endl;
      else
	std::cout << std::endl;
      std::cout <<"**********************************************" << std::endl;
      showResult(result);
      reciter.test(result);
    }else { //If the dictionary can't look up the current word
      std::cout << '\"'	<< reciter.getWord() << '\"'
		<< " can't be found in your dictionary."
		<< "\n Modify Or Remove it from task(M/r) ";
      std::string m_r;
      getLine(m_r);
      if(m_r == "R" || m_r == "r") {
	inputStr = reciter.getWord();
	reciter.remove(inputStr);
	std::cout << "SUCCESS!" << std::endl;
      } else {
	modify(reciter.getWord());
      }
    }
  }

  time(&endTime);
  startTime = endTime - startTime;
  std::cout << std::endl << "Used Time: " 
	    << startTime/60 << "minutes" << std::endl;
  float r_num = static_cast<float>(reciter.capability() - reciter.getScore());
  std::cout << "There're " << reciter.capability() << "words in this task. "
	    << reciter.getScore() << " of it you have known before." 
	    << std::endl << "Your Reciting Rate is: "
	    << r_num / static_cast<float>(startTime/60)
	    << " word(s)/min" << std::endl;
}




void CUI::remove(time_t taskID) {
  if(!manager.hasTask(taskID)) {
    std::cerr<< "There's no this task!" << std::endl;
    return;
  }
  manager.removeTask(taskID);
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
  
  if(!dictionary.modify(newItem)) {
    std::cout<<"ERROR!" << std::endl;
    return false;
  }
  std::cout<<"SUCCESS!"<<std::endl;
  return true;
}

void CUI::showActive() {
  if(!manager.refresh()) {
    std::cout << "The configure file is broken!" << std::endl;
    exit(EXIT_FAILURE);
  }
  if(manager.getActiveTaskNum() == 0) {

    std::cout << "There's no new task should be review,type \"new\" to creat one."
	      << std::endl;

    if(manager.getNextTime() != 0) {
      time_t nextTime = manager.getNextTime();
      struct tm * timeinfo;
      char buffer[30];
      timeinfo = localtime(&nextTime);
      strftime(buffer,30,"%Y.%m.%d %H:%M:%S",timeinfo);
      std::cout << "The nearest reviewing time is at " << buffer
		<< std::endl
		<< "Please use FreeRecite at that time!"
		<< std::endl;
    }
  }
  else {
    std::cout << "There's " << manager.getActiveTaskNum()
	      << " tasks should review." << std::endl;
    std::cout << std::setw(3) << std::setfill(' ') << std::left << "N" 
	      << std::setw(20) << std::setfill(' ') << std::left << "Name"
	      << std::setw(5) << std::setfill(' ') << std::left  << "Step"
	      << std::setw(10) << std::setfill(' ') << std::right  << "ID    "
	      <<std::endl;

    for(int i = 0; i < manager.getActiveTaskNum(); ++i) {
      std::cout << std::setw(3) << std::setfill(' ') << std::left << i 
		<< std::setw(20) << std::setfill(' ') << std::left
		<< manager.getTaskName(manager.getActiveTasks().at(i))
		<< std::setw(5) << std::setfill(' ') << std::left
		<< manager.getTaskStep(manager.getActiveTasks().at(i))
		<< std::setw(10) << std::setfill(' ') << std::right
		<< manager.getActiveTasks().at(i)
		<< std::endl;
    }
  }
}



void CUI::getLine(std::string &input) {
  input.clear();
  char t;
  while(std::cin.get(t),t != '\n')
    input += t;
}

void CUI::showResult(bool result) {
  if(result) {
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
}

void CUI::help() {
  std::cout << "usage: frt [--version]  [--help] COMMAND [ARGS]" << std::endl
	    << "The most commonly used git commands are:" << std::endl
	    << " cls                 Clean the strees' words in the system" << std::endl
	    << " export <taskID>     Export a tasks' words to the screen" << std::endl
	    << " ls                  List the should reviewed tasks's information" << std::endl
	    << " new    <filename>   Creat new tasks with the words in the file" << std::endl
	    << " recite <taskID>     Recite the task whose ID is taskID"<<std::endl
	    << " remove <taskID>     Remove a task which you don't want to recite" << std::endl
	    << " stress              Show some words which may be difficult for you" << std::endl
	    << " test   <taskID>     Test the task whose ID is taskID" << std::endl
	    << " --help              Show this help information" << std::endl
	    << " --version           Show the current version" << std::endl;
}

} // namespace freeRecite End.
