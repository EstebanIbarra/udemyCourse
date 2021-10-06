#include <Arduino.h>

int value;

void setup() {
  Serial.begin(115200);
  delay(1750);

  /* Serial.print */
  Serial.print("--- Serial.print ---");
  Serial.print("--- Serial.print2 ---");

  /* Serial.println */
  Serial.println("\n");
  Serial.println("--- Serial.println ---\n");
  Serial.println(117);
  Serial.println(123.123);    // Por defecto trunca el número de decimales a 2
  Serial.println('E');
  Serial.println("String");

  /* Serial.println con segundo argumento */
  Serial.println("\n\n--- Serial.println con segundo argumento ---\n");
  Serial.println("Las siguientes líneas imprimen el número 117 en distintas bases:\nDEC, HEX, BIN y OCT");
  Serial.println(117, DEC);
  Serial.println(117, HEX);
  Serial.println(117, BIN);
  Serial.println(117, OCT);
  Serial.println("\nLas siguientes líneas imprimen el número 123.123 con distinto truncado de decimales");
  Serial.println(123.123, 0);
  Serial.println(123.123, 3);
}

void loop() {
  while (Serial.available()) {    // Serial.available retorna un bool si existen valores en el buffer
    /**
     * Comment/Uncomment the next code block to test Serial.readString
     **/
  /*
    value = Serial.read();        // Serial.read lee un byte del buffer y retorna su valor entero
    Serial.println(value);
    Serial.write(value);
    Serial.println();
  */
    Serial.println(Serial.readString());
  }
  delay(100);
}