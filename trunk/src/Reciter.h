/**
 * FileName: Reciter.h.
 * Used to define the class Reciter which is used to test or recite the words.
 *
 * Copyright (C) 2008 Kermit Mei (中文名：梅延涛).
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation.
 *
 * Written by Kermit Mei <kermit.mei@gmail.com>
 *
 * Many of the ideas implemented here are from the author's experiment.
 * But the dictionary's format coincide with the other word recite software
 * to help the users get more available data. And the review times designed
 * by means of the theory named Forgetting Curve which dicoveried by the
 * German psychologist named Hermann Ebbinghaus(1850–1909).
 *
 **/ 

#ifndef RECITER_HPP
#define RECITER_HPP

#include <vector>
#include <string>
#include <cstdlib>
#include "WordList.h"

namespace freeRecite {

class Reciter
{
public:
  Reciter()
    :score(0),wordList(0)
  { /* Do Nothing Here! */ }
  ~Reciter()
  { /* Do Nothing Here! */ } 

  /**
   * Load the words from the file, success return true.
   * Whenever you want to use this class, you must call it first.
   **/
  bool loadWords(time_t taskID,const char *configDir);

  //The amount of the words in this task.
  unsigned capability() const;

  //The number of the unremembered (or untested) words.
  unsigned size() const;

  /**
   * Get the next word you should recite.
   * Before you get it, you must test whether it is valid.
   **/
  const std::string &getWord() const;

  //If return true, then the object is valid.
  bool isValid()const;

  //Do this test again with an random order.
  bool redo();

  //Add a word to the current task.
  bool addWord(const std::string &word);

  //Remove a word from the current task.
  bool removeWord(const std::string &word);

  //Get the number of the words which you answered right.
  unsigned getScore() const;

  /**
   * Test with the result. The argument 'result' is true when the word
   * which the user input is correct.
   **/
  void test(bool result);

protected:
  //Reload the words from the file.
  bool reload();

  //Make words be random.
  bool makeRandom();
  
  unsigned score;
  WordList *wordList;
  std::vector<std::string> words;
  std::string taskFileName;
};

class Random {
public:
  ptrdiff_t operator() (ptrdiff_t max) {
    double tmp = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
    return static_cast<ptrdiff_t>(tmp * max);
  } 
};

inline 
const std::string &Reciter::getWord() const {
  return words[wordList->getNext()];
}


inline
unsigned Reciter::capability() const {
    return words.size();
}

inline
unsigned Reciter::size() const{
  if(wordList == 0)
    return 0;
  if(wordList->isValid())
    return wordList->size();
  else
    return 0;
}

inline 
bool Reciter::isValid() const {
  if(wordList == 0)
    return false;
  else
    return wordList->isValid();
}

inline
unsigned Reciter::getScore() const {
  return score*100/words.size();
}

} //namespace freeRecite end
#endif
