enum Color { RED, YELLOW, NONE };
enum GameStatus {PLAYING, GAME_OVER, BEAT_LEVEL};

class Step {
  public:
    Step(Color color) :next(NULL), color(color) { }
    Step* next;
    Color color;
    Step* increment() {
      Step* step = new Step(randomColor());
      Step* current = this;
      while(current->next != NULL)
        current = current->next;
      current->next = step;
      return step;
    }
    static Step* first() {
      if(_first == NULL)
        _first = new Step(randomColor());
      return _first;
    }
  private:
    static Step* _first;
    static Color randomColor() {
      return random(2) == 0 ? RED : YELLOW;
    }
};

Step* Step::_first = NULL;
class Game {
  public:
    Game(Step* first): first(first), current(first), status(PLAYING) { }

    Step* first;

    GameStatus record(Color color) {
      GameStatus status = PLAYING;
      if(color == NONE) return status;
      if(color == current->color) {
        if(current->next == NULL) {
          current->increment();
          current = first;
          status = BEAT_LEVEL;
        } else current = current->next;
      } else status = GAME_OVER;
      return status;
    }

  private:
    Step* current;
    GameStatus status;
};
