// Be the first to press your button when the song ends!
#include "round.h"
#include "pin.h"
#include "interrupt_button.h"

Pin* yellow;
Pin* piezo;
Pin* red;
const int roundCount = 3;
Round* rounds[roundCount];

bool song() {
	return false;
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
	red                  = new Pin(10, OUTPUT);
	piezo                = new Pin(11, OUTPUT);
	yellow               = new Pin(13, OUTPUT);
	Button* redButton    = new InterruptButton(0);
	Button* yellowButton = new InterruptButton(1);
	int i = 0;
	while(i < roundCount)
		rounds[i++] = new Round(redButton, yellowButton);

	Serial.begin(9600);
}

int redWins() {
	int wins = 0;
	for(int i = 0; i < roundCount; i++)
		if(rounds[i]->redWon()) wins++;

	return wins;
}

void winner(Pin* ledPin) {
	ledPin->digitalWrite(HIGH);
}

void loop() {
	static bool gameOver   = false;
	static int roundNumber = 1;
	Round* round           = rounds[roundNumber - 1];

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
