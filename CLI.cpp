
#include "CLI.h"

CLI::CLI(Stream* _stream){
  stream = _stream;
  workStart = workSpace;
  methodList = MethodList();
  _commitReady = false;
}

/*Characters Verification Functions*/

boolean CLI::isSpace(char c){
  return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

boolean CLI::isValidTypeChar(char c){
  return (c >= 'a' && c <= 'z') || c == '*';
}

boolean CLI::isValidMethodChar(char c){
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

boolean CLI::isValidNumberChar(char c){
  return c >= '0' && c <= '9';
}

boolean CLI::isScapedChar(char c){
  return c == '\"' || c == '\\';
}


/*Find Elements Functions*/

byte CLI::findMethod(char c){
  if(_startFound){
    if(_endFound){
      if(isSpace(c)){
        return CLI_RESULT_NONE;
      }
      else if(c == '('){
         return CLI_RESULT_SUCCESS;
      }
      return CLI_RESULT_ERROR;
    }
    else {
      if(isSpace(c)){
        _endFound = true;
        work[workIndex] = '\0';
        return CLI_RESULT_NONE;
      }
      else if(c == '('){
        work[workIndex] = '\0';
        return CLI_RESULT_SUCCESS;
      }
      else if(isValidMethodChar(c)){
        work[workIndex] = c;
        workIndex++;
        return CLI_RESULT_NONE;
      }
      return CLI_RESULT_ERROR;
    }
  }
  else {
    if(isSpace(c)){
      return CLI_RESULT_NONE;
    }
    else if(!isValidNumberChar(c) && isValidMethodChar(c)){
      _startFound = true;
      work[workIndex] = c;
      workIndex++;
      return CLI_RESULT_NONE;
    }
    return CLI_RESULT_ERROR;
  }
}

byte CLI::findType(char c){
  if(_startFound){
    if(_endFound){
      if(isSpace(c)){
        return CLI_RESULT_NONE;
      }
      else if(c == ','){
        return CLI_RESULT_SUCCESS; 
      }
      else if(c == ')'){ 
        return CLI_RESULT_END; 
      }
      return CLI_RESULT_ERROR;
    }
    else {
      if(isSpace(c)){
        _endFound = true;
        work[workIndex] = '\0';
        return CLI_RESULT_NONE;
      }
      else if(c == ','){
        work[workIndex] = '\0';
        return CLI_RESULT_SUCCESS;
      }
      else if(c == ')'){
        work[workIndex] = '\0';
        return CLI_RESULT_END;
      }
      else if(isValidTypeChar(c)){
        work[workIndex] = c;
        workIndex++;
        return CLI_RESULT_NONE;
      }
      return CLI_RESULT_ERROR;
    }
  }
  else{
    if(isSpace(c)){
      return CLI_RESULT_NONE;
    }
    else if(c == ')'){
      work[0] =  '\0';
      return CLI_RESULT_END;
    }
    else if(isValidTypeChar(c)){
      _startFound = true;
      work[workIndex] = c;
      workIndex++;
      return CLI_RESULT_NONE;
    }
    return CLI_RESULT_ERROR;
  }
}

byte CLI::findString(char c){
  if(_startFound){
    if(_endFound){
      if(isSpace(c)){
        return CLI_RESULT_NONE;
      }
      else if(c == ','){
         return CLI_RESULT_SUCCESS;
      }
      else if(c == ')'){
         return CLI_RESULT_END;
      }
      return CLI_RESULT_ERROR;
    }
    else {
      if(_scapeFound){
        if(isScapedChar(c)){
          work[workIndex] = c;
          workIndex++;
          _scapeFound = false;
          return CLI_RESULT_NONE;
        }
        return CLI_RESULT_ERROR;
      }
      else if(c == '\\'){
        _scapeFound = true;
        return CLI_RESULT_NONE;
      }
      else if(c == '\"'){
        _endFound = true;
        _inString = false;
        work[workIndex] = '\0';
        return CLI_RESULT_NONE;
      }
      work[workIndex] = c;
      workIndex++;
      return CLI_RESULT_NONE;
    }
  }
  else {
    if(isSpace(c)){
      return CLI_RESULT_NONE;
    }
    else if(c == '\"'){
      _startFound = true;
      _inString = true;
      return CLI_RESULT_NONE;
    }
    return CLI_RESULT_ERROR;
  }
}

byte CLI::findNumber(char c){
  if(_startFound){
    if(_endFound){
      if(isSpace(c)){
        return CLI_RESULT_NONE;
      }
      else if(c == ','){
        return CLI_RESULT_SUCCESS; 
      }
      else if(c == ')'){ 
        return CLI_RESULT_END; 
      }
      return CLI_RESULT_ERROR;
    }
    else {
      if(isSpace(c)){
        _endFound = true;
        work[workIndex] = '\0';
        if(_dotFound && workIndex < 2){
          return CLI_RESULT_ERROR;  
        }
        return CLI_RESULT_NONE;
      }
      else if(c == ','){
        work[workIndex] = '\0';
        if(_dotFound && workIndex < 2){
          return CLI_RESULT_ERROR;  
        }
        return CLI_RESULT_SUCCESS;
      }
      else if(c == ')'){
        work[workIndex] = '\0';
        if(_dotFound && workIndex < 2){
          return CLI_RESULT_ERROR;  
        }
        return CLI_RESULT_END; 
      }
      else if(c == '.'){
        if(_dotFound)
          return CLI_RESULT_ERROR;    
        _dotFound = true;
        work[workIndex] = c;
        workIndex++;
        return CLI_RESULT_NONE;
      }
      else if(isValidNumberChar(c)){
        work[workIndex] = c;
        workIndex++;
        return CLI_RESULT_NONE;
      }
      return CLI_RESULT_ERROR;
    }
  }
  else{
    if(isSpace(c)){
      return CLI_RESULT_NONE;
    }
    else if(c == '.' || isValidNumberChar(c)){
      _startFound = true;
      work[workIndex] = c;
      workIndex++;
      return CLI_RESULT_NONE;
    }
    return CLI_RESULT_ERROR;
  }
}


/*Work status control functions*/

void CLI::resetFindProcess(){
  workIndex = 0;
  _startFound = 0;
  _endFound = 0;
  _scapeFound = 0;
  _dotFound = 0;
  _inString = 0;
}

void CLI::resetWorkProcess(){
  work = workStart;
  resetFindProcess();
}

void CLI::resetParseProcess(){
  method = 0;
  typeIndex = 0;
  if(sources)
    free(sources);
  workStatus = CLI_WORK_METHOD;
  resetWorkProcess();
}


/*Utility Functions*/

byte CLI::statusForType(byte type){
  if(type == METHOD_TYPE_BYTE)
    return CLI_WORK_BYTE;
  if(type == METHOD_TYPE_CHAR)
    return CLI_WORK_CHAR;
  if(type == METHOD_TYPE_INT)
    return CLI_WORK_INT;
  if(type == METHOD_TYPE_FLOAT)
    return CLI_WORK_FLOAT;
  if(type == METHOD_TYPE_CHAR_P)
    return CLI_WORK_CHAR_P;
  if(type == METHOD_TYPE_BOOLEAN)
    return CLI_WORK_BOOLEAN;
  return 0;
}

void CLI::setParameter(void* destiny){
  if(_commitReady && typeIndex < method->typesCount){
    void* source = sources[typeIndex];
    if(method->types[typeIndex] == METHOD_TYPE_BYTE)
      *((byte*)destiny) = *((byte*)source);
    if(method->types[typeIndex] == METHOD_TYPE_INT)
      *((int*)destiny) = *((int*)source);
    if(method->types[typeIndex] == METHOD_TYPE_FLOAT)
      *((float*)destiny) = *((float*)source);
    if(method->types[typeIndex] == METHOD_TYPE_CHAR)
      *((char*)destiny) = *((char*)source);
    if(method->types[typeIndex] == METHOD_TYPE_CHAR_P)
      *((char**)destiny) = (char*)source;
    if(++typeIndex >= method->typesCount)
      _commitReady = false;
  }
}

boolean CLI::isEqual(char* a, char* b){
  int i = 0;
  while(a[i]){
    if(a[i] != b[i])
      return false;
    i++;
  }
  if(b[i])
    return false;
  return true;
}

int CLI::methodHash(char* data){
  char s1 = 0;
  char s2 = 1;
  int i = 0;
  while(data[i]){
    s1 = (s1 + data[i]) % 250;
    s2 = (s2 + s1) % 250;
    i++;
  }
  return (s1 << 8) | s2;
}

byte CLI::typeCode(char* type){
  if(isEqual(type, "char"))
    return METHOD_TYPE_CHAR;
  if(isEqual(type, "char*"))
    return METHOD_TYPE_CHAR_P;
  if(isEqual(type, "byte"))
    return METHOD_TYPE_BYTE;
  if(isEqual(type, "int"))
    return METHOD_TYPE_INT;
  if(isEqual(type, "boolean"))
    return METHOD_TYPE_BOOLEAN;
  if(isEqual(type, "float"))
    return METHOD_TYPE_FLOAT;
  return 0;
}


/* Debug Functions */

void CLI::printMethods(){
  MethodList list = methodList;
  while(true){
    if(list.item)
      printMethod(list.item);
    if(list.next)
      list = *list.next;
    else 
      break;
  }
}

void CLI::printMethod(Method* m){
  Serial.print("hash: ");
  Serial.print(m->hash);
  Serial.print("; types: "); 
  for(int i = 0; i < m->typesCount; i++){
    if(m->types[i] == METHOD_TYPE_CHAR)
      Serial.print("char");
    if(m->types[i] == METHOD_TYPE_CHAR_P)
      Serial.print("char*");
    if(m->types[i] == METHOD_TYPE_INT)
      Serial.print("int");
    if(m->types[i] == METHOD_TYPE_FLOAT)
      Serial.print("float");
    if(m->types[i] == METHOD_TYPE_BYTE)
      Serial.print("byte");  
    if(m->types[i] == METHOD_TYPE_BOOLEAN)
      Serial.print("boolean");  
    Serial.print(" "); 
  }
  Serial.println();
}


/*User Functions*/

void CLI::parseInput(){
  while(stream->available()){
    char c = stream->read();
    if(c == _commit){
      if(!_inString && workStatus != CLI_WORK_COMMIT && workStatus != CLI_WORK_NEXT){
        resetParseProcess();
        continue;
      }
    }
    if(workStatus == CLI_WORK_METHOD){
      byte result = findMethod(c);
      if(result == CLI_RESULT_SUCCESS){
        if(method = methodList.find(methodHash(work))){
          typeIndex = 0;
          if(method->typesCount > 0){
            sources = malloc(method->typesCount * sizeof(void*));
            for(int i = 0; i < method->typesCount; i++)
              sources[i] = 0;
            workStatus = statusForType(method->types[0]);
          }
          else{
            workStatus = CLI_WORK_TYPE;
          }
          resetFindProcess();
        }
      }
      else if(result == CLI_RESULT_ERROR){
        workStatus = CLI_WORK_NEXT;
      }
    }
    else if(workStatus == CLI_WORK_INT || workStatus == CLI_WORK_CHAR_P){
      byte result; 
      
      if(workStatus == CLI_WORK_INT || workStatus == CLI_WORK_BYTE || workStatus == CLI_WORK_FLOAT)
        result = findNumber(c);
      else if(workStatus == CLI_WORK_CHAR_P)
        result = findString(c);
        
      if(result == CLI_RESULT_NONE)
        continue;
      if(
        (result == CLI_RESULT_SUCCESS && typeIndex >= method->typesCount - 1) ||
        (result == CLI_RESULT_END && typeIndex != method->typesCount - 1)
       ){
        workStatus = CLI_WORK_NEXT;
        continue;
      }
      
      sources[typeIndex++] = work;

      if(workStatus == CLI_WORK_INT){
        *((int*)work) = atoi(work);
        work = work + sizeof(int);  
      }
      else if(workStatus == CLI_WORK_CHAR_P){
        work = work + workIndex + 1;
      }
      
      if(result == CLI_RESULT_END)
        workStatus = CLI_WORK_COMMIT;
      else
        workStatus = statusForType(method->types[typeIndex]);
      resetFindProcess();
    }
    else if(workStatus == CLI_WORK_TYPE){
      byte result = findType(c);
      if(result == CLI_RESULT_END){
        if(work[0] == '\0'){
          resetFindProcess();
          workStatus = CLI_WORK_COMMIT;  
        }
        else{
          workStatus = CLI_WORK_NEXT;  
        }
      }
      else if(result == CLI_RESULT_SUCCESS){
        workStatus = CLI_WORK_NEXT;
      }
    }
    else if(workStatus == CLI_WORK_COMMIT){
      if(c == _commit){
        if(typeIndex == method->typesCount){
          typeIndex = 0;
          _commitReady = true;
          method->f();
          _commitReady = false;
          resetParseProcess();
        }
      }
      else if(!isSpace(c)){
        workStatus = CLI_WORK_NEXT;
      }
    }
    else if(workStatus == CLI_WORK_NEXT){
      if(c == _commit){
        resetParseProcess();
      }
    }
  }
}

boolean CLI::addMethod(char* prototype, void (* f)()){
  int i = 0;
  int hash;
  byte* types = workStart;
  byte typesCount = 0;

  resetWorkProcess();
  workStatus = CLI_WORK_METHOD;
  while(prototype[i]){
    char c = prototype[i];
    if(workStatus == CLI_WORK_METHOD){
      int result = findMethod(c); 
      if(result == CLI_RESULT_SUCCESS){
        hash = methodHash(work);
        workStatus = CLI_WORK_TYPE;
        resetWorkProcess();
      }
      else if(result == CLI_RESULT_ERROR){
        workStart = types;
        resetParseProcess();
        return false;
      }
    }
    else if(workStatus == CLI_WORK_TYPE){
      int result = findType(c);
      if(result == CLI_RESULT_SUCCESS){
        types[typesCount] = typeCode(work);
        workStart = workStart + 1;
        typesCount++;
        resetWorkProcess();
      }
      else if(result == CLI_RESULT_ERROR){
        workStart = types;
        resetParseProcess();
        return false;
      }
      else if(result == CLI_RESULT_END){
        if(work[0]){
          types[typesCount] = typeCode(work);
          workStart = workStart + 1;
          typesCount++;
        }
        resetWorkProcess();
        workStatus == CLI_WORK_COMMIT;
      }
    }
    else if(workStatus == CLI_WORK_COMMIT){
      if(!isSpace(c)){
        workStart = types;
        resetParseProcess();
        return false;
      }
    }
    i++;
  }
  Method* newMethod = new Method(hash, types, typesCount, f);
  methodList.add(newMethod);
  resetParseProcess();
  return true;
}

CLI CLI::operator >> (int & destiny){
  setParameter(&destiny);
  return *this; 
}

CLI CLI::operator >> (char* & destiny){
  setParameter(&destiny);
  return *this; 
}

