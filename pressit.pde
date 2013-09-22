// Be the first to press your button when the song ends!
const int yellow       = 13;
const int yellowButton = 12;
const int piezo        = 11;
const int red          = 10;
const int redButton    = 9;

bool song() {
	const int A        = 440;
	const int E        = 659;
	const int duration = 250;
	bool playing       = true;
	unsigned long currentMillis = millis();
	if      (currentMillis < duration * 1) tone(piezo, A);
	else if (currentMillis < duration * 2) noTone(piezo);
	else if (currentMillis < duration * 3) tone(piezo, A);
	else if (currentMillis < duration * 4) noTone(piezo);
	else if (currentMillis < duration * 5) tone(piezo, E);
	else {
		noTone(piezo);
		playing = false;
	}
	return playing;
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
	if(!song()) {
		if (pressedFirst(redButton)) digitalWrite(red, HIGH);
		if (pressedFirst(yellowButton)) digitalWrite(yellow, HIGH);
	}
}
