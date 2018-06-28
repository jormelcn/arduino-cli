#include "MethodList.h"

MethodList::MethodList(){
  item = 0;
  next = 0;
}

void MethodList::add(Method* method){
  if(item){
    if(!next)
      next = new MethodList();
    next->add(method);
  }
  else{
    item = method;
  }
}

Method* MethodList::find(int hash){
  if(item && item->hash == hash){
    return item;
  }
  if(next)
    return next->find(hash);
  return 0;
}

