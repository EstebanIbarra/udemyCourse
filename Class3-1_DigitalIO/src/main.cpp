#include <Arduino.h>

#define pinIn 18
#define pinOut 2

void setup() {
  /*  

      Utiliza la resistencia pull down integrada dentro del módulo esp32,
      en caso de usar una resistencia externa, colocar una en  el circuito
      y reemplazar INPUT_PULLDOWN por INPUT.

      Existe la definición INPUT_PULLUP y INPUT_PULLDOWN
  
  */
  pinMode(pinIn, INPUT_PULLDOWN);
  pinMode(pinOut, OUTPUT);
}

void loop() {
  digitalRead(pinIn) ? digitalWrite(pinOut, HIGH) : digitalWrite(pinOut, LOW);
}