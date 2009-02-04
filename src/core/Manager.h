/**
 * FileName: Manager.h.
 * Used to define the class Manager which is used to handle the tasks.
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


#ifndef MANAGER_H
#define MANAGER_H

#include <list>
#include <map>
#include <vector>
#include "ConfigHolder.h"
#include "Task.h"

namespace freeRecite {

class Manager;
extern Manager manager;

class Manager
{
public:

  Manager()
    :firstReviewTime(0)
  { /* Do Nothing Here! */ }

  //Read the information from freeRecite.mgr file.
  bool load();

  //Save the information to freeRecite.mgr file.
  bool save();

  //Refresh the Manager information.
  bool refresh();

  //Test whether this task ID is available.
  bool hasTask(time_t taskID);

  //Get the task's name.
  const std::string &getTaskName(time_t taskID)const;

  //Get the next reviewing time which is nearest.
  time_t getNextTime() const;

  //Get the conrespond task's next reviewing time.
  time_t getNextTime(time_t taskID) const;

  //Get the step of the task.
  int getTaskStep(time_t taskID) const;

  //Create a new task with the words set.
  bool createTask(const std::set<std::string> &words,
		  const char *taskName = 0,
		  unsigned maxLimit = 100000);
    
  //Remove a task which had been created.
  void removeTask(time_t taskID);

  //Get the number of the tasks that should be reviewed. 
  int getActiveTaskNum() const;

  //Get the active tasks' ID.
  const std::vector<time_t> &getActiveTasks() const;

  /**
   * This method is used to test whether the mark you got can pass 
   * this test. If you pass it, the task will come to the next step,
   * else it won't. 
   * 
   * Return Value:
   *   -1: If you can't pass it.
   *    0: If you pass this setp.
   *    1: If you complish this task.
   * 
   * After calling this method, itr will point to the tested Task 
   * object, see Task::test() for more information.
   **/
  int test(const time_t &taskID, const int &mark);
private:
  /**
   * mgrDir is the directory where default freeRecite.mgr 
   * file is saved. This file name is set by default, and
   * anyone can NOT modify it at Run-time,including that
   * they can't creat a file with this name under the same
   * directory.
   **/
  time_t firstReviewTime;
  std::map<time_t,Task> allTasks;
  std::vector<time_t> activeID;
  time_t maxTaskID;
};

inline
time_t Manager::getNextTime() const { 
  return firstReviewTime;
}

} //namaspace freeRecite end

#endif
