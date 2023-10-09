#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <ArduinoBLE.h>
#include <fstream>
#include <stdlib.h>
// #include <json.hpp>
// using json = nlohmann::json;

// typedef unsigned char BYTE;
// using namespace nlohmann::literals;

int buttonPin = 2;
boolean ledSwitch;

BLEService LEDService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service
// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
// BLEByteCharacteristic LEDCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite);
// BLEStringCharacteristic LEDCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite, 512);
BLEStringCharacteristic LEDCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite, 512);
// array<array<float>> song_arr;
StaticJsonDocument<3072> doc;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy failed!");
  }
  // set advertised local name and service UUID:
  BLE.setLocalName("ARDUINO");
  BLE.setDeviceName("ARDUINO");
  BLE.setAdvertisedService(LEDService);
  // add the characteristic to the service
  LEDService.addCharacteristic(LEDCharacteristic);
  // add service
  BLE.addService(LEDService);
  // start advertising
  BLE.advertise();
  Serial.println("BLE LED Peripheral, waiting for connections....");
}
void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();
  String songJson = "";
  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    // while the central is still connected to peripheral:
    
    while (central.connected()) {
      if (LEDCharacteristic.written()) {
        Serial.println("char written, new val:: ");
        String value = LEDCharacteristic.value();
        songJson += value;
        Serial.println(value);
        Serial.println(songJson);
        // std::cout << s << std::endl;
      }

  }
  Serial.print(F("Disconnected from central: "));
  Serial.println(central.address());
  Serial.println(songJson);
  Serial.println("lasdlajsknjhjhjhjjhjhjhjhjhdl");
  DeserializationError error = deserializeJson(doc, songJson);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  Serial.println("lasdlajsdl");
  int tempo = doc["tempo"]; // 120
  int numEvents = doc["numEvents"]; // idk
  Serial.println("numEvents");
  Serial.println(numEvents);
  JsonArray notes = doc["notes"];
  Serial.println("notes.size()");
  Serial.println(notes.size());
  float song[numEvents][3];
  for (int i = 0; i < numEvents; i++) {
    float currEvent[3];
    for (int j = 0; j < 3; j++) {
      currEvent[j] = std::atof(notes[i][j]);
      Serial.print("curr num/note:: ");
      Serial.println(currEvent[j]);
    }
    std::memcpy(song[i], currEvent, 3);
  }
}
}