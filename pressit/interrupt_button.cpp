#include "interrupt_button.h"

volatile int
InterruptButton::changes[] = {0, 0};

InterruptButton::InterruptButton(int num) {
	reportedChanges = digitalRead(num + 2);
	interuptNumber = num;
	changes[num] = reportedChanges;
	attachInterrupt(num, num == 0 ? change0 : change1, CHANGE);
}

/* the button has been pressed since the last time we asked */
bool
InterruptButton::wasPressed() {
	bool unreportedChanges = false;
	if(changes[interuptNumber] > reportedChanges) {
		unreportedChanges = true;
		reportedChanges++;
	}
	return unreportedChanges;
}

void
InterruptButton::change0() {
	changes[0]++;
}

void
InterruptButton::change1() {
	changes[1]++;
}
