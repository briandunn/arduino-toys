#include "interrupt_button.h"
#include "gtest/gtest.h"

void (* callback)();
int nextReading;

void attachInterrupt(int num, void (* cb)(), int event) {
	callback = cb;
}

int digitalRead(int num) { return nextReading; }

class InterruptButtonTest : public testing::Test {
	protected:
	virtual void SetUp() {
		button = new InterruptButton(0);
	}
	InterruptButton* button;
};

TEST_F(InterruptButtonTest, noPresses) {
	EXPECT_FALSE(button->wasPressed());
}

TEST_F(InterruptButtonTest, onePress) {
	nextReading = 0;
  callback();
	EXPECT_FALSE(button->wasPressed()); // the button is up
	nextReading = 1;
  callback();
	EXPECT_TRUE(button->wasPressed()); // down
	EXPECT_FALSE(button->wasPressed()); // the press was already reported
}

TEST_F(InterruptButtonTest, twoPresses) {
	nextReading = 0;
  callback();
	nextReading = 1;
  callback();
	nextReading = 0;
  callback();
	nextReading = 1;
  callback();
	EXPECT_TRUE(button->wasPressed());
	EXPECT_TRUE(button->wasPressed());
	EXPECT_FALSE(button->wasPressed());
}

TEST_F(InterruptButtonTest, misfiredCallbacks) {
	nextReading = 0;
  callback();
  callback();
  callback();
	nextReading = 1;
  callback();
	nextReading = 0;
  callback();
  callback();
  callback();
	nextReading = 1;
  callback();
	EXPECT_TRUE(button->wasPressed());
	EXPECT_TRUE(button->wasPressed());
	EXPECT_FALSE(button->wasPressed());
}
