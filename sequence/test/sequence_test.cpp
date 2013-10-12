#include "pin.h"
#include "test/test_pin.h"
#include "interrupt_button.h"
void delay(int i) {}
int random(int range) { return 1; }
InterruptButton::InterruptButton(int i) {}
bool InterruptButton::wasPressed() { return true; }

TEST(RecorderTest, recordPresses) {
  // there are three ways out:
  // 1) neither button has been pressed
  // 2) the correct button has been pressed
  // 3) the incorrect button has been pressed
}
