
#include <Arduino.h>

#define METHOD_TYPE_BYTE    1
#define METHOD_TYPE_INT     2
#define METHOD_TYPE_FLOAT   3
#define METHOD_TYPE_CHAR    4
#define METHOD_TYPE_CHAR_P  5
#define METHOD_TYPE_BOOLEAN 6

class Method {

  public:

    int hash;
    byte* types;
    byte typesCount;
    void (* f)();
  
    Method(int hash, byte* types, byte typesCount, void (* f)());
};

