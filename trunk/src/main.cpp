#include <string>
#include <cstdlib>
#include "Cui.h"

using namespace std;

int main(void)
{
  std::string dir(getenv("HOME"));
  dir.append("/.FreeRecite/");
  freeRecite::CUI cui(dir);
  cui.run();
  return 0;
}
