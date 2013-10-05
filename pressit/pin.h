#ifndef _pin_h_
#define _pin_h_
class Pin {
public:
	Pin(int, int);
	virtual int digitalRead();
	virtual void digitalWrite(int);
	virtual void tone(int);
	virtual void tone(int, int);
	virtual void noTone();
private:
	int num;
};
#endif
