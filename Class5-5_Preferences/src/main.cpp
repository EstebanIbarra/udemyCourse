#include <Arduino.h>
#include <Preferences.h>
#include <vector>

Preferences preferences;

int envUpdate(int, char *, int);

void setup() {
  int counter = 0;
  String keyString = "counter";
  char *key = new char[keyString.length() + 1];     // Crea un espacio en memoria para el apuntador de la llave
  strcpy(key, keyString.c_str());                   // Transforma el string en un apuntador de caracteres y lo asigna al apuntador de la llave

  Serial.begin(115200);
  delay(1000);

  counter = envUpdate(0, key, 0);

  Serial.print("\nCounter's value: ");
  Serial.print(counter);
  Serial.println('\n');
  counter++;

  counter <= 25 ? envUpdate(1, key, counter) : envUpdate(3, key, 0);

  delay(2000);
  Serial.end();
  ESP.restart();
}

void loop() {}

int envUpdate(int crud, char *key, int value) {
  int getValue = 0;
  /** 
   * Opens a file or creates it if it doesn't exist
   * @param String File name
   * @param bool File mode    true -> Read only   false -> Read and write
   */
  preferences.begin(".env", false);

  switch (crud) {
    case 0:
      /**
       * Gets data from a key-value pair, assigns default data if it doesn't exist
       * @param String Key
       * @param int Default value
       */
      getValue = preferences.getInt(key, value);
      break;
    case 1:
      /**
       * Puts data to a key-value pair
       * @param String Key
       * @param int Value
       */
      preferences.putInt(key, value);
      break;
    case 2:
      /** Removes specified key from the open preferences file
       * @param char Pointer of the key
       */
      preferences.remove(key);
      break;
    case 3:
      /** Removes all data from the open preferences file */
      preferences.clear();
      break;
    default:
      break;
  }

  /** Closes the file */
  preferences.end();
  return getValue;
}
