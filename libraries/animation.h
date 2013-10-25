template<typename T> class List {
	class node {
		public:
		node(T item, node* next) : item(item), next(next) {}
		T item;
		node* next;
	};

	node* first;

	public:
    List() : first(NULL) {}

		class iterator {
			node* current;
			public:
			iterator(node* start) : current(start) { }
			bool operator!=(const iterator& other) {
				return false;
			}

			bool operator!=(long foo) {
				return true;
			}

			T operator->() {
				return NULL;
			}

			T value() {
				return current->item;
			}

			iterator& operator++() {
				return *this;
			}
		};
		void push(T item) {
			first = new node(item, first);
		}
		iterator begin() {
			return iterator(NULL);
		};
};


class Animation {
	public:
		virtual void setup() = 0;
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
			for(auto i = list->begin(); i != NULL; ++i)
				i->loop();
		}

	private:
		static List<Animation*>* list;
		static bool calledSetup;
};

bool AnimationQueue::calledSetup = false;
List<Animation*>* AnimationQueue::list = new List<Animation*>();
