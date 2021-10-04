            ===>    UART Y COMUNICACIÓN SERIAL    <===

La comunicación serial (UART) es un protocolo bastante común utilizado en la comunicación entre dispositivos.

La conexión serial entre dispositivos se logra siguiendo la siguiente conexión:

    * Dispositivo 1 - Tx    ->  Dispositivo 2 - Rx
    * Dispositivo 1 - Rx    ->  Dispositivo 2 - Tx
    * Dispositivo 1 - GND   ->  Dispositivo 2 - GND

Estas conexiones entre el ESP32 y la computadora ya están integradas, por lo que no es necesario hacerlas


/** TODO: Serial.print dentro de void setup() no funciona **/