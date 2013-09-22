const int pot    = 3;
const int piezo  = 9;
const int button = 2;

void setup() {
	pinMode(piezo, OUTPUT);
	pinMode(button, INPUT);
}

int note(int value) {
	const int A      = 440;
	const int CS     = 523;
	const int E      = 659;
	const int chord[] = {A/2,CS/2,E/2,A,CS,E,A*2,CS*2,E*2};
	/* 0..1024 */
	return chord[int((value / 1024.0) * 9)];
}

void loop() {
	if(digitalRead(button) == 0) tone(piezo, note(analogRead(pot)));
	else noTone(piezo);
}
