#include <CLI.h>

CLI Console(Serial);

void test(){
  Serial.println("CLI Test");
}

void sum(float a, int b){
  Console >> a >> b;
  Serial.print("sum: ");
  Serial.println(a + b);
}

void echoMessage(char* m){
  Console >> m
  Serial.print("echo : ")
  Serial.println(m);
}

void setup() {
  Serial.begin(9600);
  Console.addMethod("test()", test);
  Console.addMethod("sum(float, int)", sum);
  Console.addMethod("echoMessage(char*)", echoMessage);
}

void loop() {
  Console.parse();
  
  //
}

