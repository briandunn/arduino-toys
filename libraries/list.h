template<typename T> class List {
	class node {
		public:
			node(T item, node* next) : item(item), next(next) {}
			T item;
			node* next;
	};

	node* first;

	public:
	class iterator {
		public:
		node* current;
		node* previous;
		iterator(node* start) : current(start), previous(NULL) { }

		T value() {
			return current->item;
		}

		iterator& operator++() {
			if(more()) {
				previous = current;
				current = current->next;
			}
			return *this;
		}

		void remove() {
			node* tmp = current;
			if(current->next != NULL) {
				current = current->next;
				if(previous != NULL)
					previous->next = current;
			} else {
				current = previous;
			}
			delete tmp;
		}

		bool more() { return !end(); }

		bool end() { return current == NULL; }
	};

	List() : first(NULL) {}

	void unshift(T item) {
		first = new node(item, first);
	}

	iterator begin() {
		return iterator(first);
	}
};
