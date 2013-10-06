#include "pin.h"
#include "Arduino.h"

Pin::Pin(int num, int mode) {
	this->num = num;
	pinMode(num, mode);
}

int  Pin::digitalRead()         { return ::digitalRead(num); }
void Pin::digitalWrite(int val) { ::digitalWrite(num, val); }
void Pin::analogWrite(int val)  { ::analogWrite(num, val); }
void Pin::tone(int val)         { ::tone(num, val); }
void Pin::tone(int val, int dur){ ::tone(num, val, dur); }
void Pin::noTone()              { ::noTone(num); }
