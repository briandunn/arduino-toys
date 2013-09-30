#include "poll_button.h"
PollButton::PollButton(Pin* pin) {
	this->pin = pin;
	wasUp = false;
}

bool PollButton::wasPressed() {
	bool was_pressed = false;
	if(isPressed()) {
		if(wasUp)
			was_pressed = true;
		wasUp = false;
	} else wasUp = true;
	return was_pressed;
}

bool PollButton::isPressed() {
	return pin->digitalRead() == 0;
}
