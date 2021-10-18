/*    ==>  Dependency Declaration  <==    */

#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <Preferences.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

/*    ==>  Constant Declaration  <==    */

#define PORT 80
#define interruptionPin 23
#define ledPin 2
#define resetTime 5000
#define rebounceTime 200
#define connectionTimeout 7000
#define defaultAPSSID "My IoT Device"
#define defaultAPPWD "password"

/*    ==>  Function Declaration  <==    */

void initWiFi();
void initWiFiAP();
void initWiFiSTA(String SSID, String PWD);
void initServer();
void notFound(AsyncWebServerRequest *request);
void networksRequest(AsyncWebServerRequest *request);
void connectWiFi(AsyncWebServerRequest *request);
void IRAM_ATTR resetToDefault();
String scanNetworks();

/*    ==>  Global Variables  <==    */

volatile long previousInterruptionTime = 0;
long connectionTime;
String SSID="";
String PWD="";
Preferences preferences;
AsyncWebServer server(PORT);

/*    ==>  Project Setup  <==    */

void setup()
{
  Serial.begin(115200);
  delay(1750);
  SPIFFS.begin();
  pinMode(ledPin, OUTPUT);
  pinMode(interruptionPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptionPin), resetToDefault, FALLING);
  initWiFi();
}

void loop() {}

void initWiFi()
{
  preferences.begin(".env");      // readOnly default value = false
  SSID = preferences.getString("SSID", "");
  PWD = preferences.getString("PWD", "");
  preferences.end();
  SSID.equals("") | PWD.equals("") ? initWiFiAP() : initWiFiSTA(SSID, PWD);
}

/** TODO: Debug this function: Brownout detector triggers */

void initWiFiAP()
{
  WiFi.mode(WIFI_AP);
  Serial.print("Starting WiFi Acces Point");
  while (!WiFi.softAP(defaultAPSSID, defaultAPPWD))
  {
    Serial.print('.');
    delay(100);
  }
  Serial.println("\nAccess Point Succesfully Started\n\nPlease connect to\n");
  Serial.print("SSID: ");
  Serial.println(defaultAPSSID);
  Serial.print("PASS: ");
  Serial.println(defaultAPPWD);
  initServer();
  Serial.println("\nOnce you've connected to your IoT device enter the following IP in a web browser\n");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());
}

/**
 * Initializes WiFi connection with the providen credentials
 * @param String SSID
 * @param String PWD
 */

void initWiFiSTA(String SSID, String PWD)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID.c_str(), PWD.c_str());
  Serial.print("Connecting to ");
  Serial.print(SSID);

  connectionTime = millis() + connectionTimeout;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(100);
    if (millis() > connectionTime) break;
  }
  switch (WiFi.status())
  {
  case WL_CONNECTED:
    Serial.println("\n\nSuccesful Connection");
    Serial.print("Your Station IP is: ");
    Serial.println(WiFi.localIP());
    break;
  
  default:
    Serial.println("\nConnection Failed\n");
    initWiFiAP();
    break;
  }
}

void initServer()
{
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("pub/index.html");
  server.on("/search", HTTP_GET, networksRequest);
  server.on("/connect", HTTP_GET, connectWiFi);
  server.onNotFound(notFound);
  server.begin();
  Serial.println("\nServer Succesfully Started");
}

void notFound(AsyncWebServerRequest *request)
{
  request->send(SPIFFS, "/errors/notFound.html");
}

void networksRequest(AsyncWebServerRequest *request)
{
  Serial.println("Networks Request");
  request->send(200, "text/plain", scanNetworks().c_str());
}

void connectWiFi(AsyncWebServerRequest *request)
{
  if (request->hasParam("SSID")) SSID = request->arg("SSID");
  if (request->hasParam("PASS")) PWD = request->arg("PASS");
  SSID.trim();
  PWD.trim();
  Serial.print("SSID: ");
  Serial.println(SSID);
  Serial.print("PASS: ");
  Serial.println(PWD);
  preferences.begin(".env");
  preferences.putString("SSID", SSID);
  preferences.putString("PWD", PWD);
  preferences.end();
  request->send(SPIFFS, "/pub/index.html");
  delay(2000);
  Serial.end();
  ESP.restart();
}

/** TODO: Debug this function: CPU panics */

void IRAM_ATTR resetToDefault()
{
  if (millis() - previousInterruptionTime > resetTime)
  {
    digitalWrite(ledPin, true);
    Serial.println("\nResetting to Default Configurations...\n");
    preferences.begin(".env");
    preferences.clear();
    preferences.end();
    digitalWrite(ledPin, false);
    delay(300);
    digitalWrite(ledPin, true);
    Serial.println("Resetting Complete.\nRestarting your ESP32...\n");
    delay(2000);
    digitalWrite(ledPin, false);
    previousInterruptionTime = millis();
    Serial.end();
    ESP.restart();
  } else if (millis() - previousInterruptionTime > rebounceTime)
  {
    Serial.println("\nMaintain the Reset button pressed for at least 5 seconds to reset to Default Configurations\n");
    previousInterruptionTime = millis();
  }
}

String scanNetworks()
{
  StaticJsonDocument<1024> networksData;
  String networksDataSerialized = "";

  WiFi.scanNetworks(true, false, true, 100);
  Serial.print("Scanning for networks");
  while (WiFi.scanComplete() < 0) {
    Serial.print('.');
    delay(100);
  };
  Serial.println('\n');

  int networkQuantity = WiFi.scanComplete();
  Serial.print(networkQuantity);
  Serial.println(" networks found\n");
  
  if (networkQuantity > 0) {
    JsonArray networks = networksData.createNestedArray("Networks");

    for (int i = 0; i < networkQuantity; i++) {
      String ssid = WiFi.SSID(i);
      int rssi = WiFi.RSSI(i);
      int encryptionType = WiFi.encryptionType(i);
      String networkType = encryptionType == WIFI_AUTH_OPEN ? "Open Network" : "Closed Network";
      networks[i]["SSID"] = ssid;
      networks[i]["RSSI"] = rssi;
      networks[i]["networkType"] = networkType;
    }
    Serial.println('\n');

    serializeJson(networksData, networksDataSerialized);
  }
  return networksDataSerialized;
}