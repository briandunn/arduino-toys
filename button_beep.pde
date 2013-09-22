const int piezo  = 9;
const int button = 2;

void setup() {
	pinMode(piezo, OUTPUT);
	pinMode(button, INPUT);
}

void loop() {
	if(digitalRead(button) == 0) analogWrite(piezo, 440);
	else noTone(piezo);
}
