/**
 * FileName: Dict.h.
 * Used to define the class Dict which can handle dictionary.
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

#ifndef DICT_HPP
#define DICT_HPP

#include <map>
#include <string>

#include "DictItem.h"

namespace freeRecite {

class Dict;
extern Dict dictionary;

class Dict
{
public:
  Dict()
  { /* Do nothing! */ }
  ~Dict()
  { /* Do nothing! */ }

  bool load();

  bool lookUp(const std::string &word);

  bool modify(const std::string &item);

  bool save();

  //Define the mothed here to make them be inline.
  const std::string &word() const {
    return dictItem.getW();
  }
  
  const std::string &phonetics() const;
  
  const std::string &translation()const { 
    return dictItem.getM();
  }

private:
  DictItem dictItem;
  std::map<std::string,std::string> dict;
};



} // namespace freeRecite end

#endif 
