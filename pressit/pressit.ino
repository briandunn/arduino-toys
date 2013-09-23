// Be the first to press your button when the song ends!
#include "pressit.h"
const int yellow       = 13;
const int yellowButton = 12;
const int piezo        = 11;
const int red          = 10;
const int redButton    = 9;
Button* yb;
Button* rb;

bool song() {
	const int A        = 440;
	const int E        = 659;
	const int duration = 250;
	bool playing       = true;
	unsigned long currentMillis = millis();
	if      (currentMillis < duration * 1) tone(piezo, A);
	else if (currentMillis < duration * 2) noTone(piezo);
	else if (currentMillis < duration * 3) tone(piezo, A);
	else if (currentMillis < duration * 4) noTone(piezo);
	else if (currentMillis < duration * 5) tone(piezo, E);
	else {
		noTone(piezo);
		playing = false;
	}
	return playing;
}

void setup() {
	pinMode(yellow, OUTPUT);
	pinMode(red, OUTPUT);
	pinMode(piezo, OUTPUT);
	yb = new Button(yellowButton);
	rb = new Button(redButton);
}

void winner(int ledPin) {
	digitalWrite(ledPin, HIGH);
}

void loop() {
	static bool over = false;
	if(!song() && !over) {
		if (rb->wasPressed()) {
			winner(red);
			over = true;
		}
		if (yb->wasPressed()) {
			winner(yellow);
			over = true;
		}
	}
}
