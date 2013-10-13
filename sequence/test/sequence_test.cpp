#include "pin.h"
#include "gtest/gtest.h"
#include "test/test_pin.h"
#include "../sequence.cpp"
void delay(int i) {}
int random(int range) { return 1; }
InterruptButton::InterruptButton(int i) {}
bool InterruptButton::wasPressed() { return true; }

  // there are four ways out:
  // 1) neither button has been pressed
  // 2) the correct button has been pressed but the sequence is incomplete
  // 2) the correct button has been pressed and the sequence is complete
  // 3) the incorrect button has been pressed and you loose
TEST(RecorderTest, noPress) {
  Button* redButton    = new InterruptButton(0);
  Button* yellowButton = new InterruptButton(1);
  Step* step = new Step(new Red(new Pin(0,0), new Pin(0,0)));
  Recorder* recorder = new Recorder(step, redButton, yellowButton);
  EXPECT_FALSE(recorder->complete);
  EXPECT_FALSE(recorder->success);
}
