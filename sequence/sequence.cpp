#include "pin.h"
#include "Arduino.h"
#include "poll_button.h"
#include "animation.h"
#include "game.h"

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

class Move {
  public:
    Move(Pin* lamp): lamp(lamp) {}
    Color color;

  virtual Move* indicate(bool) = 0;
  protected:
    Pin* lamp;
};

class MoveAnimation : public Animation {
  Pin* lamp;
  Pin* piezo;
  int brightness;
  Pitch pitch;
  unsigned long lastLoop;
  unsigned long start;
  bool started;
  void frame(unsigned long elapsedSinceLoop, unsigned long elapsedSinceStart) {
    if(elapsedSinceStart == 0) piezo->tone(pitch);
    if(elapsedSinceStart < 200) {
      // fade to 255 in 200 ms
      lamp->analogWrite(elapsedSinceStart * 0.784);
      brightness ++;
    } else {
      piezo->noTone();
      lamp->digitalWrite(LOW);
      finished = true;
    }
  }
  public:
  MoveAnimation(Pin* lamp, Pin* piezo, Pitch pitch) :
    lamp(lamp), piezo(piezo), brightness(0), pitch(pitch), start(0), started(false) {
    blocking = true;
  }
  void loop() {
    if(!started) {
      started = true;
      start = millis();
      lastLoop = start;
    }
    unsigned long time = millis();
    frame(time - lastLoop, time - start);
    lastLoop = time;
  }
};

class Red : public Move {
  public:
    Red(Pin* lamp, Pin* piezo) : Move(lamp), piezo(piezo) { color = RED; }
    Red* indicate(bool blocking) {
      MoveAnimation* a = new MoveAnimation(lamp, piezo, A);
      a->blocking = blocking;
      AnimationQueue::add(a);
      return this;
    }
  private:
    Pin* piezo;
};

class Yellow : public Move {
  public:
    Yellow(Pin* lamp, Pin* piezo) : Move(lamp), piezo(piezo) { color = YELLOW; }
    Yellow* indicate(bool blocking) {
      MoveAnimation* a = new MoveAnimation(lamp, piezo, E);
      a->blocking = blocking;
      AnimationQueue::add(a);
      return this;
    }
  private:
    Pin* piezo;
};

class IndicatingButton : public PollButton {
  public:
    IndicatingButton(Move* move, int pin) : PollButton(new Pin(pin, INPUT)), move(move) { }

  bool wasPressed() {
    bool was = PollButton::wasPressed();
    if(was) move->indicate(false);
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

void indicateLevel(int level) {
  Pitch scale[] = {C,D,E,F,G,A,B};
  int factor = -1;
  for(int i = 0; i< level; i++) {
    redLamp->digitalWrite(i % 2);
    yellowLamp->digitalWrite((i + 1) % 2);
    int j = (i % 7);
    if(j == 0) factor ++;
    int freq = scale[j] * (1 << factor);
    piezo->tone(freq, 100);
    delay(125);
  }
  redLamp->digitalWrite(LOW);
  yellowLamp->digitalWrite(LOW);
}

Color getPress() {
  static bool seeded = false;
  Color color = NONE;
  if(yellowButton->wasPressed()) {
    if(!seeded) {
      randomSeed(millis());
      seeded = true;
    }
    color = YELLOW;
  } else if(redButton->wasPressed()) {
    color = RED;
  }
  return color;
}

Move* red;
Move* yellow;

void playSequence(List<Color> * sequence) {
  for(List<Color>::iterator i = sequence->begin(); i.more(); ++i)
    (i.value() == YELLOW ? yellow : red)->indicate(true);
}

void setup() {
  redLamp      = new Pin(10, OUTPUT);
  yellowLamp   = new Pin(9, OUTPUT);
  piezo        = new Pin(11, OUTPUT);
  red          = new Red(redLamp, piezo);
  yellow       = new Yellow(yellowLamp, piezo);
  redButton    = new IndicatingButton(red,    2);
  yellowButton = new IndicatingButton(yellow, 3);
  List<Color>* sequence = new List<Color>();
  sequence->push(YELLOW);
  game         = new Game(sequence);
  playSequence(sequence);
}


void loop() {
  AnimationQueue::loop();
  switch(game->record(getPress())) {
    case GAME_OVER:
      gameOver();
      indicateLevel(game->level);
      break;
    case BEAT_LEVEL:
      beatLevel();
      playSequence(game->sequence);
      break;
    case PLAYING:
      break;
  }
}
