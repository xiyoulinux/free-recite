/**
 * FileName: WordList.h
 * Used to define the class WordList which is used to creat a circular
 * reviewing system for the user, and help them recite something quickly.
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
  WordList(unsigned initSize = 0);
  ~WordList();
  
  //Return the size of word list, put it here to make it inline.
  unsigned size() const;

  //Detect whether it is valid.
  bool isValid() const;

  /**
   * Get the Next word's index.
   * Note: It's undefined to call this method on an unvalid object.
   *       So you must always call isValid() before using it.
   **/
  unsigned getNext() const;

  /**
   * If the input word is true, then you must call pass().
   * This method will updata the WordList to help you get 
   * the correct index of the next word, when you call 
   * getNext() next time.:P
   * If it return 0, then the word is tested first.
   **/
  int pass();

  /**
   * If the input word is false, then you must call lose().
   * It will also help like the pass() do.
   **/
  void lose();

private:
  //Move the first element one afterward, advance all the pointers
  void moveToPos(unsigned i);

  //Move the first element to the last, advance all the pointers.
  void moveToLast();

  //The element is passed, remove it form the WordList;
  void pop();

  //Advance all pos to their next.
  void advancePos(unsigned status = 0);

  struct NodeWord
  {
    NodeWord(unsigned initValue)
      :index(initValue),status(0),next(0)
    { /* Do Nothing Here! */ }
    unsigned index;
    int status;
    NodeWord *next;
  };
  unsigned __size;
  NodeWord *first;
  NodeWord *last;

  NodeWord *pos[4]; //pos[0] used as a temporary variable.
};

inline
unsigned WordList::size() const {
  return __size;
}


inline
unsigned WordList::getNext() const {
  return first->index;
}

inline
bool WordList::isValid() const {
  return __size > 0;
}

} //namespace freeRecite end.

#endif
