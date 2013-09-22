const int blue   = 6;
const int green  = 10;
const int red    = 11;
const int pot    = 3;
const int button = 2;
const int colors[] = {red, green, blue};

void setup() {
	pinMode(red, OUTPUT);
	pinMode(green, OUTPUT);
	pinMode(blue, OUTPUT);
	pinMode(button, INPUT);
}

int currentColor() {
	static bool buttonWasDown;
	static int currentIndex = 0;

	if(digitalRead(button) == 0) {  // pushed
		if(!buttonWasDown)
			currentIndex = (currentIndex + 1) % 3;
		buttonWasDown = true;
	} else          		// button up
		buttonWasDown = false;
	return colors[currentIndex];
}

void loop() {
	static int lastBrightness = 0;
	int brightness = analogRead(pot) / 4;
	int color = currentColor();
	if(brightness != lastBrightness) {
		lastBrightness = brightness;
		analogWrite(color, brightness);
	}
}
