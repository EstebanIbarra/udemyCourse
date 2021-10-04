#include <Arduino.h>

#define pinPWM1 13
#define pinPWM2 14
#define CONFIG_CHANNEL_1 0
#define CONFIG_CHANNEL_2 1
#define FREQUENCY 1000
#define RESOLUTION 10

int dutyCycle1 = 100;
int dutyCycle2 = 900;

void setup() {
  pinMode(pinPWM1, OUTPUT);
  pinMode(pinPWM2, OUTPUT);

  /* Configuración de los canales */
  ledcSetup(CONFIG_CHANNEL_1, FREQUENCY, RESOLUTION);   // Cada canal puede tomar valores de 0 a 15
  ledcSetup(CONFIG_CHANNEL_2, FREQUENCY, RESOLUTION);   // La resolución puede tomar valores de 1 a 16

  /* Configuración de las terminales por las que saldrán los PWM */
  ledcAttachPin(pinPWM1, CONFIG_CHANNEL_1);
  ledcAttachPin(pinPWM1, CONFIG_CHANNEL_2);

  /* Asignamos el ciclo útil a cada canal */
  ledcWrite(CONFIG_CHANNEL_1, dutyCycle1);
  ledcWrite(CONFIG_CHANNEL_2, dutyCycle2);
}

void loop() {
  delay(50);
}