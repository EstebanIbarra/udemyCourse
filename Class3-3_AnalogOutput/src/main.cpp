#include <Arduino.h>

/*

  El ejercicio creará una función senoidal que se mostrará por medio del DAC

*/

#define AMPLITUDE 127
#define FREQUENCY 50
#define Y_OFFSET 128
#define pinOUT 25

void setup() {}

void loop() {
  for (double degrees = 0; degrees < 360; degrees+= 0.0012){
    double radians = degrees * PI/180;
    double sinVoltage = AMPLITUDE * sin(2 * PI * FREQUENCY * radians) + Y_OFFSET;
    dacWrite(pinOUT, sinVoltage);                                                   // dacWrite es el método encargado de generar una señal analógica
  }
}