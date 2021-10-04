#include <Arduino.h>

#define pinADC 34
#define resolution 3.3/4095                   // Resolución del ADC de 12 bits (4096 combinaciones)
//#define resolution 3.3 / 2047                 // Resolución del ADC de 11 bits (2048 combinaciones)
//#define resolution 3.3 / 1023                 // Resolución del ADC de 10 bits (1024 combinaciones)
//#define resolution 3.3 / 511                  // Resolución del ADC de 9 bits (512 combinaciones)

int adcRead = 0;
int voltage = 0;

void setup() {
  Serial.begin(115200);

  analogReadResolution(12);                     // Resolución del ADC, acepta valores entre 9 y 12 (bits)
  //analogSetAttenuation(ADC_11db);             // Atenuación de TODOS los ADC, acepta las siguientes constantes => ADC_0db, ADC_2_5db, ADC_6db, ADC_11db
  analogSetPinAttenuation(pinADC, ADC_11db);    // Atenuación del pin designado del ADC, el resto quedarán en default (ADC_11db) o como se haya configurado previamente

  /*

    La atenuación de 0 db proporciona una escala completa de voltaje de 1.1V
    La atenuación de 2.5 db proporciona una escala completa de voltaje de 1.5V
    La atenuación de 6 db proporciona una escala completa de voltaje de 2.2V
    La atenuación de 11 db proporciona una escala completa de voltaje de 3.3V

    ==>  UTILIZANDO LA ATENUACIÓN DE 11 DB, EL VOLTAJE MÁXIMO ESTÁ LIMITTADO POR EL VOLTAJE DE ALIMENTACIÓN, NO POR UNA ESCALA <==
  
  */
}

void loop() {
  adcRead = analogRead(pinADC);

  Serial.print("Value: ");
  Serial.println(adcRead);
  Serial.print("Voltage: ");
  Serial.print(adcRead * resolution, 2);
  Serial.println('V');

  delay(500);
}