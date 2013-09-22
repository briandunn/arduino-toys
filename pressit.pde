// Be the first to press your button when the song ends!
const int yellow       = 13;
const int red          = 3;
const int piezo        = 9;
const int redButton    = 2;
const int yellowButton = 12;

bool song() {
	const int A        = 440;
	const int E        = 659;
	const int duration = 500;
	bool over          = false;
	unsigned long currentMillis = millis();
	if      (currentMillis < duration * 1) tone(piezo, A);
	else if (currentMillis < duration * 2) noTone(piezo);
	else if (currentMillis < duration * 3) tone(piezo, A);
	else if (currentMillis < duration * 4) noTone(piezo);
	else if (currentMillis < duration * 5) tone(piezo, E);
	else {
		noTone(piezo);
		over = true;
	}
	return over;
}

bool pressedFirst(int pin) {
	static bool pressed = false;
	bool first = true;
	if(digitalRead(pin) == 0 && !pressed) pressed = true;
	else first = false;
	return first;
}

void setup() {
	pinMode(yellow, OUTPUT);
	pinMode(red, OUTPUT);
	pinMode(piezo, OUTPUT);
	pinMode(redButton, INPUT);
	pinMode(yellowButton, INPUT);
}

void loop() {
	if(song()) {
		if (pressedFirst(redButton)) digitalWrite(red, HIGH);
		if (pressedFirst(yellowButton)) digitalWrite(yellow, HIGH);
	}
}
