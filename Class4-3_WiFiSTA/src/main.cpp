#include <Arduino.h>
#include <WiFi.h>

#define SSID "insert your SSID"
#define PWD "insert your password"

void initWiFi();

void setup() {
  Serial.begin(115200);
  delay(1750);
  initWiFi();
}

void loop() {}

void initWiFi() {
  WiFi.mode(WIFI_STA);            // WiFi en modo Estación
  //WiFi.mode(WIFI_AP);           // WiFi en modo Punto de Acceso
  //WiFi.mode(WIFI_MODE_APSTA);   // WiFi en ambos modos

  WiFi.begin(SSID, PWD);          // Inicia la conexión WiFi con las credenciales mostradas previamente
  Serial.print("Connecting to: ");
  Serial.print(SSID);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(50);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println('\n');
    Serial.println("Succesful connection");
  }

  Serial.print("\nYour IP Address is: ");
  Serial.println(WiFi.localIP());
}