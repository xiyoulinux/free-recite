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

//This is a global variable.
ConfigHolder configHolder;

}//End of namespaec freeRecite.
