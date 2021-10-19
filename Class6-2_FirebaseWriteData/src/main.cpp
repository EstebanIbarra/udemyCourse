/*    ==>  Dependency Declaration  <==    */

#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

/*    ==>  Global Constants  <==    */

#define SSID "yourSSID"
#define PASS "yourPassword"
#define DB_URL "yourFirebaseDatabaseUrl"
#define SECRET_KEY "YourSecretKey"

/*    ==>  Function Declaration  <==    */

void initWiFi();
void restartSequence();
void succesfulConnection();

/*    ==>  Global Variables  <==    */

FirebaseData firebaseData;

void setup() {
  Serial.begin(115200);
  delay(1750);
  initWiFi();
  Firebase.begin(DB_URL, SECRET_KEY);     // Firebase connection init
  Firebase.reconnectWiFi(true);           // Reconnects if lost WiFi connection
}

void loop() {
  for (int i = 0; i < 10; i++)
  {
    Firebase.set(firebaseData, "test", i + 1);      // Writes an entry in the DB under the node of "test" and value of i + 1, updates it if it exists
    delay(5000);
  }
}

void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);
  Serial.print("Connecting to ");
  Serial.print(SSID);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(50);
  }
  WiFi.status() == WL_CONNECTED ? succesfulConnection() : restartSequence();
}

void succesfulConnection()
{
  Serial.println("\nSuccesful Connection");
  Serial.print("Your IP Address is: ");
  Serial.println(WiFi.localIP());
}

void restartSequence()
{
  Serial.println("\nRestarting your ESP32...");
  delay(2000);
  Serial.end();
  ESP.restart();
}