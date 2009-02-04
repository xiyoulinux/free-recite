#include <vector>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <set>
#include "ConfigHolder.h"
#include "Reciter.h"
#include "WordList.h"

namespace freeRecite {

bool Reciter::load(time_t initID,bool Random) {
  if( loadWords(initID,Random) ) {
    wordList = new WordList(words.size(),configHolder.r_list());
    return true;
  }else {
    return false;
  }
}

void Reciter::test(bool result) {
  if(result) {
    if(wordList->status() == 0)
      ++score;
    wordList->pass();
  } else {
    wordList->lose();
  }
}

} //namespace freeRecite end
