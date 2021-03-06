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
    }else if(!strcmp(argv[1],"all")) {
      showAll();
      return;
    }else if(!strcmp(argv[1],"cls")) {
      cleanStress();
      return;
    }else if(!strcmp(argv[1],"done")) {
      exportDone();
      return;
    }else if(!strcmp(argv[1],"ls")) {
      showActive();
      return;
    }else if(!strcmp(argv[1],"modify")) {
      modify(std::string(""));
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
    }else if(!strcmp(argv[1],"modify")) {
      modify(std::string(argv[2]));
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
    while(!dictionary.lookUp(word) && !word.empty()) {
      std::cout << '\"' << word << '\"' 
		<< " can't be fond in your dictionary"<<std::endl
		<< "Remove(R),Modify(M) or Add to dictionary(A)? (R/m/a)"
		<< std::endl;
      getLine(cmd);

      if(cmd == "A" || cmd == "a")
	while(!modify(word)) /* An empty sentance. */;
      else if(cmd == "M" || cmd == "m") {
	std::cout << "Input the new word: ";
	getLine(word); //Get a new word from the user.
	continue;
      } else
	std::getline(ifs,word); //Read a new word from file.
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

void CUI::exportDone() {
  exportFromFile(configHolder.doneFile().c_str());
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
  if(!tester.load(taskID)) {
    std::cerr << "error when load words!" << std::endl;
    return;
  }
  time_t startTime = 0;
  time_t endTime = 0;
  time(&startTime);

  scanProcess(tester);

  time(&endTime);
  startTime = endTime - startTime;
  std::cout << std::endl << "Used Time: " 
	    << startTime/60 << "minutes" << std::endl;
  int testResult = manager.test(taskID,tester.getScore());
  if(testResult == 0) {
    std::cout<<"Your score is " << tester.getScore() << std::endl;
    std::cout<<"You passed it!"<<std::endl;
    time_t nextTime = manager.getNextTime(taskID);
    struct tm * timeinfo;
    char buffer[30];
    timeinfo = localtime(&nextTime);
    strftime(buffer,30,"%Y.%m.%d %H:%M:%S",timeinfo);
    std::cout << "Then next reviewing time is: " << buffer
	      << std::endl;
  }else if(testResult == -1){
    std::cout<<"Your score is " << tester.getScore() << std::endl
	     <<"You haven't passed it :} "<<std::endl;
  }else {  // testResult == 1
    std::cout << "Congratulations! You have complish this task!" 
	      << std::endl;
  }
}

void CUI::recite(time_t taskID) {
  if(!manager.hasTask(taskID)) {
    std::cerr<< "There's no this task!" << std::endl;
    return;
  }

  Reciter reciter;
  if(!reciter.load(taskID)) {
    std::cerr << "error when load words!" << std::endl;
    return;
  }
  time_t startTime = 0;
  time_t endTime = 0;
  time(&startTime);
  scanProcess(reciter);
  time(&endTime);
  startTime = (endTime - startTime)/60;
  time_t usedTime = startTime > 0 ? startTime : 1;
  std::cout << std::endl << "Used Time: " 
	    << usedTime << " minutes" << std::endl;
  float r_num = reciter.capability() - reciter.getScore();
  std::cout << "There're " << reciter.capability() << " words in this task. "
	    << reciter.getScore() << " of them you have known before." 
	    << std::endl << "Your Reciting Rate is: "
	    << r_num / usedTime
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
    std::cout << std::setw(5) << std::setfill(' ') << std::left << "N" 
	      << std::setw(25) << std::setfill(' ') << std::left << "Name"
	      << std::setw(5) << std::setfill(' ') << std::left  << "Step"
	      << std::setw(10) << std::setfill(' ') << std::left  << "     ID"
	      <<std::endl;

    for(int i = 0; i < manager.getActiveTaskNum(); ++i) {
      std::cout << std::setw(5) << std::setfill(' ') << std::left << i+1
		<< std::setw(25) << std::setfill(' ') << std::left
		<< manager.getTaskName(manager.getActiveTasks().at(i)) << ' '
		<< std::setw(5) << std::setfill(' ') << std::left
		<< manager.getTaskStep(manager.getActiveTasks().at(i))
		<< std::setw(10) << std::setfill(' ') << std::right
		<< manager.getActiveTasks().at(i)
		<< std::endl;
    }
  }
}

void CUI::showAll() {
  if(!manager.refresh()) {
    std::cout << "The configure file is broken!" << std::endl;
    exit(EXIT_FAILURE);
  }
  if(manager.getAllTasks().empty()) {
    std::cout << "There's no new task should be review,type \"new\" to creat one."
	      << std::endl;
  }
  else {
    std::cout << "There's " << manager.getAllTasks().size()
	      << " tasks should review." << std::endl;
    std::cout << std::setw(5) << std::setfill(' ') << std::left << "N" 
	      << std::setw(20) << std::setfill(' ') << std::left << "Name"
	      << std::setw(5) << std::setfill(' ') << std::left  << "Step"
	      << std::setw(12) << std::setfill(' ') << std::left  << "       ID"
	      << std::setw(12) << std::setfill(' ') << std::left << "       Review Time"
	      <<std::endl;

    std::map<time_t,Task>::const_iterator itr = manager.getAllTasks().begin();
    unsigned i = 0;
    while(++i,itr != manager.getAllTasks().end()) {
      std::cout << std::setw(5) << std::setfill(' ') << std::left << i
		<< std::setw(20) << std::setfill(' ') << std::left
		<< manager.getTaskName(itr->first) << ' '
		<< std::setw(5) << std::setfill(' ') << std::left
		<< manager.getTaskStep(itr->first)
		<< std::setw(12) << std::setfill(' ') << std::right
		<< itr->first << "  ";
      time_t nextTime = manager.getNextTime(itr->first);
      struct tm * timeinfo;
      char buffer[30];
      timeinfo = localtime(&nextTime);
      strftime(buffer,30,"%Y.%m.%d %H:%M:%S",timeinfo);
      std::cout << std::setw(12) << std::setfill(' ') << std::left
		<< buffer << std::endl;
      ++itr;
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

void CUI::scanProcess(Scanner &scanner) {
  bool result;
  std::string inputStr;
  while(scanner.isValid()) {
    if(dictionary.lookUp(scanner.getWord())){
      clear();
      std::cout <<" Type \'\\help\' to show the implicit command!" 
		<< std::endl;
      std::cout <<" Amount: "<< scanner.capability()
		<<" R_Num: " << scanner.size()
		<<" R_Times: "<< scanner.times()<<std::endl;
      std::cout<<"[M]: "<<dictionary.translation()<<std::endl;
      std::cout <<"**********************************************" << std::endl;
      std::cout<<"*Input : ";
      getLine(inputStr);

      if(inputStr == "\\hint") {
	std::cout << "*Hint  : " << dictionary.word().at(0);
	for(unsigned i = 1; i < dictionary.word().size(); ++i)
	  std::cout << '_';
	if( !dictionary.phonetics().empty() )
	  std::cout << "  /" << dictionary.phonetics() << "/";
	std::cout << std::endl <<"*Input : ";
	getLine(inputStr);
      } 

      if(inputStr == "\\help"){
	scanProHelp();
	continue;
      } else if(inputStr == "\\modify"){
	modify(scanner.getWord());
	continue;
      } else if(inputStr == "\\add"){
	std::cout << "Input new word: ";
	getLine(inputStr);
	scanner.add(inputStr);
	std::cout << "SUCCESS!" << std::endl;
	continue;
      } else if(inputStr == "\\rm"){
	inputStr = scanner.getWord();
	scanner.remove(inputStr);
	std::cout << "SUCCESS!" << std::endl;
	continue;
      } else if(inputStr == "\\stop") {
	exit(EXIT_SUCCESS);
      }
      result = ( inputStr == scanner.getWord() ? true : false );
      std::cout << "*Answer: " << dictionary.word();
      if( !dictionary.phonetics().empty() )
	std::cout << "  /" << dictionary.phonetics() << "/";
      std::cout << std::endl;
      std::cout <<"**********************************************" << std::endl;
      showResult(result);
      scanner.test(result);
    } else { //If the dictionary can't look up the current word
      std::cout << '\"'	<< scanner.getWord() << '\"'
		<< " can't be found in your dictionary."
		<< "\n Modify Or Remove it from task(M/r) ";
      std::string m_r;
      getLine(m_r);
      if(m_r == "R" || m_r == "r") {
	inputStr = scanner.getWord();
	scanner.remove(inputStr);
	std::cout << "SUCCESS!" << std::endl;
      } else {
	modify(scanner.getWord());
      }
    } 
  }  //End of while()
}


void CUI::scanProHelp() {
  std::cout << std::endl << "usage: \\command " << std::endl
	    << "\\help    Show this help information" << std::endl
	    << "\\add     Add new word to this task" << std::endl
	    << "\\rm      Remove the current word from this task" << std::endl
	    << "\\modify  Modify the current word in the dictionary" << std::endl
	    << "\\hint    Get the hint of current word" << std::endl
	    << "\\stop    Stop Free Recite at once" << std::endl;
}

void CUI::help() {
  std::cout << "usage: frt [--version]  [--help] COMMAND [ARGS]" << std::endl
	    << "The most commonly used git commands are:" << std::endl
	    << " all                 Show the detail of all the tasks"
	    << std::endl
	    << " cls                 Clean the strees' words in the system" 
	    << std::endl
	    << " done                Export the words which you have remembered"
	    <<std::endl
	    << " export <taskID>     Export a tasks' words to the screen"
	    << std::endl
	    << " ls                  List the information of the tasks that should be reviewed"
	    << std::endl
	    << " new    <filename>   Creat new tasks with the words in the file" 
	    << std::endl
	    << " modify [word]       Modify the word in the dictionary"
	    << std::endl
	    << " recite <taskID>     Recite the task whose ID is taskID"
	    <<std::endl
	    << " remove <taskID>     Remove a task which you don't want to recite"
	    << std::endl
	    << " stress              Show some words which may be difficult for you" 
	    << std::endl
	    << " test   <taskID>     Test the task whose ID is taskID"
	    << std::endl
	    << " --help              Show this help information"
	    << std::endl
	    << " --version           Show the current version"
	    << std::endl;
}

} // namespace freeRecite End.
