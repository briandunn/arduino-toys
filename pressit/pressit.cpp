// Be the first to press your button when the song ends!
#include "round.h"
#include "pin.h"
#include "interrupt_button.h"
#include "song.h"

Pin* yellow;
Pin* piezo;
Pin* red;
const int roundCount = 3;
Round* rounds[roundCount];

void setup() {
	red                  = new Pin(10, OUTPUT);
	piezo                = new Pin(11, OUTPUT);
	yellow               = new Pin(13, OUTPUT);
	Button* redButton    = new InterruptButton(0);
	Button* yellowButton = new InterruptButton(1);
	Song* song           = new Song(piezo, millis);
	int i = 0;
	while(i < roundCount)
		rounds[i++] = new Round(song, redButton, yellowButton);
	Serial.begin(9600);
}

int redWins() {
	int wins = 0;
	for(int i = 0; i < roundCount; i++)
		if(rounds[i]->redWon()) wins++;

	return wins;
}

void roundWon(Pin* ledPin) {
	ledPin->digitalWrite(HIGH);
}

void gameWon(Pin* ledPin) {
	ledPin->digitalWrite(HIGH);
}

void loop() {
	static bool gameOver   = false;
	static int roundNumber = 1;
	Round* round           = rounds[roundNumber - 1];
	if(round->tick() && !gameOver) {
		roundWon(round->redWon() ? red : yellow);
		if(roundNumber == roundCount) { // game over
			gameOver = true;
			if(redWins() > roundCount / 2.0)
				gameWon(red);
			else
				gameWon(yellow);
		} else roundNumber++; // move on to the next round
	}
}
