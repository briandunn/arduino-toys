#include <iostream>
#include <string>
#include "round.h"
Pin::Pin(int p, int m) { }

int
Pin::read() {
	return 1;
}

class MockPin : public Pin {
public:
	MockPin(): Pin(0, 0) {}
	void setNextRead(int nextRead) {
		this->nextRead = nextRead;
	}
	int read() {
		return nextRead;
	}
private:
	int nextRead;
};

int exitStatus = 0;

void assert(bool test, std::string message) {
	if(!test) {
		std::cerr << message << "\n";
		exitStatus = 1;
	}
	else
		std::cout << '.';
}

int main(int argc, char** argv) {
	MockPin* redPin    = new MockPin();
	MockPin* yellowPin = new MockPin();
	Button* red        = new Button(redPin);
	Button* yellow     = new Button(yellowPin);
	Round* round       = new Round(red, yellow);

	redPin->setNextRead(1);
	yellowPin->setNextRead(1);
	round->tick();
	yellowPin->setNextRead(0);
	round->tick();
	assert(!round->redWon(), "yellow should've been the winner");
	assert(round->over(), "the round should be over");

	redPin->setNextRead(true);
	round->tick();
	assert(!round->redWon(), "the round has already been decided");
	delete round;
	delete red;
	delete yellow;
	delete redPin;
	delete yellowPin;
	std::cout << '\n';
	return exitStatus;
}
