// Be the first to press your button when the song ends!
#include "round.h"

Pin* yellow          = new Pin(13, OUTPUT);
Pin* yellowButtonPin = new Pin(12, INPUT);
Pin* piezo           = new Pin(11, OUTPUT);
Pin* red             = new Pin(10, OUTPUT);
Pin* redButtonPin    = new Pin(9, INPUT);
const int roundCount       = 1;
Round* rounds[roundCount];

bool song() {
	const int A        = 440;
	const int E        = 659;
	const int duration = 250;
	bool playing       = true;
	unsigned long currentMillis = millis();
	if      (currentMillis < duration * 1) piezo->tone(A);
	else if (currentMillis < duration * 2) piezo->noTone();
	else if (currentMillis < duration * 3) piezo->tone(A);
	else if (currentMillis < duration * 4) piezo->noTone();
	else if (currentMillis < duration * 5) piezo->tone(E);
	else {
		piezo->noTone();
		playing = false;
	}
	return playing;
}

void setup() {
	Serial.begin(9600);
        Button* yellowButton = new Button(yellowButtonPin);
        Button* redButton    = new Button(redButtonPin);
	for(int i = 0; i++; i < roundCount) {
		rounds[i] = new Round(redButton, yellowButton);
	}
}

void winner(Pin* ledPin) {
	ledPin->digitalWrite(HIGH);
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
