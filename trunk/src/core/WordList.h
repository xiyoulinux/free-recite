/**
 * FileName: WordList.h
 * Used to define the class WordList which is used to creat a circular
 * reviewing system for the user, and help them recite something quickly.
 *
 * Copyright (C) 2008 Kermit Mei <kermit.mei@gmail.com>
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

#ifndef WORDLIST_H
#define WORDLIST_H

namespace freeRecite {

/**
 * To use this class you should put something in an Random Accessing
 * Container such as std::vector or an array.
 **/

class WordList
{
public:
  //Initialize WordList using the size of the container object.
  WordList(unsigned initSize = 0, const std::vector<unsigned> *initPt = 0);

  //Destroy WordList.
  ~WordList();
  
  //Return the size of word list, put it here to make it inline.
  unsigned size() const;

  //Return r_times;
  int times() const;

  //Detect whether it is valid.
  bool isValid() const;

  /**
   * Get the Next word's index.
   * Note: It's undefined to call this method on an unvalid object.
   *       So you must always call isValid() before using it.
   **/
  unsigned getNext() const;

  //Get the first word's status.
  int status() const;

  /**
   * If the input word is true, then you must call pass().
   * This method will updata the WordList to help you get 
   * the correct index of the next word, when you call 
   * getNext() next time.:P
   * If it return 0, then the word is tested first.
   **/
  void pass();

  /**
   * If the input word is false, then you must call lose().
   * The return value is the status of the current word.
   **/
  void lose();

  /**
   * After you add one word to the contain, you must call
   * this method to synchronize the the contain withe the list.
   * The argument 'size' is the size of the new contain.
   **/
  void add();

  /**
   * After you remove one word from the contain, you must call
   * this method to synchronize the the contain withe the list.
   * The argument 'size' is the size of the new contain.
   **/
  void remove();

private:

  //Move the first element one afterward, advance all the pointers
  void moveAfter(unsigned i);

  //The element is passed, remove it form the WordList;
  void pop();

  struct NodeWord
  {
    NodeWord(unsigned initValue)
      :index(initValue),status(0),next(0)
    { /* Do Nothing Here! */ }
    unsigned index;
    unsigned status;
    NodeWord *next;
  };

  unsigned __size;
  unsigned __maxIndex;
  const std::vector<unsigned> *posToStatus;
  int r_times;
  NodeWord *first;
  NodeWord *last;
};

inline
unsigned WordList::size() const {
  return __size;
}

inline
int WordList::times() const {
  return r_times;
}

inline
unsigned WordList::getNext() const {
  return first->index;
}

inline
int WordList::status() const {
  return first->status;
}

inline
bool WordList::isValid() const {
  return (__size > 0 && posToStatus != 0);
}

} //namespace freeRecite end.

#endif
