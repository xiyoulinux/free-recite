#include <vector>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <set>
#include "ConfigHolder.h"
#include "Reciter.h"
#include "WordList.h"

namespace freeRecite {

bool Reciter::redo(bool Random) {
  if(Random && !makeRandom())
    return false;

  if(wordList != 0)
    delete wordList;
  wordList = new WordList(words.size());
  r_times = 0;
  return true;
}

void Reciter::test(bool result) {
  if(result) {
    if(wordList->status() == 0)
      ++score;
    --r_times;
    wordList->pass();
  } else {
    if( wordList->status() == 0) {
      r_times += 4;
    }else {
      r_times += wordList->status()-1;
    }
    wordList->lose();
  }
}

} //namespace freeRecite end
