#include "gtest/gtest.h"
#include "animation.h"

class FooAnimation : public Animation {
	public:
		FooAnimation() : loopCalls(0) {}
		void loop() { loopCalls ++; }
		int loopCalls;
};

class AnimationQueueTest : public testing::Test {
	virtual void SetUp() {
		AnimationQueue::clear();
	}
};

TEST_F(AnimationQueueTest, loopCallsAnimationLoops) {
	FooAnimation* animation = new FooAnimation();
	AnimationQueue::add(animation);
	AnimationQueue::loop();
	ASSERT_EQ(animation->loopCalls, 1);
	AnimationQueue::loop();
	ASSERT_EQ(animation->loopCalls, 2);
}

TEST_F(AnimationQueueTest, acceptMultipleAnimationLoops) {
	FooAnimation* a = new FooAnimation();
	FooAnimation* b = new FooAnimation();
	AnimationQueue::add(a);
	AnimationQueue::add(b);
	AnimationQueue::loop();
	ASSERT_EQ(a->loopCalls, 1);
	ASSERT_EQ(b->loopCalls, 1);
}

TEST_F(AnimationQueueTest, removesFinishedAnimations) {
	FooAnimation* a = new FooAnimation();
	AnimationQueue::add(a);
	a->finished = true;
	AnimationQueue::loop();
	ASSERT_EQ(a->loopCalls, 0);
}

TEST_F(AnimationQueueTest, keepsRollingAfterRemoval) {
	FooAnimation* a = new FooAnimation();
	FooAnimation* b = new FooAnimation();
	AnimationQueue::add(a);
	a->finished = true;
	AnimationQueue::loop();
	ASSERT_EQ(a->loopCalls, 0);
	AnimationQueue::add(b);
	AnimationQueue::loop();
	ASSERT_EQ(a->loopCalls, 0);
	ASSERT_EQ(b->loopCalls, 1);
}

TEST_F(AnimationQueueTest, onlyAdvancesBlockingAnimations) {
	FooAnimation* a = new FooAnimation();
	FooAnimation* b = new FooAnimation();
	AnimationQueue::add(a);
	AnimationQueue::add(b);
	b->blocking = true;
	AnimationQueue::loop();
	ASSERT_EQ(a->loopCalls, 1);
	ASSERT_EQ(b->loopCalls, 1);
	AnimationQueue::loop();
	ASSERT_EQ(a->loopCalls, 1);
	ASSERT_EQ(b->loopCalls, 2);
}

TEST_F(AnimationQueueTest, removesBlockersAndCariesOn) {
	FooAnimation* a = new FooAnimation();
	FooAnimation* b = new FooAnimation();
	AnimationQueue::add(a);
	AnimationQueue::add(b);
	ASSERT_EQ(2, AnimationQueue::length());
	b->blocking = true;
	AnimationQueue::loop();
	ASSERT_EQ(a->loopCalls, 1);
	ASSERT_EQ(b->loopCalls, 1);
	AnimationQueue::loop();
	ASSERT_EQ(a->loopCalls, 1);
	ASSERT_EQ(b->loopCalls, 2);
	b->finished = true;
	AnimationQueue::loop();
	ASSERT_EQ(a->loopCalls, 2);
	ASSERT_EQ(b->loopCalls, 2);
}
