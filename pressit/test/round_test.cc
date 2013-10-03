#include "round.h"
#include "poll_button.h"
#include "test_pin.h"
#include "gtest/gtest.h"

class MockPin : public Pin {
public:
	MockPin(): Pin(0, 0) {}
	void setNextRead(int nextRead) {
		this->nextRead = nextRead;
	}
	int digitalRead() {
		return nextRead;
	}
private:
	int nextRead;
};

TEST(RoundTest, Works) {
	MockPin* redPin    = new MockPin();
	MockPin* yellowPin = new MockPin();
	Button* red        = new PollButton(redPin);
	Button* yellow     = new PollButton(yellowPin);
	Round* round       = new Round(red, yellow);

	redPin->setNextRead(1);
	yellowPin->setNextRead(1);
	round->tick();
	yellowPin->setNextRead(0);
	round->tick();
	EXPECT_FALSE(round->redWon());
	EXPECT_TRUE(round->over());

	redPin->setNextRead(true);
	round->tick();
	EXPECT_FALSE(round->redWon());
}
