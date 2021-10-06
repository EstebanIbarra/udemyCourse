#include <Arduino.h>
#include <ArduinoJson.h>

StaticJsonDocument<192> jsonMessage;    // Variable de tipo StaticJsonDocument con un tamaño de 192 bytes
String jsonMessageString;
/* https://arduinojson.org/v6/assistant   ->  Calculadora en línea para el tamaño ideal para los objettos de tipo StaticJsondocument */

int var1 = 123;
float var2 = 123.123;
bool var3 = true;
String var4 = "Hello there";

void setup() {
  Serial.begin(115200);
  delay(1750);

  /* Almacenamos las variables dentro del Json */
  jsonMessage["var1"].set(var1);    // .set() e = son equivalentes
  jsonMessage["var2"].set(var2);
  jsonMessage["var3"] = var3;
  jsonMessage["var4"] = var4;

  JsonObject nestedJsonMessage = jsonMessage.createNestedObject("Nested");  // Genera un objeto anidado dentro de nuestro json original

  nestedJsonMessage["nestedVar1"] = 123.123;
  nestedJsonMessage["nestedVar2"] = "Some string";
  nestedJsonMessage["nestedVar3"] = false;

  //serializeJson(jsonMessage, jsonMessageString);          // Método que serializa nuestro objeto json en una cadena de texto y lo asigna a la variable jsonMessageString
  serializeJsonPretty(jsonMessage, jsonMessageString);      // La diferencia entre serializeJson y serializeJsonPretty solo es el formato del mensaje, uno cuenta con saltos de línea e indentaciones mientras el otro no

  Serial.println(jsonMessageString);
}

void loop() {
  
}