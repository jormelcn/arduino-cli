

#include "Method.h"

Method::Method(int _hash, byte* _types, byte _typesCount, void (* _f)()){
  hash = _hash;
  types = _types;
  typesCount = _typesCount;
  f = _f;
}

