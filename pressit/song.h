#ifndef _song_h_
#define _song_h_
#include "pin.h"
#include "Arduino.h"
class Song {
	public:

	Song(Pin* pin, long unsigned int (*clock)()) {
		this->pin = pin;
		this->clock = clock;
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
	long unsigned int (* clock)();
	static const int A        = 440;
	static const int E        = 659;
	static const int duration = 250;
};
#endif
