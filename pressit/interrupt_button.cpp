#include "interrupt_button.h"

volatile int
InterruptButton::changes[] = {0, 0};

InterruptButton::InterruptButton(int num) {
	reportedChanges = 0;
	interuptNumber = num;
	changes[num] = reportedChanges;
	attachInterrupt(num, num == 0 ? change0 : change1, CHANGE);
}

/* the button has been pressed since the last time we asked */
 /* if the button starts down, the changes sequence will be something like */
 /* 0 ^ 1 v 2 ^ 3 v 4 */
 /* if the button starts up, it looks like */
 /*     1 v 2 ^ 3 v 4 */

 /* so every even number represents the button in the downward position. */
 /* so the answer to wasPressed: does the difference between the reported Changes and the changes span any even numbers? */

bool
InterruptButton::wasPressed() {
	bool unreportedChanges = false;
	if(changes[interuptNumber] > reportedChanges) {
		unreportedChanges = true;
		reportedChanges++;
	}
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
	Serial.println(changes[0]);
}
void
InterruptButton::change1() {
	if(changed(digitalRead(3)))
		changes[1] ++;
	Serial.println(changes[1]);
}
