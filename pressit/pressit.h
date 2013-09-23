#include <Arduino.h>
class Button {

	public:

	Button(uint8_t p) {
		pinMode(p, INPUT);
		wasUp = false;
		pin = p;
	}

	/* returns true when on the last call the button was up
	 * but on this call the button is down */
	bool wasPressed() {
		bool was_pressed = false;
		if(isPressed()) {
			if(wasUp)
				was_pressed = true;
			wasUp = false;
		} else wasUp = true;
		return was_pressed;
	}

	bool isPressed() {
		return digitalRead(pin) == 0;
	}

	private:

	uint8_t pin;
	bool wasUp;
};
