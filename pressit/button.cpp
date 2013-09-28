#include "button.h"
Button::Button(Pin* pin) {
	pin = pin;
	wasUp = false;
}

/* returns true when on the last call the button was up
 * but on this call the button is down */
bool Button::wasPressed() {
	bool was_pressed = false;
	if(isPressed()) {
		if(wasUp)
			was_pressed = true;
		wasUp = false;
	} else wasUp = true;
	return was_pressed;
}

bool Button::isPressed() {
	return pin->read() == 0;
}
