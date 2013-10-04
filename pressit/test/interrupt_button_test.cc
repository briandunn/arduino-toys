#include "interrupt_button.h"
#include "gtest/gtest.h"

void (* callback)();

void attachInterrupt(int num, void (* cb)(), int event) {
	callback = cb;
}

int digitalRead(int num) {return 1;}

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
  callback();
	EXPECT_TRUE(button->wasPressed());
}
