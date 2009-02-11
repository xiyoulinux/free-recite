#include <fstream>
#include <iostream>

#include "Dict.h"
#include "ConfigHolder.h"

namespace freeRecite {

Dict::~Dict() {
  if(ifsgdic != 0) 
    delete ifsgdic;
}

bool Dict::load() {
  std::string dictName = configHolder.dictFile().c_str();
  std::ifstream ifs(dictName.c_str());
  if(!ifs.is_open())
    return false;

  std::string lineStr;
  while(ifs.good()) {
    std::getline(ifs,lineStr);
    if( dictItem.refer(lineStr) )
      dict[dictItem.getW()] = lineStr;
  }

  ifsgdic = new std::ifstream("/usr/share/FreeRecite/freeRecite.dict");
  if(!ifsgdic->is_open())
    return false;

  return true;
}

bool Dict::lookUp(const std::string &word) {
  //Find in local dictionary.
  std::map<std::string,std::string>::iterator itr = dict.find(word);
  if( itr != dict.end() ) {
    dictItem.refer(itr->second);
    return true;
  }

  //Find in global dictionary.   
  return findInGlobl(word);
}

bool Dict::findInGlobl(const std::string &swatch) {
  // get length of file
  ifsgdic->seekg(0,std::ios::end);
  int length = ifsgdic->tellg();
  int before = 0;
  int after = length;
  int current = -1;
  std::string line;
  while(after-before>1) {
    ifsgdic->seekg((after+before)/2);
    ifsgdic->ignore(std::numeric_limits<int>::max(),'\n');
    current = ifsgdic->tellg();
    getline(*ifsgdic,line);
    if(!dictItem.refer(line))
       return false;
    if(swatch > dictItem.getW())
      before = (after+before)/2;
    else if(swatch < dictItem.getW())
      after = (after+before)/2;	
    else if( swatch == dictItem.getW() )
      return true;
  }
  if(before == 0) {
    ifsgdic->seekg(0);
    getline(*ifsgdic,line);
    if(swatch == dictItem.getW())
      return true;
  }
  return false;
}

bool Dict::modify(const std::string &item) {
  static DictItem itemAdd;
  
  if(!itemAdd.refer(item)){
    return false;
  }
  
  dict[itemAdd.getW()] = item;
  if(save())
    return true;
  else
    return false; 
}

bool Dict::save() {
  std::ofstream ofs(configHolder.dictFile().c_str());
  if(!ofs.is_open()) {
    return false;
  }
  std::map<std::string,std::string>::const_iterator itr = dict.begin();
  while(itr != dict.end()) {
    if(!ofs.good()){
      return false;
    }
    ofs << itr->second << std::endl;
    ++itr;
  }
  return true;
}

const std::string &Dict::phonetics() const {
  static std::string __phonetics;
  __phonetics = "";
  for(unsigned i = 0; i < dictItem.getT().size(); ++i) {
    switch(dictItem.getT().at(i)) {
    case '0':
      __phonetics += "θ";
      break;
    case '1':
      __phonetics +="ɑ";
      break;
    case '2':
      __phonetics += "ʌ";
      break;
    case '3':
      __phonetics += "ә";
      break;
    case '4':
      __phonetics +="є";
      break;
    case '5':
      __phonetics +="æ";
      break;
    case '6':
      __phonetics += "ɔ";
      break;
    case '7':
      __phonetics += "ʃ";
      break;
    case '8':
      __phonetics += "ð";
      break;
    case '9':
      __phonetics += "ŋ";
      break;
    case '=':	
      __phonetics += "ʒ";
      break;
    case ';':
      __phonetics += ",";
      break;
    default:
      __phonetics += dictItem.getT().at(i);
    }
  }
  return __phonetics;
}

//This is a global variable.
Dict dictionary;

} //namespace freeRecite end
