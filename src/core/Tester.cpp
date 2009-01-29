#include "Tester.h"
#include "ConfigHolder.h"
#include <fstream>

namespace freeRecite {

Tester::~Tester() {
  std::ifstream ifs(configHolder.keyFile().c_str());
  std::string tmpWord;

  while(ifs.good()) {
    std::getline(ifs, tmpWord);
    if(!tmpWord.empty())
      errWords.insert(tmpWord);
  }
  ifs.close();

  std::ofstream ofs(configHolder.keyFile().c_str());
  std::set<std::string>::const_iterator itr = errWords.begin();
  while(itr != errWords.end()) {
    ofs << *itr << std::endl;
    ++itr;
  }
  ofs.close();
}

void Tester::test(bool result) {
  if(result) {
    if(wordList->status() == 0)
      ++score;
    --r_times;
    wordList->pass();
  } else {
    if( wordList->status() == 0) {
      errWords.insert(getWord());
      r_times += 4;
    }else {
      r_times += wordList->status()-1;
    }
    wordList->lose();
  }
}

} //End of namespace freeRecite
