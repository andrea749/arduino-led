#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <ArduinoBLE.h>
#include <fstream>
#include <stdlib.h>
#include <FastLED.h>
#include <list>
#include <vector>

using namespace std;

#define LED_PIN     17
#define NUM_LEDS    60
#define BRIGHTNESS  3
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

int buttonPin = 2;

BLEService LEDService("19B10000-E8F2-537E-4F6C-D104768A1214"); 
BLEStringCharacteristic LEDCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify | BLEWrite, 512);
StaticJsonDocument<3072> doc;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT); //??
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP); //???
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
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    // while the central is still connected to peripheral:
    
    while (central.connected()) {
      if (LEDCharacteristic.written()) {
        String value = LEDCharacteristic.value();
        songJson += value;
      }
    }
  // }
  Serial.print(F("Disconnected from central: "));
  Serial.println(central.address());
  DeserializationError error = deserializeJson(doc, songJson);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  int tempo = doc["tempo"]; // 120
  JsonArray notes = doc["notes"];
  int numEvents = notes.size();
  float song[numEvents][3]; // 2d array representing whole song. numEvents # of subarrays and each subarray has 3 things (time, pitch, on/off)
  // convert everything to floats
  for (int i = 0; i < numEvents; i++) {
    for (int j = 0; j < 3; j++) {
      song[i][j] = std::atof(notes[i][j]);
    }
  }
  // }
  int note_window_start = 0; // 1
  int note_window_end = 1; //2
  while (note_window_end < numEvents-1) {
    float curr_note_time = song[note_window_start][0];
    // get window size of all notes at the same time stamp
    while (curr_note_time == song[note_window_end][0]) {
      note_window_end++;
    }
    // iterate through all notes of same time stamp and set LED
    for (int i=note_window_start; i<note_window_end; i++) { // i = 0, end = 1, start = 0
      int pitch = (int)song[i][1];
      if (pitch < 0) {
        pitch *= -1;
      }
      // Black == LED off
      leds[pitch] = (song[i][2] == 0.0) ? CRGB::Black : CRGB::Purple;
    }

    FastLED.show();
    // 1 tick every 2 seconds (1/8th note per second, approx 30bpm?) = magnitude of 2000
    // FastLED.delay(delay*2000);
    FastLED.delay(2000);
    // slide window down past processed lights. not sure if subscripting error might happen with moving window past end of song
    if (note_window_end < numEvents-1) {
      note_window_start = note_window_end;
      note_window_end = note_window_start + 1;
    } else {
      //last event should always be the last LED clearing, so just call .clear()
      FastLED.clear();
    }
    // one final clear just in case
    FastLED.clear();
  }
  // for debugging purposes
  leds[40] = CRGB::Blue;
  leds[35] = CRGB::Blue;
  leds[30] = CRGB::Blue;
  leds[25] = CRGB::Blue;
  leds[20] = CRGB::Blue;
}
  leds[40] = CRGB::Blue;
  leds[35] = CRGB::Blue;
  leds[30] = CRGB::Blue;
  leds[25] = CRGB::Blue;
  leds[20] = CRGB::Blue;
}
