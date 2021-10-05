#include <Arduino.h>
#include <BluetoothSerial.h>    // Librería que contiene los metodos necesarios para la comunicación Bluetooth

#define pinLED 2

int value;
BluetoothSerial serialBT;     // Instancia de BluetoothSerial asignada a la variable serialBT

void ledOnOff(int);

void setup() {
  Serial.begin(115200);
  pinMode(pinLED, OUTPUT);
  serialBT.begin("eirESP32");     // Inicialización de la comunicación serial por medio de Bluetooth
}

void loop() {
  while (serialBT.available()) {      // Revisa si el serial del Bluetooth contiene información en el buffer
    value = serialBT.read();          // Lee el valor del caracter dentro del buffer y registra su valor numérico
    Serial.print("Value: ");
    Serial.println(value);            // Imprime el valor numérico del caracter leído previamente en la terminal
    Serial.print("Character: ");
    Serial.write(value);              // Imprime el caracter que corresponde al valor numérico leído previamente en la terminal
    Serial.println();
    ledOnOff(value);
  }

  while (Serial.available()) {        // Revisa si el serial de la terminal contiene información en el buffer
    value = Serial.read();            // Lee el valor del caracter dentro del buffer y registra su valor numérico
    serialBT.print("Value: ");
    serialBT.println(value);          // Imprime el valor numérico del caracter leído previamente en el serial del Bluetooth
    serialBT.print("Character: ");
    serialBT.write(value);            // Imprime el caracter que corresponde al valor numérico leído previamente en la el serial del Bluetooth
    serialBT.println();
    ledOnOff(value);
  }

}

void ledOnOff(int ledState) {
  if (ledState == '1') {
    digitalWrite(pinLED, HIGH);
    Serial.println("LED ON\n");
  } else if (ledState == '0') {
    digitalWrite(pinLED, LOW);
    Serial.println("LED OFF\n");
  }
}