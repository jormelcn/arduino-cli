
#include "Method.h"

class MethodList {

  public:
  Method* item;
  MethodList* next;

  MethodList();
  void add(Method* newItem);
  Method* find(int hash);
};

