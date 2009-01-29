#include "WordList.h"

namespace freeRecite {

WordList::WordList(unsigned init__Size)
  :__size(init__Size),first(0),last(0)
{
  for(unsigned i = 0; i < __size; ++i) {
    if(i == 0) {
      first = new NodeWord(0);
      last = first;
      for(int i = 1; i < 4; ++i)
	pos[i] = first;
    }else {
      last->next = new NodeWord(i);
      last = last->next;
      
      if( i == 1 )
	pos[1] = pos[1]->next;
      if( i > 0 && i <= 3 )
	pos[2] = pos[2]->next;
      if( i > 0 && i <= 7)
	pos[3] = pos[3]->next;
    }
  }
}

WordList::~WordList()
{
  pos[0] = first;
  while(first != 0) {
    first = first->next;
    delete pos[0];
    pos[0] = 0;
  }
}

void WordList::pass()
{
  if( status() > 0 && status() < 4) //You should review it! status = 1,2,3
    moveToPos(status());
  else if(status() == 4) //Put the word to the last.
    moveToLast();
  else //You have remembered it! status = 0,5
    pop();
} //end of function WordList::pass().

void WordList::lose()
{
  first->status = 1;
}

void WordList::moveToPos(unsigned i) {
  if(pos[i] == last){
    moveToLast();
  }else {
    ++(first->status);
    //pos[0] is a tempory variable.
    pos[0] = pos[i]->next;
    pos[i]->next = first;
    first = first->next;
    pos[i]->next->next = pos[0];
    advancePos(i);
  }
}

void WordList::moveToLast() {
  ++(first->status);
  last->next = first;
  last = last->next;
  first = first->next;
  last->next = 0;
  advancePos();;
}

void WordList::pop() {
  pos[0] = first;
  if(first == last) {
    first = 0;
    last = 0;
    __size = 0;
    delete pos[0];
    for(int i = 0; i < 4; ++i)
      pos[i] = 0;
  }else {
    first = first->next;
    delete pos[0];
    --__size;
    advancePos();;
  }
}

void WordList::add(unsigned size) {
  NodeWord *tmp = new NodeWord(size - 1);
  last->next = tmp;
  last = last->next;
  ++__size;
}

void WordList::remove(unsigned size) {
  NodeWord *tmp = first;
  NodeWord *max = first;

  while(tmp != last){
    tmp = tmp->next;
    if(max->index < tmp->index)
      max = tmp;
  }

  if( max->index == (size - 1) ){
    max->index = first->index;
    pop();
  }else
    pop();
}

void WordList::advancePos(unsigned status) {
  switch(status) {
  case 0:  case 3:
    if(pos[3] != last)
      pos[3] = pos[3]->next;
  case 2:
    if(pos[2] != last)
      pos[2] = pos[2]->next;
  case 1:
    if(pos[1] != last)
      pos[1] = pos[1]->next;
  }
}

} //Namespace freeRecite end.

