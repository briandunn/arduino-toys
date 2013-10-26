#include "list.h"
class Animation {
	public:
		virtual void loop() = 0;
};

class AnimationItem {
	public:
		AnimationItem(Animation* animation): animation(animation) {}
		AnimationItem* next;
	private:
	Animation* animation;
};

class AnimationQueue {
	public:
		static void add(Animation * animation) {
			list->unshift(animation);
		}
		static void loop() {
			for(List<Animation*>::iterator i = list->begin(); i.more(); ++i)
				i.value()->loop();
		}

	private:
		static List<Animation*>* list;
};

List<Animation*>* AnimationQueue::list = new List<Animation*>();
