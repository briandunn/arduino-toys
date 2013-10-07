#include "pin.h"
#include "Arduino.h"

class Step {
  public:
    Step(void (*song)(), Pin* lamp) {
      this->song = song; this-> lamp = lamp;
    }

  Step* indicate() {
    song();
    for(int i = 1; i < 255; i++) {
      lamp->analogWrite(i);
      delay(1);
    }
    lamp->digitalWrite(LOW);
    return this;
  }

  private:
    void (*song) ();
    Pin* lamp;
};

class Sequence {
  public:
    Sequence(Step* red, Step* yellow) {
      this->red = red; this->yellow = yellow;
    }
    Step* next() {
      return (random(2) == 0 ? red : yellow)->indicate();
    }

  private:
    Step* red;
    Step* yellow;
};

Sequence* sequence;
Pin* piezo;

void redSong() {
  piezo->tone(220, 100);
}

void yellowSong() {
  piezo->tone(660, 100);
}

void setup() {
  piezo = new Pin(11, OUTPUT);
  Pin* redLamp    = new Pin(10, OUTPUT);
  Pin* yellowLamp = new Pin(9, OUTPUT);
  Step* red       = new Step(redSong, redLamp);
  Step* yellow    = new Step(yellowSong, yellowLamp);
  sequence        = new Sequence(red, yellow);
}

void loop() {
  sequence->next();
  delay(100);
}
