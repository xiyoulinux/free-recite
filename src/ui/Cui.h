#include <string>

#ifndef FREERECITE__CUI_H
#define FREERECITE__CUI_H

#include <vector>

namespace freeRecite {

class Scanner;

class CUI
{
public:
  CUI();
  ~CUI();
  void run(int argc, char *argv[]);

private:
  time_t atoid(const char *argv);
  void cleanStress();
  void clear();
  void createNew(const char *fileName);
  void exportFromFile(const char *fileName);
  void exportTask(time_t taskID);
  void exportStress();
  void help();
  void getLine(std::string &input);
  bool modify(const std::string &word);
  void recite(time_t taskID);
  void remove(time_t taskID);
  void showActive();
  void showResult(bool result);
  void test(time_t taskID);
};

} //Namespace freeRecite end.

#endif
