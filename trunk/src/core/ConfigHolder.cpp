#include <cstdlib>

#include "ConfigHolder.h"

namespace freeRecite {

ConfigHolder::ConfigHolder() {
  rootDirectory = getenv("HOME");
  outExportPath = rootDirectory + "/";
  outKeystoneFile = rootDirectory + "/keystone.txt";
  rootDirectory.append("/.FreeRecite/");
  tasksDirectory = rootDirectory + "tasks/";
  managerFile = rootDirectory + "freeRecite.mgr";
  dictionaryFile = rootDirectory + "freeRecite.dict";
  keystoneFile = rootDirectory + "keystone.txt";
}

void ConfigHolder::setRootDir(const char *dir) {
  rootDirectory = dir;
  setTasksDir( (rootDirectory + "/tasks/").c_str() );
  setMgrFile( (rootDirectory + "/freeRecite.mgr").c_str() );
  setDictFile( (rootDirectory + "/freeRecite.dict").c_str() );
  setKeyFile((rootDirectory + "/keystone.txt").c_str() );
}

void ConfigHolder::setTasksDir(const char *dir) {
  tasksDirectory = dir;
}

void ConfigHolder::setMgrFile(const char *filePath) {
  managerFile = filePath;
}

void ConfigHolder::setDictFile(const char *filePath) {
  dictionaryFile = filePath;
}

void ConfigHolder::setKeyFile(const char *filePath) {
  keystoneFile = filePath;
}

void ConfigHolder::setOutKeyFile(const char *filePath) {
  outKeystoneFile = filePath;
}

void ConfigHolder::setExportPath(const char *dir) {
  outExportPath = dir;
  outExportPath.append("/");
}

//This is a global variable.
ConfigHolder configHolder;

}//End of namespaec freeRecite.
