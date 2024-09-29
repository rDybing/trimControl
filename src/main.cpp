// ****************************************** library imports **********************************************************

#include <Arduino.h>
#include <stdio.h>

// ****************************************** pin assignments **********************************************************

// pin assignments
const uint8_t pots = 3;
const uint8_t buttons = 4;
const uint8_t toggles = 2;
const uint8_t sr_clock = 10;
const uint8_t sr_latch = 11;
const uint8_t sr_data = 12;
const uint8_t potPins[pots] = {A0, A1, A2};
const uint8_t buttPins[buttons] = {4, 5, 6};
const uint8_t togglePins[toggles] = {7, 8};
const uint8_t rotaryPin = 9;
const uint8_t srPins[3] = {sr_clock, sr_latch, sr_data};

// ****************************************** struct definitions *******************************************************

struct stateT {
  uint8_t leds;
  int8_t pot[pots];
  uint8_t rotary;
  bool button[buttons];
  bool toggle[toggles];
};

// ****************************************** set up global variables **************************************************

stateT state;
bool test = true;

// ****************************************** function declarations ****************************************************

int8_t readPot(uint8_t pin);
bool readButton(uint8_t pin, bool oldValue);
uint8_t readRotary(uint8_t pin, uint8_t oldValue);
void updateLEDs(uint8_t leds);
void output(stateT st);

// ****************************************** set pins and initial state ***********************************************

void setup(){

  // set up potentiometers
  for (int i = 0; i < pots; i++){
    pinMode(potPins[i], INPUT);
  }
  // set up buttons
  for (int i = 0; i < buttons; i++){
    pinMode(buttPins[i], INPUT_PULLUP);
  }
  // set up toggle switches
  pinMode(togglePins[0], INPUT_PULLUP);
  pinMode(togglePins[1], INPUT_PULLUP);
  // set up ShiftRegister
  for (int i = 0; i < 3; i++){
    pinMode(srPins[i], OUTPUT);
  }

  if (test){
    Serial.begin(9600);
  } else {
    // spin up joystick functionality
  }
}

// ****************************************** main loop ****************************************************************

void loop(){
}

// ****************************************** function implementations *************************************************

int8_t readPot(uint8_t pin){
  // analogRead uses 10 bit number (0..1024) so divide result from
  // read with 4 before returning, then subtract 127 to go from 0..255 to -127..128
}

bool readButton(uint8_t pin, bool oldValue){
  // decide if button have been pressed
}

uint8_t readRotary(uint8_t pin, uint8_t oldValue){
  // get new value for rotary encoder
}

void updateLEDs(uint8_t leds){
  // push out to shift register
}

void output(stateT st){
  if (test){
    char str[30];
    // pots
    Serial.println("X:\tY:\tZ:\t");
    printf(str, "%04d\t%04d\t%04d", st.pot[0], st.pot[1], st.pot[2]);
    Serial.println(str);
    // buttons
    Serial.println("B0:\tB1:\tB2:\tB3:");
    printf(str, "%d\t%d\t%d\t%d", st.button[0], st.button[1], st.button[2], st.button[3]);
    Serial.println(str);
    // toggle switches
    Serial.println("T0:\tT1:");
    printf(str, "%d\t%d", st.toggle[0], st.toggle[1]);
    Serial.println(str);
    // rotary
    // leds
    Serial.println("LEDS:");
    Serial.println(st.leds, BIN);
  } else {
    // send as joystick
  }
}