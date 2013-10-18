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

enum Pitch {
  C  = 262,
  CS = 277,
  D  = 294,
  E  = 330,
  F  = 348,
  G  = 392,
  A  = 440,
  B  = 494
};

struct Note {
  Pitch pitch;
  float duration;
};


void playSong(Note notes[], int length) {
  const int bpm = 150;
  for(int i = 0; i < length; i++) {
    int ms = notes[i].duration * bpm;
    Pitch pitch = notes[i].pitch;
    redLamp->analogWrite(pitch);
    yellowLamp->analogWrite(ms);
    piezo->tone(pitch, ms * 0.75);
    delay(ms);
  }
  redLamp->digitalWrite(LOW);
  yellowLamp->digitalWrite(LOW);
}

void beatLevel() {
  Note notes[] = {{C,1},{C,1},{C,1},{G,3}};
  playSong(notes, 4);
}

void gameOver() {
  Note notes[] = {
    {D, 3}, {D, 2}, {D, 1}, {D, 3},
    {F, 2}, {E, 1}, {E, 2}, {D, 1}, {D, 2}, {CS, 1},
    {D, 3}
  };
  playSong(notes, 11);
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
