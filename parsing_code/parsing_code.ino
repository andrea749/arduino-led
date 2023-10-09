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
vector<vector<float>> song_vec;


void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
  // time, LED #, on/off
  vector<float> note0 {0.0, 2.0, 1.0};  
  vector<float> note1 {0.25, 2.0, 0.0};
  vector<float> note2 {0.25, 3.0, 1.0};
  vector<float> note3 {0.25, 4.0, 1.0};
  vector<float> note4 {0.5, 2.0, 1.0};
  vector<float> note5 {0.5, 3.0, 0.0};
  vector<float> note6 {0.75, 1.0, 1.0};
  vector<float> note7 {0.75, 4.0, 0.0};
  vector<float> note8 {1.0, 0.0, 1.0};
  vector<float> note9 {1.0, 1.0, 0.0};
  vector<float> note10 {1.0, 2.0, 0.0};
  vector<float> note11 {1.0, 3.0, 1.0};
  vector<float> note12 {1.25, -2.0, 1.0};
  vector<float> note13 {1.25, 0.0, 0.0};
  vector<float> note14 {1.25, 3.0, 0.0};
  vector<float> note15 {1.5, -2.0, 0.0};
  vector<float> note16 {1.5, 0.0, 1.0};
  vector<float> note17 {2.0, -3.0, 1.0};
  vector<float> note18 {2.0, -1.0, 1.0};
  vector<float> note19 {2.0, 0.0, 0.0};
  vector<float> note20 {2.0, 1.0, 1.0};
  vector<float> note21 {2.5, -3.0, 0.0};
  vector<float> note22 {2.5, -1.0, 0.0};

  song_vec = {note0, note1, note2, note3, note4, note5, note6, note7, note8, note9, note10, note11, note12, note13, note14, note15, note16, note17, note18, note19, note20, note21, note22};

}

void loop() {

  // add light at the beginning to set tempo

  int note_window_start = 0; // 1
  int note_window_end = 1; //2
  while (note_window_end < song_vec.size()-1) {
    float curr_note_time = song_vec[note_window_start][0];
    // get window size of all notes at the same time stamp
    while (curr_note_time == song_vec[note_window_end][0]) {
      note_window_end++;
    }
    // iterate through all notes of same time stamp and set LED
    for (int i=note_window_start; i<note_window_end; i++) { // i = 0, end = 1, start = 0
      vector<float> curr_note = song_vec[i]; // {0.0, 2.0, 1.0}
      int pitch = (int)curr_note[1];
      if (pitch < 0) {
        pitch *= -1;
      }
      // Black == LED off
      leds[pitch] = (curr_note[2] == 0.0) ? CRGB::Black : CRGB::Purple;
    }
    // set delay, subtract timestamp of current window from timestamp of next window
    // error: if last note is at the same time as prev note, we might not clear the LEDs at the same time. potentially doesn't matter
    // float delay = song_vec[note_window_end][0] - song_vec[note_window_end-1][0]; // replace w/start
    FastLED.show();
    // 1 tick every 2 seconds (1/8th note per second, approx 30bpm?) = magnitude of 2000
    // FastLED.delay(delay*2000);
    FastLED.delay(2000);
    // slide window down past processed lights. not sure if subscripting error might happen with moving window past end of song
    if (note_window_end < song_vec.size()-1) {
      // Serial.println("move window");
      note_window_start = note_window_end;
      note_window_end = note_window_start + 1;
    } else {
      //last event should always be the last LED clearing, so just call .clear()
      FastLED.clear();
    }
    // one final clear just in case
    FastLED.clear();
  }
  leds[40] = CRGB::Blue;
  leds[35] = CRGB::Blue;
  leds[30] = CRGB::Blue;
  leds[25] = CRGB::Blue;
  leds[20] = CRGB::Blue;
  // put your main code here, to run repeatedly:

}
