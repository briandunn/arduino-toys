#include "list.h"
class Animation {
	public:
		Animation() : finished(false), blocking(false) {}
		bool finished;
		bool blocking;
		virtual void loop() = 0;
		virtual ~Animation() {}
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
			if(blocker != NULL) {
				if(blocker->finished)
					blocker = NULL;
				else
					blocker->loop();
			}

			if(blocker == NULL) {
				List<Animation*>::iterator i = list->begin();
				while(i.more()) {
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
						++i;
					}
				}
			}
		}
		static int length() {
			return list->length();
		}
		static void clear() {
			list->clear();
			blocker = NULL;
		}

	private:
		static List<Animation*>* list;
		static Animation* blocker;
};

List<Animation*>* AnimationQueue::list = new List<Animation*>();
Animation* AnimationQueue::blocker = NULL;
