/*    ==>  Dependency Declaration  <==    */

#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <Preferences.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <DNSServer.h>

/*    ==>  Global Constants  <==    */

#define PORT 80
#define DNS_PORT 53
#define interruptionPin 23
#define ledPin 2
#define resetTime 5000
#define overloadTime 10000
#define debounceTime 300
#define connectionTimeout 7000
#define defaultAPSSID "My IoT Device"
#define defaultAPPWD "password"
#define hostName "myiotdevice.com"

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
volatile bool resetFlag = false;
long connectionTime;
String SSID="";
String PWD="";
Preferences preferences;
AsyncWebServer server(PORT);
DNSServer dnsServer;

/*    ==>  Class Declaration  <==    */

class MyHandler : public AsyncWebHandler      // Extends AsyncWebHandler
{
  public:
    MyHandler();              // Constructor
    virtual ~MyHandler();     // Destructor
    /* The virtual specifier specifies that a non-static member function is virtual and supports dynamic dispatch
          Reference: https://en.cppreference.com/w/cpp/language/virtual */

    /**
     * Overrides parent's method
     * @param AsyncWebServerRequest *request
     * @return bool
     */
    bool canHandle(AsyncWebServerRequest *request);

    /**
     * Redirects to the Home Page
     * @param AsyncWebServerRequest *request
     */
    void handleRequest(AsyncWebServerRequest *request);
};

/*    ==>  Class Definition  <==    */

MyHandler::MyHandler() {};

MyHandler::~MyHandler() {};

bool MyHandler::canHandle(AsyncWebServerRequest *request) {return true;}

void MyHandler::handleRequest(AsyncWebServerRequest *request) {request->send(SPIFFS, "/pub/index.html");}

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
  if (resetFlag) clearPreferences();
  if (WiFi.status() != WL_CONNECTED) dnsServer.processNextRequest();      // Method to keep listening to all DNS requests while the WiFi is not connected
}

void initWiFi()
{
  preferences.begin(".env");
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
  initServer();
  Serial.println("\nAccess Point Succesfully Started\n\nPlease connect to\n");
  Serial.print("SSID: ");
  Serial.println(defaultAPSSID);
  Serial.print("PASS: ");
  Serial.println(defaultAPPWD);
  Serial.print("\nOnce you've connected to your IoT device go to ");
  Serial.print(hostName);
  Serial.println(" to setup your device");
}

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
  server.on("/connect", HTTP_POST, connectWiFi);
  server.onNotFound(notFound);

  dnsServer.start(DNS_PORT, hostName, WiFi.softAPIP());         // Redirects all request from the hostName to the declared IP
  server.addHandler(new MyHandler()).setFilter(ON_AP_FILTER);   // Handles all requests sent to the AP IP

  server.begin();
  Serial.println("\nServer Succesfully Started");
}

void notFound(AsyncWebServerRequest *request)
{
  request->send(SPIFFS, "/errors/notFound.html");
}

void networksRequest(AsyncWebServerRequest *request)
{
  request->send(200, "text/plain", scanNetworks().c_str());
}

void connectWiFi(AsyncWebServerRequest *request)
{
  if (request->hasParam("SSID", true)) SSID = request->arg("SSID");
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
      resetFlag = true;
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
  Serial.print("\n\nScanning for networks");
  while (WiFi.scanComplete() < 0) {
    Serial.print('.');
    delay(100);
  };
  Serial.println('\n');
  int networkQuantity = WiFi.scanComplete();
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