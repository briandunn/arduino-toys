template<typename T> class List {
	class node {
		public:
		node(T item, node* previous, node* next) : item(item), previous(previous), next(next) {}
		T item;
		node* previous;
		node* next;
	};

	node* first;
	node* last;

	public:
	class iterator {
		public:
		List<T>* list;
		node* current;
		iterator(List<T>* list, node* start) : list(list), current(start) { }

		T value() {
			return current->item;
		}

		iterator& operator++() {
			if(more()) {
				current = current->next;
			}
			return *this;
		}

		/* f -> x <- l
		 * f -> x <-> 0 <- l
		 * f -> 0 <-> x <- l
		 * f -> 0 <-> x <-> 0 <- l
		 */
		void remove() {
			node* tmp = current;

			if(tmp == list->first) list->first = tmp->next;
			else tmp->previous->next = tmp->next;

			if(tmp == list->last) list->last = tmp->previous;
			else tmp->next->previous = tmp->previous;

			current = tmp->next;
			delete tmp;
		}

		bool more() { return !end(); }

		bool end() { return current == NULL; }
	};

	List() : first(NULL), last(NULL) {}

	void unshift(T item) {
		node* newFirst = new node(item, NULL, first);
		if(first != NULL) first->previous = newFirst;
		else last = newFirst;
		first = newFirst;
	}

	void push(T item) {
		node* newLast = new node(item, last, NULL);
		if(last != NULL) last->next = newLast;
		else first = newLast;
		last = newLast;
	}

	iterator begin() {
		return iterator(this, first);
	}
};
