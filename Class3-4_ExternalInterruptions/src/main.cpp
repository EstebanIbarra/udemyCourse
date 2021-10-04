#include <Arduino.h>

#define pinINTERRUPTION 23
#define pinOUT 2
#define rebounceTime 200

/*

  El calificador volatile le indica al compilador que el valor de esta variable puede cambiar en cualquier momento.
  TODAS LAS VARIABLES A UTILIZAR EN LAS INTERRUPCIONES DEBEN TENER EL CALIFICADOR VOLATILE
  https://www.tutorialspoint.com/what-is-volatile-keyword-in-cplusplus

*/

volatile bool toggle = true;
volatile unsigned long previousInterruptionTime  = 0;

/*  ==> FUNCTION DECLARATION <==  */
void IRAM_ATTR interruption();    // La palabra rerrseervada IRAM_ATTR se refiere a que la función se almacene en la memoria RAM en lugar de la flash, para acelerar la ejecución del progrrama

void setup() {
  Serial.begin(115200);
  pinMode(pinOUT, OUTPUT);
  pinMode(pinINTERRUPTION, INPUT_PULLUP);
  attachInterrupt(                            // Función necesaria para configurar una interrupción
    digitalPinToInterrupt(pinINTERRUPTION),   // Función que declara que la terminal designada en el argumento va a ser utilizada como interrupción
    interruption,                             // Función a ejecutar en caso de una interrupción
    FALLING                                   // Tipo de evento de interrupción
  );
}

void loop() {
  Serial.println("Message");
  delay(500);
}

/*  ==> FUNCTION DEFINITION <== */
void IRAM_ATTR interruption() {
  if (millis() - previousInterruptionTime > rebounceTime){    // millis() cuenta el tiempo desde que el ESP32 encendió
    digitalWrite(pinOUT, toggle);
    Serial.println("Interruption");
    toggle ^= true;     // ^ es un operador XOR binario: copia el bit de información si está presente en un operando pero no en ambos. https://www.tutorialspoint.com/cplusplus/cpp_operators.htm
    previousInterruptionTime = millis();
  }
}