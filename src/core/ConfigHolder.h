#ifndef CONFIGHOLDER_H
#define CONFIGHOLDER_H

#include <string>

namespace freeRecite {

class ConfigHolder;
extern ConfigHolder configHolder;

class ConfigHolder
{
public:
  ConfigHolder();
  void setRootDir(const char *dir);
  void setTasksDir(const char *dir);
  void setMgrFile(const char *filePath);
  void setDictFile(const char *filePath);
  void setKeyFile(const char *filePath);
  void setOutKeyFile(const char *filePath);  
  void setExportPath(const char *dir);

  const std::string &rootDir() const;
  const std::string &tasksDir() const;
  const std::string &mgrFile() const;
  const std::string &dictFile() const;
  const std::string &keyFile() const;
  const std::string &outKeyFile() const;
  const std::string &exportPath() const;
private:
  std::string rootDirectory;
  std::string tasksDirectory;
  std::string managerFile;
  std::string dictionaryFile;
  std::string keystoneFile;
  std::string outKeystoneFile;
  std::string outExportPath;
};

inline
const std::string &ConfigHolder::rootDir() const {
  return rootDirectory;
}

inline
const std::string &ConfigHolder::tasksDir() const {
  return tasksDirectory;
}
 
inline
const std::string &ConfigHolder::mgrFile() const {
  return managerFile;
}

inline
const std::string &ConfigHolder::dictFile() const {
  return dictionaryFile;
}

inline
const std::string &ConfigHolder::keyFile() const {
  return keystoneFile;
}

inline
const std::string &ConfigHolder::outKeyFile() const {
  return outKeystoneFile;
}

inline
const std::string &ConfigHolder::exportPath() const {
  return outExportPath;
}

}//End of namespace freeRecite.

#endif

