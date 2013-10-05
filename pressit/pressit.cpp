// Be the first to press your button when the song ends!
#include "pin.h"
#include "song.h"

Song* song;

void setup() {
	Song* song = new Song(new Pin(11, OUTPUT), millis);
}

void loop() {
	song->play();
}
