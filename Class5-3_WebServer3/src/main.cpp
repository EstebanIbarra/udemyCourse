#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>

#define SERVER_PORT 80
#define pinADC 34

AsyncWebServer server(SERVER_PORT);
String message = "ESP32 test message";

void initWiFiAP();
void notFound404(AsyncWebServerRequest *request);
void IRAM_ATTR sendAdcData(AsyncWebServerRequest *request);
void IRAM_ATTR sendMessage(AsyncWebServerRequest *request);

void setup() {
  Serial.begin(115200);
  delay(1750);
  SPIFFS.begin();
  initWiFiAP();

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  server.on("/adc", HTTP_GET, sendAdcData);
  server.on("/message", HTTP_GET, sendMessage);
  server.onNotFound(notFound404);

  server.begin();
  Serial.print("Server started on port: ");
  Serial.println(SERVER_PORT);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {}

void initWiFiAP() {
  WiFi.mode(WIFI_AP);
  while (!WiFi.softAP("eirESP32", "password")) {
    Serial.print('.');
    delay(100);
  }
}

void notFound404(AsyncWebServerRequest *request) {
  request->send(SPIFFS, "/errors/notFound404.html");
}

void IRAM_ATTR sendAdcData(AsyncWebServerRequest *request) {
  request->send(200, "text/plain", String(analogRead(pinADC)).c_str());
}

void IRAM_ATTR sendMessage(AsyncWebServerRequest *request) {
  request->send(200, "text/plain", message.c_str());
}