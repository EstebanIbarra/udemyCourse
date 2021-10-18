/*    ==>  Dependency Declaration  <==    */

#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <Preferences.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

/*    ==>  Global Constants  <==    */

#define PORT 80
#define interruptionPin 23
#define ledPin 2
#define resetTime 5000
#define overloadTime 10000
#define debounceTime 300
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
void restartSequence();
void clearPreferences();
void IRAM_ATTR resetToDefault();
String scanNetworks();

/*    ==>  Global Variables  <==    */

volatile unsigned long previousInterruptionTime = 0;
volatile unsigned long pressedTime = 0;
volatile unsigned long unpressedTime = 0;
volatile bool resetTrigger = false;
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
  pinMode(interruptionPin, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(interruptionPin), resetToDefault, CHANGE);
  initWiFi();
}

void loop() {
  if (resetTrigger)
  {
    clearPreferences();
  }
}

void initWiFi()
{
  preferences.begin(".env");      // readOnly default value = false
  SSID = preferences.getString("SSID", "");
  PWD = preferences.getString("PWD", "");
  preferences.end();
  SSID.equals("") | PWD.equals("") ? initWiFiAP() : initWiFiSTA(SSID, PWD);
}

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
  switch (WiFi.status())      // Switch case for different WiFi.status() responses
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
  server.on("/connect", HTTP_POST, connectWiFi);      // POST method routing
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
  if (request->hasParam("SSID", true)) SSID = request->arg("SSID");     // In case of POST, second argument in hasParam() must be provided
  if (request->hasParam("PASS", true)) PWD = request->arg("PASS");
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
  restartSequence();
}

void restartSequence()
{
  Serial.println("\nRestarting your ESP32...");
  delay(2000);
  Serial.end();
  ESP.restart();
}

void clearPreferences()
{
  Serial.println("\nReseting to Default Configurations...\n");
  preferences.begin(".env");
  preferences.clear();
  preferences.end();
  for (int i = 0; i <= 10; i++)
  {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
  Serial.println("Reseting Complete.");
  restartSequence();
}

void IRAM_ATTR resetToDefault()
{
  if (millis() - previousInterruptionTime > debounceTime)
  {
    int interruptionRead = digitalRead(interruptionPin);
    unsigned long totalPressedTime = 0;
    if (interruptionRead)
    {
      digitalWrite(ledPin, HIGH);
      pressedTime = millis();
    } else
    {
      digitalWrite(ledPin, LOW);
      unpressedTime = millis();
      totalPressedTime = unpressedTime - pressedTime;
    }
    if (totalPressedTime == 0){}
    else if (totalPressedTime < resetTime)
    {
      Serial.println("Maintain the Reset button pressed for at least 5 seconds to reset to Default Configurations\n");
    } else if (totalPressedTime <= overloadTime)
    {
      resetTrigger = true;
    } else
    {
      Serial.println("Error reading the interruption button, please try again...\n");
    }
    totalPressedTime = 0;
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
    serializeJson(networksData, networksDataSerialized);
  }
  return networksDataSerialized;
}