#include "pin.h"
#include "Arduino.h"
#include "interrupt_button.h"
#include "game.h"
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

Pin* piezo;
IndicatingButton* redButton;
IndicatingButton* yellowButton;
Pin* redLamp;
Pin* yellowLamp;
Game* game;

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

Color getPress() {
  Color color = NONE;
  if(yellowButton->wasPressed())
    color = YELLOW;
  else if(redButton->wasPressed())
    color = RED;
  return color;
}

void playSequence(Step* step) {
  Step* current = step;
  while(current != NULL) {
    /* step->indicate(); */
    current = current->next;
  }
}

void setup() {
  redLamp      = new Pin(10, OUTPUT);
  yellowLamp   = new Pin(9, OUTPUT);
  piezo        = new Pin(11, OUTPUT);
  Move* red    = new Red(redLamp, piezo);
  Move* yellow = new Yellow(yellowLamp, piezo);
  redButton    = new IndicatingButton(red,    0);
  yellowButton = new IndicatingButton(yellow, 1);
  game         = new Game(Step::first());
  playSequence(game->first);
}


void loop() {
  switch(game->record(getPress())) {
    case GAME_OVER:
      gameOver();
      break;
    case BEAT_LEVEL:
      playSequence(game->first);
      break;
    case PLAYING:
      break;
  }
}
