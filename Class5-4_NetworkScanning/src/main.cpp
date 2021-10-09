#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>

String searchForNetworks();

void setup() {
  Serial.begin(115200);
  delay(750);
  Serial.print("Searching for Networks");
  String networksDataSerialized = searchForNetworks();
  Serial.print("Networks Data Serialized\n\n");
  Serial.print(networksDataSerialized);
}

void loop() {}

String searchForNetworks() {
  StaticJsonDocument<1024> networksData;
  String networksDataSerialized = "";

  WiFi.mode(WIFI_STA);
  /** 
   * @param bool Búsqueda asíncrona.              Default -> false
   * @param bool Mostrar redes ocultas.           Default -> false
   * @param bool Búsqueda pasiva.                 Default -> false
   * @param int  Milisegundos màximos por canal.  Default -> 300
   * @param int  Número de canal.                 Default -> 0
   * @return int Número de redes descubiertas
   */
  WiFi.scanNetworks(true, false, true, 100);

  while (WiFi.scanComplete() < 0) {   // Retorna -1 si no se encontraron redes, -2 si la búsqueda no ha iniciado, de lo contrario retorna el número de redes
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
      String ssid = WiFi.SSID(i);   // Retorna el nombre de la red encontrada
      int rssi = WiFi.RSSI(i);      // Retorna la intensidad de la red en dBm
      int encryptionType = WiFi.encryptionType(i);      // Retorna el tipo de encripción de la red en un índice numérico
      String networkType = encryptionType == WIFI_AUTH_OPEN ? "Open Network" : "Closed Network";

      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(ssid);
      Serial.print(" ");
      Serial.print(rssi);
      Serial.print("dBm ");
      Serial.println(networkType);

      networks[i]["SSID"] = ssid;
      networks[i]["RSSI"] = rssi;
      networks[i]["networkType"] = networkType;
    }
    Serial.println('\n');

    serializeJson(networksData, networksDataSerialized);

    return networksDataSerialized;

  } else {
    return "No networks have been found...";
  } 
}