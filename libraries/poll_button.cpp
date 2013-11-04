#include "poll_button.h"
PollButton::PollButton(Pin* pin) : pin(pin), val(pin->digitalRead()), lastAt(0) {}

bool PollButton::wasPressed() {
	int cur = pin->digitalRead();
	unsigned long now = millis();
	if(val != cur && (now - lastAt) > 10) {
		val = cur;
		lastAt = now;
		if(cur == 0) return true;
	}
	return false;
}
