#include <Arduino.h>
#include <WiFi.h>                 // Librería para configurar el esp32 como Access Point en este caso
#include <SPIFFS.h>               // Librería para almacenar archivos dentro del sistema de archivos en la flash del esp32
#include <ESPAsyncWebServer.h>    // Librería para configurar el esp32 como servidor web

#define SERVER_PORT 80

AsyncWebServer server(SERVER_PORT);    // Creación de nuestro servidor en el puerto 80

void initWiFiAP();

void setup() {
  Serial.begin(115200);
  delay(1750);
  SPIFFS.begin();   // Inicialización del sistema de archivos
  /*

    Debe crearse una carpeta llamada data en el mismo nivel jerárquico que nuestra carpeta src.
    En esta carpeta se alojarán todos los archivos que necesitemos
  
  */

  initWiFiAP();

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    /*
    
      serveStatic nos permite retornar un archivo a las solicitudes hechas a la dirección especificada.
      Consta de 3 argumentos obligatorios y uno opcional:

        -> Ruta a la que el cliente hace la solicitud
        -> Sistema de archivos
        -> Ruta dentro del sistema de archivos en la que se encuentra el documento que se va a mostrar
        -> Control de cache
      
      El método setDefaultFile() define el archivo a retornar dentro de la ruta especificada
    
    */

  server.onNotFound([](AsyncWebServerRequest *request) {    // Callback function
    request->send(404, "text/plain", "404, Not Found");
  });

  server.begin();       // Inicia  nuestro servidor

  Serial.print("Server started on port: ");
  Serial.println(SERVER_PORT);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());    // imprime la dirección IP del AP
}

void loop() {}

void initWiFiAP() {
  WiFi.mode(WIFI_AP);                               // Configuración del modo de WiFi del ESP32
  while (!WiFi.softAP("eirESP32", "password")) {    // Generación de la red con el nombre y contraseña asignados.
    /* 

      softAP retorna un booleano dependiente de la creación de la red del AP. (true == red creada exitosamente).

      El segundo argumento es opcional, si se omite será una red abierta

    */
    Serial.print('.');
    delay(100);
  }
}