#include "pin.h"
#include "Arduino.h"
#include "poll_button.h"
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

class IndicatingButton : public PollButton {
  public:
    IndicatingButton(Move* move, int pin) : PollButton(new Pin(pin, INPUT)), move(move) { }

  bool wasPressed() {
    bool was = PollButton::wasPressed();
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

enum Note {
  C  = 262,
  CS = 277,
  D  = 294,
  E  = 330,
  F  = 348,
  G  = 392,
  A  = 440,
  B  = 494
};

void playSong(Note notes[], int durations[], int length) {
  const int bpm = 150;
  for(int i = 0; i < length; i++) {
    int ms = durations[i] * bpm;
    redLamp->analogWrite(notes[i]);
    yellowLamp->analogWrite(ms);
    piezo->tone(notes[i], ms * 0.75);
    delay(ms);
  }
  redLamp->digitalWrite(LOW);
  yellowLamp->digitalWrite(LOW);
}

void beatLevel() {
  Note notes[]    = {C, C, C, G};
  int durations[] = {1, 1, 1, 3};
  playSong(notes, durations, 4);
}

void gameOver() {
  Note notes[]    = {D, D, D, D, F, E, E, D, D, C, D};
  int durations[] = {3, 2, 1, 3, 2, 1, 2, 1, 2, 1, 3};
  playSong(notes, durations, 11);
}

Color getPress() {
  Color color = NONE;
  if(yellowButton->wasPressed()) {
    color = YELLOW;
    Serial.println("YELLOW");
  } else if(redButton->wasPressed()) {
    color = RED;
  Serial.println("RED");
  }
  return color;
}

Move* red;
Move* yellow;

void playSequence(Step* step) {
  Step* current = step;
  while(current != NULL) {
    Move* move;
    move = current->color == YELLOW ? yellow : red;
    move->indicate();
    current = current->next;
  }
}

void setup() {
  redLamp      = new Pin(10, OUTPUT);
  yellowLamp   = new Pin(9, OUTPUT);
  piezo        = new Pin(11, OUTPUT);
  red          = new Red(redLamp, piezo);
  yellow       = new Yellow(yellowLamp, piezo);
  redButton    = new IndicatingButton(red,    2);
  yellowButton = new IndicatingButton(yellow, 3);
  game         = new Game(Step::first());
  playSequence(game->first);
  Serial.begin(9600);
}


void loop() {
  switch(game->record(getPress())) {
    case GAME_OVER:
      gameOver();
      break;
    case BEAT_LEVEL:
      beatLevel();
      playSequence(game->first);
      break;
    case PLAYING:
      break;
  }
}
