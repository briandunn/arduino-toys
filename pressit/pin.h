class Pin {
public:
	Pin(int, int);
	virtual int digitalRead();
	virtual void digitalWrite(int);
	virtual void tone(int);
	virtual void noTone();
private:
	int num;
};
