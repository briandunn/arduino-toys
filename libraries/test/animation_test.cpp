#include "gtest/gtest.h"
#include "animation.h"

TEST(ListTest, push) {
		auto list = new List<int>();
		int i = 1;
		list->push(i);
		ASSERT_EQ(list->begin().value(), 1);
}

class FooAnimation : public Animation {
	public:
		FooAnimation() : setupCalls(0), loopCalls(0) {}
		void setup() { setupCalls ++; }
		void loop() { loopCalls ++; }
		int setupCalls, loopCalls;
};

TEST(AnimationQueueTest, loopCallsAnimationLoops) {
	FooAnimation* animation = new FooAnimation();
	AnimationQueue::add(animation);
	ASSERT_EQ(animation->setupCalls, 0);
	AnimationQueue::loop();
	ASSERT_EQ(animation->setupCalls, 1);
	ASSERT_EQ(animation->loopCalls, 1);
	AnimationQueue::loop();
	ASSERT_EQ(animation->setupCalls, 1);
	ASSERT_EQ(animation->loopCalls, 2);
}

TEST(AnimationQueueTest, acceptMultipleAnimationLoops) {
	FooAnimation* a = new FooAnimation();
	FooAnimation* b = new FooAnimation();
	AnimationQueue::add(a);
	AnimationQueue::add(b);
	AnimationQueue::loop();
	ASSERT_EQ(a->setupCalls, 1);
	ASSERT_EQ(a->loopCalls, 1);
	ASSERT_EQ(b->setupCalls, 1);
	ASSERT_EQ(b->loopCalls, 1);
}
