#include <fstream>
#include <algorithm>
#include <ctime>
#include <set>
#include "ConfigHolder.h"
#include "Reciter.h"
#include "WordList.h"
#include "Scanner.h"


namespace freeRecite {

std::string Scanner::getTaskFileName(time_t initID) {
  struct tm * timeinfo;
  char buffer[20];
  timeinfo = localtime(&initID);
  strftime(buffer,20,"%Y%m%d%H%M%S.tkwd",timeinfo);
  return (configHolder.tasksDir() + buffer);
}

bool Scanner::loadWords(time_t initID,bool Random) {
  taskFileName = getTaskFileName(initID);
  taskID = initID;
  std::ifstream ifs(taskFileName.c_str());
  if(!ifs.is_open())
    return false;
  std::string tmpWord;
  while(ifs.good()) {
    std::getline(ifs,tmpWord);
    if(!tmpWord.empty())
      words.push_back(tmpWord);
  }

  //If Random is false, then do not call makeRandom().
  if(Random)
    if(!makeRandom())
      return false;

  wordList = new WordList(words.size());
  return true;
}

//Add a word to the current task.
bool Scanner::add(const std::string &word) {
  words.push_back(word);
  wordList->add(words.size());
  return save();
}

//Remove a word from the current task.
bool Scanner::remove(const std::string &word) {
  unsigned index = 0;
  while( index < words.size() ) {
    if(words[index] == word)
      break;
    else
      ++index;
  }
  if(index == words.size()) //Can't find this word.
    return false;

  if(wordList->status() != 0)
    r_times -= 5 - wordList->status();

  words[index] = words[words.size()-1];
  words.pop_back();
  wordList->remove(words.size());
  return save();
}

bool Scanner::save() {
  std::set<std::string> wordSet;
  for(size_t i = 0; i < words.size(); ++i)
    if( !wordSet.insert(words[i]).second)
      return false;

  std::ofstream ofs(taskFileName.c_str());
  if(!ofs.is_open())
    return false;

  std::set<std::string>::iterator itr = wordSet.begin();
  while(itr != wordSet.end()) {
    if(!ofs.good())
      return false;
    else{
      ofs << *itr << std::endl;
      ++itr;
    }
  }
  ofs.close();
  return true;
}
 
bool Scanner::makeRandom() {
  if(words.empty())
    return false;
  Random rd;
  srand(time(0));
  std::random_shuffle(words.begin(),words.end(),rd);
  return true;
}


} //End of namespace freeRecite
