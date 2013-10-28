template<typename T> class List {
	class node {
		public:
		node(T item, node* previous, node* next) : item(item), previous(previous), next(next) {}
		T item;
		node* previous;
		node* next;
	};

	node* first;

	public:
	class iterator {
		public:
		node* current;
		List<T>* list;
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

		/* f -> x
		 * f -> x <-> 0
		 * f -> 0 <-> x
		 * f -> 0 <-> x <-> 0
		 */
		void remove() {
			node* tmp = current;
			if(tmp == list->first)
				list->first = tmp->next;
			if(tmp->next != NULL)
				tmp->next->previous = tmp->previous;
			if(tmp->previous != NULL)
				tmp->previous->next = tmp->next;

			current = tmp->next;
			delete tmp;
		}

		bool more() { return !end(); }

		bool end() { return current == NULL; }
	};

	List() : first(NULL) {}

	void unshift(T item) {
		node* newFirst = new node(item, NULL, first);
		if(first != NULL) first->previous = newFirst;
		first = newFirst;
	}

	iterator begin() {
		return iterator(this, first);
	}
};
