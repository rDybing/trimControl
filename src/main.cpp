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
const uint8_t srClock = 14;
const uint8_t srLatch = 15;
const uint8_t srData = 10;
const uint8_t potPins[potsReal] = {A0, A1, A2};
const uint8_t buttPins[buttonsReal] = {3, 4, 5, 6};
const uint8_t togglePins[toggles] = {7, 8};
const uint8_t srPins[3] = {srClock, srLatch, srData};

// ****************************************** struct definitions *******************************************************

struct stateT {
  uint8_t leds;
  int pot[potsVirtual];
  bool button[buttonsVirtual];
  bool toggle[toggles];
  bool jitter;
};

struct timerT {
  unsigned long prev;
  unsigned long next;
  unsigned long delay;
  bool direction;
};

// ****************************************** set up global variables **************************************************

stateT state;
timerT timer;
bool test = true;

// ezButtons
ezButton button0(buttPins[0]);
ezButton button1(buttPins[1]);
ezButton button2(buttPins[2]);
ezButton button3(buttPins[3]);
ezButton toggle0(togglePins[0]);
ezButton toggle1(togglePins[1]);

// ****************************************** functions ****************************************************************

void initButtonTimers(){
  button0.loop();
  button1.loop();
  button2.loop();
  button3.loop();
  toggle0.loop();
  toggle1.loop();
}

int readPot(uint8_t pin){
  int val = analogRead(pin);
  val = map(val, 0, 1023, 0, 255);
  return val;
}

void readPots(){
  for (int i = 0; i < potsReal; i++){
    if (state.toggle[0] == false){
      int old = state.pot[i];
      state.pot[i] = readPot(potPins[i]);
      if ((state.pot[i] != old)  && test) {
        Serial.println(state.pot[i]);
      }
    }
    if (state.toggle[0] == true){
      int old = state.pot[i+3];
      state.pot[i+3] = readPot(potPins[i]);
      if ((state.pot[i+3] != old) && test) {
        Serial.println(state.pot[i]);
      }
    }
  }
}

void readButtons(){
  int bank = 0; 
  if (state.toggle[1]){
    bank = 4;
  } 
  if (button0.isPressed()){
    state.button[bank] = true;
    if (test){
      Serial.println("B0");
    }
  } else {
    state.button[bank] = false;
  }
  if (button1.isPressed()){
    state.button[bank + 1] = true;
    if (test){
      Serial.println("B1");
    }
  } else {
    state.button[bank + 1] = false;
  }
  if (button2.isPressed()){
    state.button[bank + 2] = true;
    if (test){
      Serial.println("B2");
    }
  } else {
    state.button[bank + 2] = false;
  }
  if (button3.isPressed()){
    state.button[bank + 3] = true;
    if (test){
      Serial.println("B3");
    }
  } else {
    state.button[bank + 3] = false;
  }
}

void readToggle(){
  if (toggle0.isPressed()){
    state.toggle[0] = true;
    if (test){
      Serial.print("T0");
    }
  }
  if (toggle1.isPressed()){
    state.toggle[1] = true;
    if (test){
      Serial.print("T1");
    }
  }
  if (toggle0.isReleased()){
    state.toggle[0] = false;
  }
  if (toggle1.isReleased()){
    state.toggle[1] = false;
  }
}

void updateLEDs(){
  for (int i; i < toggles; i++){
      bitWrite(state.leds, i * 2, state.toggle[i]);
      bitWrite(state.leds, (i * 2) + 1, !state.toggle[i]);
  }
  for (int i; i < buttonsReal; i++){
    if (state.button[i] || state.button[i + 4]){
      bitWrite(state.leds, i + buttonsReal, true);
    } else {
      bitWrite(state.leds, i + buttonsReal, false);
    }
  }
}

void pushShiftRegister(){
  digitalWrite(srPins[1], LOW);
  shiftOut(srPins[2], srPins[0], LSBFIRST, state.leds);
  digitalWrite(srPins[1], HIGH);
}

// if jitter is on, add/sub 1 from bank2 per second
// this to auto set gyro sights in IL2 after looking at map where it resets...
void jitter(){
  timer.next = millis();
  if (timer.next > (timer.prev + timer.delay)){
    if (test){
      Serial.print("JIT ");
    }
    timer.prev = timer.prev;
    if (timer.direction){
      if (test){
        Serial.println("up!");
      }
      state.pot[3]++;
      state.pot[4]++;
    }
    if (!timer.direction){
      if (test){
        Serial.print("dn!");
      }
      state.pot[3]--;
      state.pot[4]--;
    }
    timer.direction = !timer.direction; 
  }
  /* reset if device been running for almost 50 days, preventing overflow
  if (timer.prev > 4200000000){
    timer.prev = 0;
    timer.next = 0;
  }
  */
}

// ****************************************** set pins and initial state ***********************************************

void setup(){
  /* set up potentiometers
  for (int i = 0; i < potsReal; i++){
    pinMode(potPins[i], INPUT);
  }
  */
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
  timer.delay = 1000;
}

// ****************************************** main loop ****************************************************************

void loop(){
  initButtonTimers();
  readButtons();
  readToggle();
  readPots();
  if (state.jitter){
    jitter();
  }
  // read rotary encoder
  updateLEDs();
  pushShiftRegister();
}