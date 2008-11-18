/**
 * FileName: ForgetCurve.h
 * Used to define the class ForgetCurve which is used to describe the
 * Forgetting Curve.
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

#ifndef FORGETCURVE_H
#define FORGETCURVE_H

#include <ctime>

namespace freeRecite {

class ForgetCurve;

extern ForgetCurve forgetCurve;

class ForgetCurve
{
 public:
  ForgetCurve();
  ~ForgetCurve()
  { /* Do Nothing! */ }

  time_t getReviewTime(int curOrder) const;
  bool pass(int curOrder,int curMark)const;

private:
  time_t interval[8];
  int    mark[8];
};

inline bool ForgetCurve::pass(int curOrder,int curMark) const{
  if(curMark >= mark[curOrder])
    return true;
  else 
    return false;
}

} //namespace freeRecite end

#endif
