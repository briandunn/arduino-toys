#include "Arduino.h"
#include "pin.h"
#include "animation.h"

auto redLamp = new Pin(10, INPUT);

class Strobe : public Animation {
				public:
				int brightness = 0;
				int direction = 1;
				void loop() {
								redLamp->analogWrite(brightness);
								brightness += direction;
								if (brightness % 255 == 0)
												direction *= -1;
				}
};

void setup() {
				AnimationQueue::add(new Strobe());
}
void loop() {
				AnimationQueue::loop();
}
