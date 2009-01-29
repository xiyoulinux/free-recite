/**
 * FileName: DictItem.h.
 * Used to define the class DictItem to declare the items from the dictionary.
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

#ifndef DICTITEM_HPP
#define DICTITEM_HPP

#include <string>

namespace freeRecite {

class DictItem
{
public:
  DictItem() : strItem(0)
  { /* Do Nothing! */ }
  ~DictItem()
  { /* Do Nothing! */ }

  bool refer(const std::string &str);

  const std::string & getW() const
  { return bufW; }
  const std::string & getT() const
  { return bufT; }
  const std::string & getM() const
  { return bufM; }

private:

  const std::string *strItem;

  const static std::string sW;
  const static std::string sT;
  const static std::string sM;
  const static int offset; 
  
  size_t posW;
  size_t posT;
  size_t posM;

  std::string bufW;
  std::string bufT;
  std::string bufM; 
};

} // namespace freeRecite end
#endif
