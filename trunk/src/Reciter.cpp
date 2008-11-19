#include <vector>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <set>
#include "Reciter.h"
#include "WordList.h"

namespace freeRecite {

bool Reciter::loadWords(time_t taskID,const char *configDir) {
  struct tm * timeinfo;
  char buffer[20];
  timeinfo = localtime(&taskID);
  strftime(buffer,20,"%Y%m%d%H%M%S.tkwd",timeinfo);

  taskFileName.clear();
  taskFileName.append(configDir);
  taskFileName.append("tasks/");
  taskFileName.append(buffer);

  std::ifstream ifs(taskFileName.c_str());
  if(!ifs.is_open())
    return false;
  std::string tmpWord;
  while(ifs.good()) {
    std::getline(ifs,tmpWord);
    if(!tmpWord.empty())
      words.push_back(tmpWord);
  }
  if(!makeRandom())
    return false;

  wordList = new WordList(words.size());
  return true;
}

bool Reciter::redo() {
  if(makeRandom()) {
    delete wordList;
    wordList = new WordList(words.size());
    score = 0;
    return true;
  }else
    return false;
}

bool Reciter::addWord(const std::string &word) {
  std::set<std::string> wordSet;
  for(size_t i = 0; i < words.size(); ++i)
    if( !(wordSet.insert(words[i])).second)
      return false;

  if(!wordSet.insert(word).second)
    return false;

  std::ofstream ofs(taskFileName.c_str());
  if(!ofs.is_open())
    return false;

  std::set<std::string>::const_iterator itr = wordSet.begin();
  while(itr != wordSet.end()) {
    if(!ofs.good())
      return false;
    else{
      ofs << *itr << std::endl;
      ++itr;
    }
  }
  ofs.close();
  return reload();
}

bool Reciter::removeWord(const std::string &word) {
  std::vector<std::string>::iterator itr = words.begin();
  while(itr != words.end()) {
    if(*itr == word) {
      words.erase(itr);
      break;
    }else
      ++itr;
  }
  
  std::set<std::string> wordSet;
  for(size_t i = 0; i < words.size(); ++i)
    if( !wordSet.insert(words[i]).second)
      return false;

  std::ofstream ofs(taskFileName.c_str());
  if(!ofs.is_open())
    return false;

  std::set<std::string>::iterator sitr = wordSet.begin();
  while(sitr != wordSet.end()) {
    if(!ofs.good())
      return false;
    else{
      ofs << *sitr << std::endl;
      ++sitr;
    }
  }
  ofs.close();
  return reload();
}

void Reciter::test(bool result) {
  if(result) {
    if(wordList->pass() == 0)
      ++score;
  }  else
    wordList->lose();
}

bool Reciter::reload() {
  words.clear();

  std::ifstream ifs(taskFileName.c_str());
  if(!ifs.is_open())
    return false;
  std::string tmpWord;
  while(ifs.good()) {
    std::getline(ifs,tmpWord);
    if(!tmpWord.empty())
      words.push_back(tmpWord);
  }
  if(!makeRandom())
    return false;
  score = 0;
  delete wordList;
  wordList = new WordList(words.size());
  return true;
}

bool Reciter::makeRandom() {
  if(words.empty())
    return false;
  Random rd;
  srand(time(0));
  std::random_shuffle(words.begin(),words.end(),rd);
  return true;
}

} //namespace freeRecite end
