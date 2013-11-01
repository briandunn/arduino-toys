#include "Arduino.h"
#include "pin.h"
#include "animation.h"

Pin* redLamp = new Pin(10, INPUT);
Pin* yellowLamp = new Pin(9, INPUT);

class SawTooth : public Animation {
	Pin* pin;
	int brightness;
	int direction;
	unsigned long elapsed;
	public:
	SawTooth(Pin* pin) : pin(pin), brightness(0), direction(1), elapsed(0) {}
	void loop() {
		unsigned long time = millis();
		if(time - elapsed > 1) {
			elapsed = time;
			pin->analogWrite(brightness);
			brightness += direction;
			if (brightness % 255 == 0) direction *= -1;
		}
	}
};

class Strobe : public Animation {
	Pin* pin;
	int brightness;
	unsigned long elapsed;
	public:
	Strobe(Pin* pin) : pin(pin), brightness(0), elapsed(0) {}
	void loop() {
		unsigned long time = millis();
		if(time - elapsed > 100) {
			elapsed = time;
			pin->analogWrite(brightness);
			brightness = brightness == 0 ? 255 : 0;
		}
	}
};

void setup() {
	AnimationQueue::add(new SawTooth(redLamp));
	AnimationQueue::add(new Strobe(yellowLamp));
}
void loop() {
	AnimationQueue::loop();
}
