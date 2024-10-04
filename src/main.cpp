// ****************************************** library imports **********************************************************

#include <Arduino.h>
#include <ezButton.h>
#include <stdio.h>

// ****************************************** pin assignments **********************************************************

// pin assignments
const uint8_t potsReal = 3;
const uint8_t potsVirtual = 6;
const uint8_t buttonsReal = 4;
const uint8_t buttonsVirtual = 8;
const uint8_t toggles = 2;
const uint8_t srClock = 13;
const uint8_t srLatch = 14;
const uint8_t srData = 15;
const uint8_t rotLeft = 10;
const uint8_t rotRight = 11;
const uint8_t rotButton = 12;
const uint8_t potPins[potsReal] = {A0, A1, A2};
const uint8_t buttPins[buttonsReal] = {4, 5, 6, 7};
const uint8_t togglePins[toggles] = {8, 9};
const uint8_t rotaryPins[3] = {rotLeft, rotRight, rotButton};
const uint8_t srPins[3] = {srClock, srLatch, srData};

// ****************************************** struct definitions *******************************************************

struct rotaryT {
  bool left;
  bool right;
  bool button;
};

struct stateT {
  uint8_t leds;
  int8_t pot[potsVirtual];
  rotaryT rotary;
  bool button[buttonsVirtual];
  bool toggle[toggles];
  bool jitter;
};

struct timerT {
  unsigned long prev;
  unsigned long next;
  bool direction;
};

// ****************************************** set up global variables **************************************************

stateT state;
timerT timer;
bool test = true;
int delay = 1000;

// ezButtons
ezButton button0(buttPins[0]);
ezButton button1(buttPins[1]);
ezButton button2(buttPins[2]);
ezButton button3(buttPins[3]);
ezButton toggle0(togglePins[0]);
ezButton toggle1(togglePins[1]);

// ****************************************** functions ****************************************************************

int8_t readPot(uint8_t pin){
  // analogRead uses 10 bit number (0..1024) so divide result from
  // read with 4 before returning, then subtract 127 to go from 0..255 to -127..128
  int in = analogRead(pin);
  int8_t out = (in / 4) - 127;
  return out;
}

void readButtons(){
  uint8_t bank = 0; 
  if (state.toggle[1]){
    bank = 4;
  } 
  if (button0.isPressed()){
    state.button[0 + bank] != state.button[0];
  }
  if (button1.isPressed()){
    state.button[1 + bank] != state.button[1];
  }
  if (button2.isPressed()){
    state.button[2 + bank] != state.button[2];
  }
  if (button3.isPressed()){
    state.button[3 + bank] != state.button[3];
  }
}

void readToggle(){
  if (toggle0.isPressed()){
    state.toggle[0] = true;
  }
  if (toggle1.isPressed()){
    state.toggle[1] = true;
  }
  if (toggle0.isReleased()){
    state.toggle[0] = false;
  }
  if (toggle1.isReleased()){
    state.toggle[1] = false;
  }
}

uint8_t readRotary(uint8_t pin){
  // get new value for rotary encoder
}

void pushShiftRegister(){
  // push out to shift register
  digitalWrite(srPins[1], LOW);
  shiftOut(srPins[2], srPins[0], LSBFIRST, state.leds);
  digitalWrite(srPins[1], HIGH);
}

// if jitter is on, add/sub 1 from bank2 per second
// this to auto set gyro sights in IL2 after looking at map where it resets...
void jitter(){
  timer.next = millis();
  if (timer.next > timer.prev + delay){
    timer.prev = timer.prev;
    if (timer.direction){
      state.pot[3]++;
      state.pot[4]++;
    }
    if (!timer.direction){
      state.pot[3]--;
      state.pot[4]--;
    }
    timer.direction = !timer.direction; 
  }
  // reset if device been running for almost 50 days, preventing overflow
  if (timer.prev > 4200000000){
    timer.prev = 0;
    timer.next = 0;
  }
}

void output(){
  if (test){
    char str[30];
    // pots bank 1
    Serial.println("1: X:\tY:\tZ:\t");
    printf(str, "%04d\t%04d\t%04d", state.pot[0], state.pot[1], state.pot[2]);
    Serial.println(str);
    // pots bank 2
    Serial.println("2: X:\tY:\tZ:\t");
    printf(str, "%04d\t%04d\t%04d", state.pot[3], state.pot[4], state.pot[5]);
    Serial.println(str);
    // buttons bank 1
    Serial.println("B0:\tB1:\tB2:\tB3:");
    printf(str, "%d\t%d\t%d\t%d", state.button[0], state.button[1], state.button[2], state.button[3]);
    Serial.println(str);
    // buttons bank 2
    Serial.println("B0:\tB1:\tB2:\tB3:");
    printf(str, "%d\t%d\t%d\t%d", state.button[4], state.button[5], state.button[6], state.button[7]);
    Serial.println(str);
    // toggle switches
    Serial.println("T0:\tT1:");
    printf(str, "%d\t%d", state.toggle[0], state.toggle[1]);
    Serial.println(str);
    // rotary
    // leds
    Serial.println("LEDS:");
    Serial.println(state.leds, BIN);
  } else {
    // send as joystick
  }
}

// ****************************************** set pins and initial state ***********************************************

void setup(){
  // set up potentiometers
  for (int i = 0; i < potsReal; i++){
    pinMode(potPins[i], INPUT);
  }
  // zero out all pots
  for (int i = 0; i < potsVirtual; i++){
    state.pot[i] = 0;
  }
  // set up buttons
  button0.setDebounceTime(50);
  button1.setDebounceTime(50);
  button2.setDebounceTime(50);
  button3.setDebounceTime(50);
  // set up toggle switches
  toggle0.setDebounceTime(50);
  toggle1.setDebounceTime(50);
  // set up rotary encoder
  for (int i = 0; i < 3; i++){
    pinMode(rotaryPins[i], INPUT_PULLUP);
  }  
  // set up ShiftRegister
  for (int i = 0; i < 3; i++){
    pinMode(srPins[i], OUTPUT);
  }

  if (test){
    Serial.begin(9600);
  } else {
    // spin up joystick functionality
  }
  // init timer
  state.jitter = true;
  timer.prev = millis();
  timer.direction = false;
}

// ****************************************** main loop ****************************************************************

void loop(){
  // init buttons and toggles timers
  button0.loop();
  button1.loop();
  button2.loop();
  button3.loop();
  toggle0.loop();
  toggle1.loop();
  // read buttons & toggles
  readButtons();
  readToggle();
  // read pots
  for (int i = 0; i < potsReal; i++){
    if (state.toggle[0] == false){
      state.pot[i] = readPot(potPins[i]);
    }
    if (state.toggle[0] == true){
      state.pot[i+3] = readPot(potPins[i]);
    }
  }
  if (state.jitter){
    jitter();
  }
  // read rotary encoder
  // update LEDs depending on state
  // push to shift register
}