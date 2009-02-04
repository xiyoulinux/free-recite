#include <cstdlib>
#include <string>

#include "ConfigHolder.h"

namespace freeRecite {

ConfigHolder::ConfigHolder() {
  rootDirectory = getenv("HOME") + std::string("/.FreeRecite/");
}

void ConfigHolder::setRootDir(const char *dir) {
  rootDirectory = dir;
}

const  std::vector<unsigned> *ConfigHolder::r_list() {
  initPt.clear();
  initPt.push_back(0); //list[0] is just a placeholder.
  initPt.push_back(1);
  initPt.push_back(3);
  initPt.push_back(5);
  initPt.push_back(std::numeric_limits<unsigned>::max());
  return &initPt;
}

const std::vector<unsigned> *ConfigHolder::t_list() {
  initPt.clear();
  initPt.push_back(0); //list[0] is just a placeholder.
  initPt.push_back(std::numeric_limits<unsigned>::max());
  return &initPt;
}

//This is a global variable.
ConfigHolder configHolder;

}//End of namespaec freeRecite.
