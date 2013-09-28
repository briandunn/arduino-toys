#include "pin.h"
#include "Arduino.h"
Pin::Pin(int num, int mode) {
	num = num;
	pinMode(num, mode);
}

int Pin::read() {
	return digitalRead(num);
}
