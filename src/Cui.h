#include <string>

#ifndef FREERECITE__CUI_H
#define FREERECITE__CUI_H

#include <vector>

namespace freeRecite {

class Manager;
class Dict;
class Task;
class Reciter;

class CUI
{
public:
  CUI(const std::string &dir);
  ~CUI();
  void run();

private:
  char getCmd();
  void creatNew();
  void testProcess(Reciter &tester);
  void test();
  void recite();
  bool modify(const char *word = 0);
  bool modify(const std::string &word);
  void showActive();
  void clear();
  void getLine(std::string &input);
  void help();

  Manager *manager;
  Dict *dict;
};

} //Namespace freeRecite end.

#endif
