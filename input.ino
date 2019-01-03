
#include "Arduino.h"

#include "input.h"

int Input::luePelaajaSyote() {
  // bittijono, johon syote luetaan.
  int syote = 0;
  
  //käytetään loadia aktiivisena, jotta parallel input latautuu rekisteriin ja asetetaan clock enable aktiiviseksi.
  digitalWrite(_74HC165::clockEnablePin, HIGH);
  digitalWrite(_74HC165::loadPin, LOW);
  delayMicroseconds(PULSE_PERIOD);
  digitalWrite(_74HC165::loadPin, HIGH);
  digitalWrite(_74HC165::clockEnablePin, LOW);
  delayMicroseconds(PULSE_PERIOD);

  //luetaan tuloste bittijonoon sarjatulostuspinnista. Sarjapinniin ladataan seuraava bittiarvo käyttämällä kelloa ylhäällä.
  for(int i = 0; i < NUM_OF_DPINS; i++) {
    syote = syote << 1;
    syote |= digitalRead(_74HC165::readPin);
    digitalWrite(_74HC165::clockPin, HIGH);
    delayMicroseconds(PULSE_PERIOD);
    digitalWrite(_74HC165::clockPin, LOW);
    delayMicroseconds(PULSE_PERIOD);
  }

  //asetetaan clock enable epäaktiiviseksi.
  digitalWrite(_74HC165::clockEnablePin, HIGH);
  delayMicroseconds(1);

  return syote;
}

