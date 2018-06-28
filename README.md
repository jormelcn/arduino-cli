# CLI
Libreria de arduino que implementa un interprete de comandos capaz de recibir parametros de multipes tipos de datos

# Descripcion
Arduino CLI permite implementar muy facilmente un interprete de comandos que analiza el texto recibido en un Stream, interpreta los datos recibidos y ejecuta el método indicado. Los metodos habilitados deben ser agregados inicialmente en la configuración.

# Uso
Para el correcto uso de esta libreria es necesario seguir 5 pasos:
1. Incluir la Libreria
2. Instanciar un objeto CLI
3. Definir los metodos que ejecutará el interprete
4. Agregar los metodos al interprete
5. Ejecutar el interprete

#Ejemplo de uso más simple

```C++
//Paso 1: Incluir Libreria
#include <CLI.h>

//Paso 2: Instanciar Objeto CLI
CLI Console(Serial);

//Paso 3: Definir metodo que ejecutara el iterprete
void miMetodo(){
  //....
}

void setup() {
  //Inicializar Serial
  Serial.begin(115200);
  //Paso 4: Agregar metodo al interprete
  Console.addMethod("miMetodo()", miMetodo);
}

void loop(){

}
```

Para ejecutar el metodo "miMethodo" bastará con enviar la lines "miMetodo()" Seguido por el fin de linea correspondiente.

## Installation
Simplemente desgarga este repositorio como .zip y agregalo a arduino como libreria.
