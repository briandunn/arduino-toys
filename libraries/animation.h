#include "list.h"
class Animation {
	public:
		Animation() : finished(false), blocking(false) {}
		bool finished;
		bool blocking;
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
			list->push(animation);
		}
		static void loop() {
			if(blocker != NULL)
				if(blocker->finished)
					blocker = NULL;
				else
					blocker->loop();
			else
				for(List<Animation*>::iterator i = list->begin(); i.more(); ++i) {
					Animation* animation = i.value();
					if(animation->finished) {
						i.remove();
						delete animation;
					} else {
						animation->loop();
						if(animation->blocking) {
							blocker = animation;
							break; // exit the loop, skipping the rest of the queue.
						}
					}
				}
		}

	private:
		static List<Animation*>* list;
		static Animation* blocker;
};

List<Animation*>* AnimationQueue::list = new List<Animation*>();
Animation* AnimationQueue::blocker = NULL;
