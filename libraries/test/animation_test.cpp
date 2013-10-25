#include "gtest/gtest.h"
#include "animation.h"

class FooAnimation : public Animation {
	public:
		FooAnimation() : loopCalls(0) {}
		void loop() { loopCalls ++; }
		int loopCalls;
};

TEST(AnimationQueueTest, loopCallsAnimationLoops) {
	FooAnimation* animation = new FooAnimation();
	AnimationQueue::add(animation);
	AnimationQueue::loop();
	ASSERT_EQ(animation->loopCalls, 1);
	AnimationQueue::loop();
	ASSERT_EQ(animation->loopCalls, 2);
}

TEST(AnimationQueueTest, acceptMultipleAnimationLoops) {
	FooAnimation* a = new FooAnimation();
	FooAnimation* b = new FooAnimation();
	AnimationQueue::add(a);
	AnimationQueue::add(b);
	AnimationQueue::loop();
	ASSERT_EQ(a->loopCalls, 1);
	ASSERT_EQ(b->loopCalls, 1);
}
