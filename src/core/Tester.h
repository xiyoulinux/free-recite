/**
 * FileName: Tester.h
 * Used to define the class Tester which is used to test whether 
 * the words be remembered.
 *
 * Copyright (C) 2008 Kermit Mei<kermit.mei@gmail.com>.
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

#ifndef TESTER_H_
#define TESTER_H_

#include "Scanner.h"
#include <set>

namespace freeRecite {

class Tester : public Scanner
{
public:
  Tester()
  { /* Do Nothing Here! */ }
  ~Tester();
  
  unsigned getScore() const;

  void test(bool result);

private:
  std::set<std::string> errWords;
};


inline
unsigned Tester::getScore() const {
  return score*100/words.size();
}

} //End of namespace freeRecite.

#endif //TESTER_H_
