#ifndef list_h
#define list_h
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
	unsigned int _length;

 /* f -> x <- l
	* f -> x <-> 0 <- l
	* f -> 0 <-> x <- l
	* f -> 0 <-> x <-> 0 <- l
	*/
	void remove(node* tmp) {
		if(tmp == first) first = tmp->next;
		else tmp->previous->next = tmp->next;

		if(tmp == last) last = tmp->previous;
		else tmp->next->previous = tmp->previous;

		delete tmp;
		_length--;
	}

	public:
	unsigned int length() { return _length; }
	class iterator {
		public:
			List<T>* list;
			node* current;
			iterator(List<T>* list, node* start) : list(list), current(start) { }

			T value() { return current->item; }

			iterator& operator++() {
				if(more()) { current = current->next; }
				return *this;
			}

			void remove() {
				if(more()) {
					node* tmp = current;
					current = tmp->next;
					list->remove(tmp);
				}
			}

			bool more() { return !end(); }

			bool last() { return !end() && current->next == NULL; }

			bool end() { return current == NULL; }
	};

	List() : first(NULL), last(NULL), _length(0) {}

	void unshift(T item) {
		node* newFirst = new node(item, NULL, first);
		if(first != NULL) first->previous = newFirst;
		else last = newFirst;
		first = newFirst;
		_length ++;
	}

	void push(T item) {
		node* newLast = new node(item, last, NULL);
		if(last != NULL) last->next = newLast;
		else first = newLast;
		last = newLast;
		_length ++;
	}

	void clear() {
		iterator i = begin();
		while(i.more()) { i.remove(); }
	}

	iterator begin() {
		return iterator(this, first);
	}
};
#endif
