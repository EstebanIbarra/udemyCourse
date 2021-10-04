#include <Arduino.h>

#define pinOUT 2

volatile bool toggle = true;
hw_timer_t *timer = NULL;     // Declara un apuntador de tipo hw_timer_t

void IRAM_ATTR onTimer();     // Declaración de la función de interrupción

void setup() {
  pinMode(pinOUT, OUTPUT);
  timer = timerBegin(0, 80, true);    // Inicialización del timer
  /*

    -> El primer argumento es la definición del timer que se va a utilizar, 
        al tener cuatro timeers en el ESP32 puede tener un valor de 0 - 3
    -> El segundo argumento es la definición del valor del prescaler,
        al ser de 16 bits puede tener un valor de 0 - 65536
    -> El tercer argumento es la definición de la dirección del contador,
        puede ser ascendente (true) o descendente (false)

  */
  timerAttachInterrupt(timer, &onTimer, true);    // Asignación de la función de interrupción al timer
  /*

    -> El primer argumento es la definición del apuntador del timer al que se va a asignar la función de interrupción
    -> El segundo argumento es la definición de la dirección en la memoria de la función de interrupción
    -> El tercer argumento ese la definición del tipo de interrupción a utilizar,
        puede ser rising (true) o falling (false)

  */
  timerAlarmWrite(timer, 1000000, true);    // Define en qué número del contador va a disparar la función de interrupción
  /*

    -> El primer argumento es la definición del apuntador del timer que va a ejecutar la función de interrupción
    -> El segundo argumento es la definición del valor al que tiene que llegar el contador para disparar la interrupción
    -> El tercer argumento define si el contador debe volver a 0 (true) o no (false)

  */
  timerAlarmEnable(timer);    // Habilita la(s) interrupción(es) en el contador designado
}

void loop() {
  digitalWrite(pinOUT, toggle);
}

void IRAM_ATTR onTimer() {    // Definición de la función de interrupción
  toggle ^= true;
}