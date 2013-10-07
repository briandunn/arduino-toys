#ifndef _song_h_
#define _song_h_
#include "pin.h"
#include "Arduino.h"
class Song {
	public:

	Song(Pin* pin) {
		this->pin = pin;
		over = false;
	};

	bool over;

	void play() {
		over = false;
		pin->tone(A, duration);
		delay(duration * 2);
		pin->tone(A, duration);
		delay(duration * 2);
		pin->tone(E, duration);
		delay(duration * 2);
		over = true;
	}

	private:

	Pin* pin;
	static const int A        = 440;
	static const int E        = 659;
	static const int duration = 250;
};
#endif
