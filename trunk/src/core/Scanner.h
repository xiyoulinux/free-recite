/**
 * FileName: Scanner.h.
 * Used to define the class Reciter which is used to test or recite the words.
 *
 * Copyright (C) 2008 Kermit Mei<kermit.mei@gmail.com>
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

#ifndef SCANNER_H_
#define SCANNER_H_

#include <vector>
#include <string>
#include <cstdlib>
#include "WordList.h"

namespace freeRecite {

//A functor to make the words save in random order.
class Random {
public:
  ptrdiff_t operator() (ptrdiff_t max) {
    double tmp = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
    return static_cast<ptrdiff_t>(tmp * max);
  } 
};

class Scanner
{
public:
  Scanner()
    :taskID(0),r_times(0),score(0),wordList(0)
  { /* Do Nothing Here! */ }
  virtual ~Scanner();

  /**
   * Load the words from the file, success return true.
   * Whenever you want to use this class, you must call it first.
   **/
  bool loadWords(time_t initID,bool Random);

  //Get a task's words file by it's ID.
  static std::string getTaskFileName(time_t taskID);

  //The ID of the corresponding task.
  time_t id() const;

  //The amount of the words in this task.
  unsigned capability() const;

  //Return the procent of the current task's progress.
  unsigned size() const;

  //Return the remaining times that must be scan.
  unsigned times() const;

  //Returns the score.
  virtual unsigned getScore() const;

  /**
   * Get the current word you should recite.
   * Before you get it, you must test whether it is valid.
   **/
  const std::string &getWord() const;

  //If return true, then the object is valid.
  bool isValid()const;

  //Add a word to the current task.
  bool add(const std::string &word);

  //Remove a word from the current task.
  bool remove(const std::string &word);

  /**
   * Test with the result. The argument 'result' is true 
   * when the word which the user input is correct.
   **/
  virtual void test(bool result) = 0;

protected:
  //Save all the words in vector to the task's file.
  bool save();

  //Mank wors' order be random.
  bool makeRandom();

  time_t taskID;
  int r_times;
  unsigned score;
  WordList *wordList;
  std::vector<std::string> words;
  std::string taskFileName;
};

inline
Scanner::~Scanner() {
  if(wordList != 0)
    delete wordList;
}

inline
time_t Scanner::id() const {
  return taskID;
}

inline 
const std::string &Scanner::getWord() const {
  return words[wordList->getNext()];
}

inline
unsigned Scanner::capability() const {
    return words.size();
}

inline
unsigned Scanner::size() const {
  return wordList->size();
}

inline
unsigned Scanner::times() const{
  return static_cast<unsigned>(words.size() + r_times);
}

inline 
bool Scanner::isValid() const {
  if(wordList == 0)
    return false;
  else
    return wordList->isValid();
}

inline
unsigned Scanner::getScore() const {
  return score;
}

} //namespace freeRecite end

#endif //SCANNER_H_
