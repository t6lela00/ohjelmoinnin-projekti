
#include "LedControl.h"
#include "NonBlockingRtttl.h"

#include "input.h"
#include "Pong.h"

// nimetty piirin mukaan
namespace MAX7219 {
const int clockPin = 7;
const int loadPin = 8;
const int dataPin = 9;
const int LKM = 2;
}

namespace _74HC165 {
const int loadPin         = 3;
const int readPin         = 2;
const int clockPin        = 4;
const int clockEnablePin  = 5;
}

const int piezoPin = 6;

// instansoidaan ledcontrol kahdelle led-näytölle.
LedControl lc(MAX7219::dataPin, MAX7219::clockPin, MAX7219::loadPin, MAX7219::LKM);

void setup() {
  Serial.begin(9600);
  // alustetaan led-näytöt
  for(int i = 0; i < MAX7219::LKM; i++) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 1);
    lc.clearDisplay(i);
  }
      
  //alustetaan peliohjain
  pinMode(_74HC165::readPin,              INPUT);
  pinMode(_74HC165::clockPin,             OUTPUT);
  pinMode(_74HC165::loadPin,              OUTPUT);
  pinMode(_74HC165::clockEnablePin,       OUTPUT);
  digitalWrite(_74HC165::loadPin,         HIGH);
  digitalWrite(_74HC165::clockPin,        LOW);
  digitalWrite(_74HC165::clockEnablePin,  HIGH);
}

void loop() {
  Pong::aja(&lc); //pong.ino
}

