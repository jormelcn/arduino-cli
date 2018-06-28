
#ifndef _CLI_H_

#define _CLI_H_

#define CLI_RESULT_ERROR    0
#define CLI_RESULT_NONE     1
#define CLI_RESULT_SUCCESS  2
#define CLI_RESULT_END      3

#define CLI_WORK_METHOD   1
#define CLI_WORK_TYPE     2
#define CLI_WORK_COMMIT   3
#define CLI_WORK_BYTE     4
#define CLI_WORK_CHAR     5
#define CLI_WORK_INT      6
#define CLI_WORK_FLOAT    7
#define CLI_WORK_BOOLEAN  8
#define CLI_WORK_CHAR_P   9
#define CLI_WORK_NEXT     10

#include <Arduino.h>
#include "MethodList.h"

class CLI {

  private:

  static CLI* _default;
  static void (* _serialEvent)();
  
  byte workSpace[40];
  Stream* stream;
  char _commit = '\n';
  MethodList methodList;
  Method* method;
  void** parametersSources;

  byte typeIndex;
  void** sources;
  
  byte* workStart;
  byte* work;
  int workIndex;
  byte workStatus;

  boolean _commitReady;
  
  boolean _startFound;
  boolean _endFound;
  boolean _scapeFound;
  boolean _dotFound;
  boolean _inString;

  void init();

  boolean isSpace(char);
  boolean isValidTypeChar(char);
  boolean isValidMethodChar(char);
  boolean isValidNumberChar(char);
  boolean isScapedChar(char);

  boolean isValidType(char*);
  boolean isValidBoolean(char*);
  
  byte findMethod(char);
  byte findType(char);
  byte findString(char);
  byte findNumber(char);

  void resetFindProcess();
  void resetWorkProcess();
  void resetParseProcess();

  byte statusForType(byte type);
  boolean isEqual(char*, char*);
  int methodHash(char*);
  byte typeCode(char* type); 
  void setParameter(void* destiny);

  void printMethod(Method* m);
  
  public :

  static void parseDefaultCLI();
  static void onSerialEvent(void (* f)());
  
  CLI(Stream*);
  CLI(Stream &);
  void parse();
  boolean addMethod(char*, void (* call)());

  void printMethods();

  CLI operator >> (boolean &);
  CLI operator >> (byte &);
  CLI operator >> (int &);
  CLI operator >> (float &);
  CLI operator >> (char &);
  CLI operator >> (char* &);
 
};

/*Easy Functions*/

void serialEvent();

#endif
