#include "pin.h"
#include "gtest/gtest.h"
#include "test/test_pin.h"
#include "../sequence.cpp"
void delay(int i) {}
int random(int range) { return 1; }
InterruptButton::InterruptButton(int i) {}
bool InterruptButton::wasPressed() { return true; }

class StubButton : public Button {
  public:
    StubButton() : pressed(false) {}
    bool wasPressed() { return pressed; }
    bool pressed;
};

class RecorderTest : public testing::Test {
  protected:
  virtual void SetUp() {
    redButton    = new StubButton();
    yellowButton = new StubButton();
    red          = new Red(new Pin(0,0), new Pin(0,0));
    yellow       = new Yellow(new Pin(0,0), new Pin(0,0));
    step         = new Step(red);
    recorder     = new Recorder(step, redButton, yellowButton);
  }

  StubButton* redButton;
  StubButton* yellowButton;
  Move*       red;
  Move*       yellow;
  Step*       step;
  Recorder*   recorder;
};

  // neither button has been pressed
TEST_F(RecorderTest, noPress) {
  recorder->recordPress();

  EXPECT_FALSE(recorder->complete);
  EXPECT_FALSE(recorder->success);
}

  // the correct button has been pressed but the sequence is incomplete
TEST_F(RecorderTest, incomplete) {
  step->next = new Step(red);
  redButton->pressed = true;

  recorder->recordPress();

  EXPECT_FALSE(recorder->complete);
  EXPECT_TRUE(recorder->success);
}

  // the correct button has been pressed and the sequence is complete
TEST_F(RecorderTest, complete) {
  redButton->pressed = true;

  recorder->recordPress();

  EXPECT_TRUE(recorder->complete);
  EXPECT_TRUE(recorder->success);
}

  // the incorrect button has been pressed and you loose
TEST_F(RecorderTest, incorrect) {
  yellowButton->pressed = true;

  recorder->recordPress();

  EXPECT_TRUE(recorder->complete);
  EXPECT_FALSE(recorder->success);
}

  // sequential presses
TEST_F(RecorderTest, sequentialPresses) {
  step->next = new Step(yellow);
  step->next->next = new Step(red);

  redButton->pressed = true;

  recorder->recordPress();

  EXPECT_FALSE(recorder->complete);
  EXPECT_TRUE(recorder->success);

  redButton->pressed = false;
  yellowButton->pressed = true;

  recorder->recordPress();

  EXPECT_FALSE(recorder->complete);
  EXPECT_TRUE(recorder->success);

  redButton->pressed = false;
  yellowButton->pressed = true;

  recorder->recordPress();

  EXPECT_TRUE(recorder->complete);
  EXPECT_FALSE(recorder->success);
}

