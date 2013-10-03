#include "interrupt_button.h"
#include "gtest/gtest.h"

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
