#include "gtest/gtest.h"
int random(int);
#include "game.h"
int random(int range) { return 1; }

class GameTest : public testing::Test {
  protected:
  virtual void SetUp() {
    step = new Step(RED);
    game = new Game(step);
  }

  Step* step;
  Game* game;
};

  // neither button has been pressed
TEST_F(GameTest, noPress) {
  ASSERT_EQ(game->record(NONE), PLAYING);
}

  // the correct button has been pressed but the sequence is incomplete
TEST_F(GameTest, incomplete) {
  step->next = new Step(RED);
  ASSERT_EQ(game->record(RED), PLAYING);
}

  // the correct button has been pressed and the sequence is complete
TEST_F(GameTest, complete) {
  ASSERT_EQ(game->record(RED), BEAT_LEVEL);
}

  // the incorrect button has been pressed and you loose
TEST_F(GameTest, incorrect) {
  ASSERT_EQ(game->record(YELLOW), GAME_OVER);
}

  // sequential presses
TEST_F(GameTest, sequentialPresses) {
  step->next = new Step(YELLOW);
  step->next->next = new Step(RED);

  ASSERT_EQ(game->record(RED), PLAYING);
  ASSERT_EQ(game->record(YELLOW), PLAYING);
  ASSERT_EQ(game->record(YELLOW), GAME_OVER);
}

  // beating a level adds a step and starts over
  // (the added steps are always YELLOW in our test)
TEST_F(GameTest, incrementLevel) {
  ASSERT_EQ(game->record(RED), BEAT_LEVEL);

  ASSERT_EQ(game->record(RED), PLAYING);
  ASSERT_EQ(game->record(YELLOW), BEAT_LEVEL);

  ASSERT_EQ(game->record(RED), PLAYING);
  ASSERT_EQ(game->record(YELLOW), PLAYING);
  ASSERT_EQ(game->record(RED), GAME_OVER);
}
