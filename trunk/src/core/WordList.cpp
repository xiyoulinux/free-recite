#include "ConfigHolder.h"
#include "WordList.h"

namespace freeRecite {

WordList::WordList(unsigned initSize,
		   const std::vector<unsigned> *initPt)
  :__size(initSize),__maxIndex(initSize-1),posToStatus(initPt),
   r_times(0),first(0),last(0)
{
  //Initialize the wordList.
  if(__size > 0) {
    first = new NodeWord(0);
    last = first;
  }
  for(unsigned i = 1; i < __size; ++i) {
      last->next = new NodeWord(i);
      last = last->next;
  }
}

WordList::~WordList()
{
  NodeWord *tmp = 0;
  while(first != 0) {
    tmp = first;
    first = first->next;
    delete tmp;
    tmp = 0;
  }
}

void WordList::pass()
{
  //You should review it!
  if(first->status > 0 && first->status < posToStatus->size()) {
    ++first->status;
    moveAfter(posToStatus->at(first->status - 1));
  }
  else //You have remembered it! (first or last status)
    pop();
  --r_times;
} //end of function WordList::pass().

void WordList::lose()
{
  if(first->status == 0)
    r_times += posToStatus->size() - 1;
  else
    r_times += first->status - 1;

  first->status = 1;
}

void WordList::moveAfter(unsigned i) {
  if(first == last)
    return;
  NodeWord *desPos = first;
  NodeWord *tmp = first;

  while(i-- > 0 && desPos != last)
    desPos = desPos->next;

  first = first->next;
  tmp->next = desPos->next;
  desPos->next = tmp;

  if(desPos == last)
    last = last->next;
}

void WordList::pop() {
  NodeWord *tmp = first;
  first = first->next;
  delete tmp;
  --__size;
}

void WordList::add() {
  NodeWord *tmp = new NodeWord(++__maxIndex);
  last->next = tmp;
  last = last->next;
  ++__size;
}

void WordList::remove() {
  NodeWord *tmp = first;
  NodeWord *max = first;

  //Adjust the value of r_times.
  if(first->status != 0)
    r_times -= posToStatus->size() - first->status;
  //Find the node with a max index in the list.
  while(tmp != last){
    tmp = tmp->next;
    if(max->index < tmp->index)
      max = tmp;
  }

  /**
   * If the last word with the max index is already in the list, then
   * it will be moved into the position which first->index refer to, and
   * then pop_back the last position.
   * So when the maxIndex is already in the list, it's index shoud equal
   * to first->index.
   **/
  if( max->index == (__maxIndex--) )
    max->index = first->index;
  pop();
}


} //Namespace freeRecite end.

