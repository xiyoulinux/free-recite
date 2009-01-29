#include "DictItem.h" 

namespace freeRecite {

//Initialize the const static members of DictItem.

const std::string DictItem::sW("[W]");
const std::string DictItem::sT("[T]");
const std::string DictItem::sM("[M]");
const int DictItem::offset(3); 


bool DictItem::refer(const std::string &str) {

  posW = str.find(DictItem::sW);
  posT = str.find(DictItem::sT);
  posM = str.find(DictItem::sM);

  if(posW == std::string::npos || posT == std::string::npos || posM == std::string::npos) {
    strItem = 0;
    return false;
  }

  strItem = &str;

  bufW = strItem->substr(posW+offset,posT-posW-offset);
  bufT = strItem->substr(posT+offset,posM-posT-offset);
  bufM = strItem->substr(posM+offset,strItem->size()-posM-offset);
  return true;
}

} // namespace freeRecite end
