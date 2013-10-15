#include "pin.h"
#include "Arduino.h"
#include "interrupt_button.h"

enum Color { RED, YELLOW };

class Move {
  public:
    Move(Pin* lamp): lamp(lamp) {}
    Color color;

  Move* indicate() {
    song();
    for(int i = 1; i < 255; i++) {
      lamp->analogWrite(i);
      delay(1);
    }
    lamp->digitalWrite(LOW);
    return this;
  }

  protected:
    virtual void song() = 0;

  private:
    Pin* lamp;
};

class Red : public Move {
  public:
    Red(Pin* lamp, Pin* piezo) : Move(lamp), piezo(piezo) { color = RED; }
  private:
    Pin* piezo;
    void song() { piezo->tone(220, 100); }
};

class Yellow : public Move {
  public:
    Yellow(Pin* lamp, Pin* piezo) : Move(lamp), piezo(piezo) { color = YELLOW; }
  private:
    Pin* piezo;
    void song() { piezo->tone(660, 100); }
};

class IndicatingButton : public InterruptButton {
  public:
    IndicatingButton(Move* move, int interrupt) : InterruptButton(interrupt), move(move) { }

  bool wasPressed() {
    bool was = InterruptButton::wasPressed();
    if(was) move->indicate();
    return was;
  }

  private:
    Move* move;
};

class Step {
  public:
    Step(Move* move) :next(NULL), move(move) {
      color = move->color;
    }
    Step* next;
    Color color;
    void indicate() { move->indicate(); }
  private:
    Move* move;
};

class Sequence {
  public:
    Sequence(Move* red, Move* yellow): played(false), first(NULL), red(red), yellow(yellow) { }
    bool played;

    Step* increment() {
      played = false;
      return append(next());
    }

    void play() {
      Step* current = first;
      if(current == NULL) return;
      do {
        current->indicate();
        current = current->next;
      } while(current != NULL);
      played = true;
    }

  private:

    Step* append(Move* move) {
      Step* step = new Step(move);
      if(first == NULL)
        first = step;
      else {
        Step* current = first;
        while(current->next != NULL)
          current = current->next;
        current->next = step;
      }
      return step;
    }

    Move* next() {
      return (random(2) == 0 ? red : yellow);
    }

    Step* first;
    Move* red;
    Move* yellow;
};

class Recorder {
  public:
    Recorder(Step* firstMove, Button* red, Button* yellow) :
      success(false),
      complete(false),
      sequence(firstMove),
      red(red),
      yellow(yellow) {}

    bool success;
    bool complete;

    void recordPress() {
      bool redPressed    = red->wasPressed();
      bool yellowPressed = yellow->wasPressed();
      if(!redPressed && !yellowPressed) {
        complete = false;
      } else {
        success  = (sequence->color == RED && red->wasPressed()) || (sequence->color == YELLOW && yellow->wasPressed());
        complete = sequence->next == NULL;
        sequence = sequence->next;
      }
    }

    Step* sequence;

  private:
    Button* red;
    Button* yellow;
};

Pin* piezo;
IndicatingButton* redButton;
IndicatingButton* yellowButton;
Sequence* sequence;
Recorder* recorder;
Pin* redLamp;
Pin* yellowLamp;

void gameOver() {
  // the frequencies of middle C, D, E, and F
  static bool played = false;
  if(played) return;
  const int C = 262;
  const int D = 294;
  const int E = 330;
  const int F = 348;
  const int notes[]     = {D, D, D, D, F, E, E, D, D, C, D};
  const int durations[] = {3, 2, 1, 3, 2, 1, 2, 1, 2, 1, 3};
  const int bpm = 200;
  for(int i = 0; i < 11; i++) {
    int ms = durations[i] * bpm;
    redLamp->analogWrite(notes[i]);
    yellowLamp->analogWrite(ms);
    piezo->tone(notes[i], ms * 0.75);
    delay(ms);
  }
  played = true;
}

void setup() {
  redLamp      = new Pin(10, OUTPUT);
  yellowLamp   = new Pin(9, OUTPUT);
  piezo        = new Pin(11, OUTPUT);
  Move* red    = new Red(redLamp, piezo);
  Move* yellow = new Yellow(yellowLamp, piezo);
  redButton    = new IndicatingButton(red,    0);
  yellowButton = new IndicatingButton(yellow, 1);
  sequence     = new Sequence(red, yellow);
  recorder     = new Recorder(sequence->increment(), redButton, yellowButton);
  sequence->play();
}

void loop() {
  recorder->recordPress();
  if(recorder->complete) {
    if(recorder->success) {
      recorder->sequence = sequence->increment();
      sequence->play();
    } else gameOver();
  }
}
