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
Song* song;

void setup() {
	red                  = new Pin(10, OUTPUT);
	piezo                = new Pin(11, OUTPUT);
	yellow               = new Pin(13, OUTPUT);
	Button* redButton    = new InterruptButton(0);
	Button* yellowButton = new InterruptButton(1);
	song                 = new Song(piezo, millis);
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

void lightsOff() {
	red->digitalWrite(LOW);
	yellow->digitalWrite(LOW);
}

void roundWon(Pin* ledPin) {
	lightsOff();
	for(int t = 100; t < 1000; t += 100) {
		ledPin->analogWrite(t % 250);
		piezo->tone(t);
		delay(100);
	}
	ledPin->digitalWrite(LOW);
}

void gameWon(Pin* ledPin) {
	lightsOff();
	for(float f = 0; f < 1; f += 0.01) {
		ledPin->analogWrite(f * 255);
		piezo->tone(f * 880);
		delay(20);
	}
	piezo->noTone();
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
		} else {
			roundNumber++; // move on to the next round
			song->over = false;
		}
	}
}
