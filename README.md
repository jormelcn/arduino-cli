# CLI
Librería de arduino que implementa un intérprete de comandos capaz de recibir parámetros de múltipes tipos de datos.

# Descripción
Arduino CLI permite implementar muy facilmente un intérprete de comandos que analiza el texto recibido en un Stream, interpreta los datos recibidos y ejecuta el método indicado.

# Uso
Para el correcto uso de esta libreria es necesario seguir 5 pasos:
1. Incluir la Librería
2. Instanciar un objeto CLI
3. Definir los métodos que ejecutará el intérprete
4. Agregar los métodos al interprete
5. Ejecutar el interprete

## Ejemplo de uso más simple

```C++
//Paso 1: Incluir Libreria
#include <CLI.h>

//Paso 2: Instanciar Objeto CLI
CLI Console(Serial);

//Paso 3: Definir metodos que ejecutara el iterprete
void miMetodo1(){
  //....
}

void miMetodo2(){
  //....
}

void setup() {
  //Inicializar Serial
  Serial.begin(115200);
  //Paso 4: Agregar metodo al interprete
  Console.addMethod("miMetodo1()", miMetodo1);
  Console.addMethod("miMetodo2()", miMetodo2);
}

void loop(){
  //Paso 5: Ejecutar el interprete
  Console.parse();
}
```

Para ejecutar el método "miMetodo1" bastará con enviar la línea " miMetodo1() " Seguido por el fin de línea correspondiente desde el monitor serial. De igual forma para el método "miMetodo2".

## Explicación

### Instanciación
Al instanciar un objeto CLI es necesario pasar como parámetro un Objeto tipo Stream, en este caso se usó el objeto Serial, por lo que se interpretará lo que se reciba a través del puerto Serial.
```C++
  CLI Console(Serial);
```

### Definir Métodos
El método más simple es un método que no recibe parámetros:
```C++
void miMetodo(){
  //....
}
```
Para Definir un método con parametros es necesario indicarle al CLI los parámetros que debe pasar al método:
```C++
void miMetodo2(int p1, char* p2){
  //Pasar parametros desde el intérprete
  Console >> p1 >> p2;
  //....
}
```
* Los métodos definidos para ejecutar desde el CLI pueden ser llamados tambien desde el código del programa sin ningun inconveniente.

### Agregar Métodos
Para agregar un método al interprete, es necesario indicarle el Prototipo del método y un puntero al metodo a ejecutar:
```C++
Console.addMethod("miMetodo()", miMetodo);
```

Para agregar un método con parametros de entrada:
```C++
  Console.addMethod("miMetodo(int, char*)", miMetodo);
```
El prototipo "miMetodo(int, char*)" indica que el método recibe dos parametros, el primero es un entero y el segundo una cadena de caracteres. 

* Cuando se envía una cadena de caracteres al intérprete, esta debe estar entre comillas, ejemplo: miMetodo(1300, "hola")

### Tipos de datos validos
1. byte
2. int
3. float
4. char*

* Los tipos de datos se indican en el prototipo y van separados por coma. char* debe ir así, sin espacios intermedios.

### Ejecutar Intérprete
Para que el intérprete funcione es necesarió ejecutar el metodo "parse" de forma periódica, la forma más simple de hacerlo es ponerlo dentro del método "loop":
```C++
void loop(){
  //Paso 5: Ejecutar el interprete
  Console.parse();
}
```

### Un Ejemplo más detallado
Imaginemos que queremos interpretar comandos de multiples puertos Seriales usando un Arduino Mega, el código seria el siguiente:

```C++
//Paso 1: Incluir Libreria
#include <CLI.h>

//Paso 2: Instanciar Objetos CLI
CLI cmd(Serial);
CLI cmd1(Serial1);

//Paso 3: Definir metodos que ejecutara el iterprete

//Se ejecutará solo desde cmd
void miMetodo1(int a, int b){
  cmd >> a >> b ;
  //....
}

//Se ejecutará desde cmd y desde cmd1
void miMetodo2(float f, char* m){
  cmd >> f >> m;
  cmd1 >> f >> m;
  //.... 
}

//Se ejecutará solo desde cmd1
void miMetodo3(byte b){
  cmd1 >> b;
  //....
}

void setup() {
  //Inicializar Serial
  Serial.begin(115200);
  Serial1.begin(9600);

  //Paso 4: Agregar metodos a los interpretes
  cmd.addMethod("miMetodo1(int, int)", miMetodo1);
  cmd.addMethod("miMetodo2(float , char* )", miMetodo2);

  cmd1.addMethod("miMetodo2(float , char* )", miMetodo2);
  cmd1.addMethod("miMetodo3(byte)", miMetodo3);
}

void loop(){
  //Paso 5: Ejecutar interpretes
  cmd1.parse();
  cmd.parse();

}
```

# Instalación
Simplemente desgarga este repositorio como .zip y agregalo a arduino como librería desde: Programa > Incluir Librería > Añadir Librería .Zip
