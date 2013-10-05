#include "button.h"
#include "song.h"
class Round {

	public:

	Round(Song* song, Button* red, Button* yellow) {
		_red    = red;
		_yellow = yellow;
		_redWon = false;
		_over   = false;
		this->song = song;
	}

	bool tick() {
		if(!song->over) {
			song->play();
			return false;
		}
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

	Song* song;
	private:

	bool _redWon;
	bool _over;
	Button* _red;
	Button* _yellow;
};
