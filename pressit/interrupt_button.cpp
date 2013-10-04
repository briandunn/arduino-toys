#include "interrupt_button.h"

volatile int
InterruptButton::changes[] = {0, 0};

InterruptButton::InterruptButton(int num) {
	reportedChanges = 0;
	interruptNumber = num;
	changes[num] = reportedChanges;
	attachInterrupt(num, num == 0 ? change0 : change1, CHANGE);
}

bool isDown(int changeNumber) { return changeNumber % 2 == 0; }

bool
InterruptButton::wasPressed() {
	int changeCount = changes[interruptNumber];
	bool unreportedChanges = isDown(changeCount) && changeCount > reportedChanges;

	if(unreportedChanges)
		reportedChanges += 2;
	return unreportedChanges;
}

bool changed(int value) {
	static int lastValue = -1;
	bool c = false;
	if(value != lastValue)
		c = true;
	lastValue = value;
	return c;
}

void
InterruptButton::change0() {
	if(changed(digitalRead(2)))
		changes[0] ++;
}
void
InterruptButton::change1() {
	if(changed(digitalRead(3)))
		changes[1] ++;
}
