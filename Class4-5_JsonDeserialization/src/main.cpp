#include <Arduino.h>
#include <ArduinoJson.h>

StaticJsonDocument<256> jsonMessageDeserialized;
String jsonMessageSerialized = "{\"var1\":123,\"var2\":123.123,\"var3\":true,\"var4\":\"Hello there\",\"Nested\":{\"nestedVar1\":123.123,\"nestedVar2\":\"Some string\",\"nestedVar3\":false}}";

int     var1;
float   var2;
bool    var3;
String  var4;

float   nestedVar1;
String  nestedVar2;
bool    nestedVar3;

void setup() {
  Serial.begin(115200);
  delay(1750);

  DeserializationError error = deserializeJson(jsonMessageDeserialized, jsonMessageSerialized);
  Serial.print("Error: ");
  Serial.println(error.c_str());

  if (error == DeserializationError::Ok) {

    if (jsonMessageDeserialized.containsKey("var1")) {
      var1 = jsonMessageDeserialized["var1"].as<int>();
    }

    if (jsonMessageDeserialized.containsKey("var2")) {
      var2 = jsonMessageDeserialized["var2"].as<float>();
    }

    if (jsonMessageDeserialized.containsKey("var3")) {
      var3 = jsonMessageDeserialized["var3"].as<bool>();
    }

    if (jsonMessageDeserialized.containsKey("var4")) {
      var4 = jsonMessageDeserialized["var4"].as<String>();
    }

    if (jsonMessageDeserialized.containsKey("Nested")) {
      JsonObject nested = nested["Nested"].as<JsonObject>();

      if (nested.containsKey("nestedVar1")) {
        nestedVar1 = nested["nestedVar1"].as<float>();
      }

      if (jsonMessageDeserialized.containsKey("nestedVar2")) {
        nestedVar2 = nested["nestedVar2"].as<String>();
      }

      if (jsonMessageDeserialized.containsKey("nestedVar3")) {
        nestedVar3 = nested["nestedVar3"].as<bool>();
      }
    }
  }

  Serial.print("Var 1: ");
  Serial.println(var1);
  Serial.print("Var 2: ");
  Serial.println(var2);
  Serial.print("Var 3: ");
  Serial.println(var3);
  Serial.print("Var 4: ");
  Serial.println(var4);
  Serial.println();
  Serial.print("Nested var 1: ");
  Serial.println(nestedVar1);
  Serial.print("Nested var 2: ");
  Serial.println(nestedVar2);
  Serial.print("Nested var 3: ");
  Serial.println(nestedVar3);
}

void loop() {}