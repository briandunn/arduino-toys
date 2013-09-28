#include "button.h"
class Round {

	public:

	Round(Button* red, Button* yellow) {
		_red    = red;
		_yellow = yellow;
		_redWon = false;
		_over   = false;
	}

	bool tick() {
		if(!_over) {
			if(_yellow->wasPressed()) _over = true;

			if(_red->wasPressed()) {
				_redWon = true;
				_over = true;
			}
		}
		return _over;
	}

	bool over() { return _over; }
	bool redWon() { return _redWon; }

	private:

	bool _redWon;
	bool _over;
	Button* _red;
	Button* _yellow;
};
