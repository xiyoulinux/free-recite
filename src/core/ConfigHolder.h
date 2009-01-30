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

  const std::string rootDir() const;
  const std::string tasksDir() const;
  const std::string mgrFile() const;
  const std::string dictFile() const;
  const std::string keyFile() const;
private:
  std::string rootDirectory;
};

inline
const std::string ConfigHolder::rootDir() const {
  return rootDirectory;
}

inline
const std::string ConfigHolder::tasksDir() const {
  return rootDirectory + "/tasks/";
}
 
inline
const std::string ConfigHolder::mgrFile() const {
  return rootDirectory + "/freeRecite.mgr";
}

inline
const std::string ConfigHolder::dictFile() const {
  return rootDirectory + "/freeRecite.dict";
}

inline
const std::string ConfigHolder::keyFile() const {
  return rootDirectory + "keystone.txt";
}

}//End of namespace freeRecite.

#endif

