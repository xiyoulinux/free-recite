/**
 * FileName: Manager.h.
 * Used to define the class Manager which is used to handle the tasks.
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


#ifndef MANAGER_H
#define MANAGER_H

#include <list>
#include <map>
#include <vector>

#include "Task.h"

namespace freeRecite {

class Manager
{
public:
  //Read the information from freeRecite.mgr file.
  //If the loadfile can not be read, it returns false.
  bool load(const std::string &dir);

  //Save the information to freeRecite.mgr file.
  bool save();

  //Refresh the Manager information.
  bool refresh();

  //Test whether this task ID is available.
  bool hasTask(time_t taskID);

  //Get the task's name.
  const std::string &getTaskName(time_t taskID)const;

  //Get the amount of the words from a task.
  int getTaskAmount(time_t taskID) const;

  //Get the step of the task.
  int getTaskStep(time_t taskID) const;

  //Creat a new task with the words set.
  bool creatTask(const std::set<std::string> &words,
		 const char *taskName = 0);
  //Get the number of the tasks that should be reviewed. 
  int getActiveTaskNum() const;

  //Get the default directory.
  const std::string &getDir() const;

  /**
   * Get a point to the Task which is active.
   **/
  const std::vector<time_t> &getActiveTasks() const;

  // Call this method will add one to the task's words amount.
  void addWord(time_t taskID);

  // Call this method will subtract one from task's words amount.
  void removeWord(time_t taskID);

  /**
   * This method is used to test whether the mark you got can pass 
   * this test. If you pass it, the task will come to the next step,
   * else it won't. 
   * 
   * Return Value:
   *    true: If you can pass it.
   *    false: If you can't pass it.
   * 
   * After calling this method, itr will point to the tested Task 
   * object, see Task::test() for more information.
   **/
  bool test(time_t taskID,int mark);
private:
  /**
   * mgrDir is the directory where default freeRecite.mgr 
   * file is saved. This file name is set by default, and
   * anyone can NOT modify it at Run-time,including that
   * they can't creat a file with this name under the same
   * directory.
   **/
  std::string mgrDir;
  std::map<time_t,Task> allTasks;
  std::vector<time_t> activeID;

  static const std::string managerFile;
};

inline
const std::string &Manager::getDir() const {
  return mgrDir;
}

 










} //namaspace freeRecite end

#endif
