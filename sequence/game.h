#include "list.h"
enum Color { RED, YELLOW, NONE };
enum GameStatus {PLAYING, GAME_OVER, BEAT_LEVEL};

class Game {
  public:
    Game(List<Color>* sequence): sequence(sequence), level(1), current(sequence->begin()), status(PLAYING) { }

    List<Color>* sequence;

    GameStatus record(Color color) {
      GameStatus status = PLAYING;
      if(color == NONE) return status;
      if(color == current.value()) {
        if(current.last()) {
          sequence->push(random(2) == 0 ? RED : YELLOW);
          current = sequence->begin();
          status = BEAT_LEVEL;
          level++;
        } else ++current;
      } else status = GAME_OVER;
      return status;
    }

    int level;

  private:
    List<Color>::iterator current;
    GameStatus status;
};
