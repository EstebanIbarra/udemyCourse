#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>

#define SERVER_PORT 80
#define pinLED 2

AsyncWebServer server(SERVER_PORT);

void initWiFiAP();
void notFound404(AsyncWebServerRequest *request);
void ledIO(AsyncWebServerRequest *request);

void setup() {
  Serial.begin(115200);
  delay(1750);
  SPIFFS.begin();
  initWiFiAP();
  pinMode(pinLED, OUTPUT);

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  server.on("/led", HTTP_GET, ledIO);     // Handler de la solicitud GET a /led
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
  request->send(SPIFFS, "/errors/notFound404.html");    // Envío de notFound404.html en caso de página no encontrada
}

void ledIO(AsyncWebServerRequest *request) {
  if (request->hasParam("led")) {
    Serial.print("Led: ");
    int ledResponse = atoi(request->arg("led").c_str());    // Casting de String -> int
    if (ledResponse) {
      Serial.println("On");
      digitalWrite(pinLED, HIGH);
    } else {
      Serial.println("Off");
      digitalWrite(pinLED, LOW);
    }
  }

  if (request->hasParam("text") && !request->arg("text").equals("")) {
    Serial.print("Text: ");
    Serial.println(request->arg("text"));
  }

  request->redirect("/");
}