/**
 * FileName: Task.h
 * Used to define the class Task which is used to indicate the status about
 * the user's reciting process.
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

#ifndef TASK_H
#define TASK_H

#include <ctime>
#include <iostream>

namespace freeRecite {

class Task
{
public:
  Task(time_t initID = 0,
       const char *initName = 0);
  ~Task();
  
  friend std::istream & operator>>(std::istream &, Task &);
  friend std::ostream & operator<<(std::ostream &, const Task &);
  friend bool operator>(const Task&, const Task&);
  friend bool operator<(const Task&, const Task&);
  friend bool operator==(const Task&, const Task&);  

  time_t getID() const;
  time_t getReviewTime() const;
  int getStep() const;
  const std::string &getName() const;
  bool shouldReview() const;
  bool hasFinished() const;
  bool isAvailable() const;

  bool test(int mark);
private:
  time_t id;
  time_t reviewTime;
  int step;
  std::string name;
};

inline
Task::Task(time_t initID, const char *initName)
  :id(initID),reviewTime(0),step(0)
{
  if(initName != 0)
    name = initName;
}

inline 
time_t Task::getID() const {
  return id;
}

inline 
time_t Task::getReviewTime() const {
  return reviewTime;
}

inline
int Task::getStep() const { 
  return step;
}

inline
const std::string &Task::getName() const {
  return name;
}

inline 
bool Task::hasFinished() const {
  return step == 8;
}

inline
bool Task::isAvailable() const {
  return id == 0 ? false : true;
}

} //namespace freeRecite end


#endif
