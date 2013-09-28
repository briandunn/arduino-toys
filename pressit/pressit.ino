// Be the first to press your button when the song ends!
#include "round.h"

const int yellow          = 13;
const int yellowButtonPin = 12;
const int piezo           = 11;
const int red             = 10;
const int redButtonPin    = 9;
const int roundCount      = 1;
Round* rounds[roundCount];

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
	Serial.begin(9600);
	pinMode(yellow, OUTPUT);
	pinMode(red, OUTPUT);
	pinMode(piezo, OUTPUT);
        Button* yellowButton = new Button(new Pin(yellowButtonPin, INPUT));
        Button* redButton    = new Button(new Pin(redButtonPin, INPUT));
	for(int i = 0; i++; i < roundCount) {
		rounds[i] = new Round(redButton, yellowButton);
	}
}

void winner(int ledPin) {
	digitalWrite(ledPin, HIGH);
}

int redWins() {
	int wins = 0;
	for(int i = 0; i < roundCount; i++)
		if(rounds[i]->redWon()) wins++;

	return wins;
}

void loop() {
	static bool gameOver   = false;
	static int roundNumber = 0;
	Round* round           = rounds[roundNumber];

	if(round->tick() && !gameOver) {
		Serial.print("round ");
		Serial.print(roundNumber);
		Serial.println(" is over.");
		winner(round->redWon() ? red : yellow);
		if(roundNumber == roundCount) { // game over
			gameOver = true;
			if(redWins() > roundCount / 2.0)
				Serial.println("red is the big winner!");
			else
				Serial.println("yellow is the best.");
		} else roundNumber++; // move on to the next round
	}
}
