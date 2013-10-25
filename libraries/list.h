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
			if(more()) {
				node* tmp = current;
				current = current->next;
				previous->next = current;
				delete tmp;
			}
		}

		bool more() { return !end(); }

		bool end() { return current == NULL; }
	};

	List() : first(NULL) {}

	void push(T item) {
		first = new node(item, first);
	}

	iterator begin() {
		return iterator(first);
	}
};
