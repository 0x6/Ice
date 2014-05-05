#ifndef DUPLI_CON_
#define DUPLI_CON_

#include<iostream>

template<typename T>
class DLNode{
public:
	DLNode(T t) : magnitude(1) {
		data = t;
		prev = NULL;
		next = NULL;
	}

	DLNode(T t, int size){
		prev = NULL;
		next = NULL;
		data = t;
		magnitude = size;
	}

	DLNode(const DLNode &src){
		prev = src.prev;
		next = src.next;
		data = src.data;
		magnitude = src.magnitude;
	}

	DLNode<T> operator=(const DLNode<T> &src){
		prev = src.prev;
		next = src.next;
		data = src.data;
		magnitude = src.magnitude;
		return *this;
	}

	void print(){
		std::cout << data << "\nThis" << this << "\nNext" << next << "\nPrev" << prev << "\n" << magnitude << "\n";
	}

	DLNode *next, *prev;
	T data;
	unsigned int magnitude;
private:
	DLNode(){}
};

template<typename T, typename Comp = std::less<T>>
class DLList{
public:
	DLList() : size(0), aggSize(0), comparator(){
		end = NULL;
		beg = NULL;
	}
	DLList(const T data) : size(1), aggSize(1), comparator() {
		DLNode<T> *begin = new DLNode<T>(data);
		begin->next = NULL;
		begin->prev = NULL;

		beg = begin;
		end = begin;
	}
	DLList(const DLList<T, Comp> &src){
		if (src.beg == NULL){
			beg = NULL;
			end = NULL;

			size = 0;
			aggSize = 0;
			comparator = src.getComp();
		}
		else {
			beg = new DLNode<T>(src.getBeg()->data, src.getBeg()->magnitude);
			DLNode<T> *currentSrc = src.getBeg()->next, *current = beg;
			
			while (currentSrc != NULL){
				current->next = new DLNode<T>(currentSrc->data, currentSrc->magnitude);
				current = current->next;
				currentSrc = currentSrc->next;
				if (currentSrc == NULL){
					end = current;
				}
			}
		}
	}
	~DLList(){
		clear();
	}

	DLList<T, Comp> operator=(const DLList<T, Comp> &src){
		this->clear();

		DLList<T, Comp> *temp = new DLList<T, Comp>(src);
		this->beg = temp->getBeg();
		this->end = temp->getEnd();

		this->size = temp->getSize();
		this->aggSize = temp->getAggSize();

		this->comparator = temp->comparator;

		return *this;
	}

	void append(const T data){
		DLNode<T> *current = beg;
		bool flag = false;

		while (current != NULL && !flag){
			if (!comparator(current->data, data) && !comparator(data, current->data)){
				flag = true;
				current->magnitude++;
				aggSize++;
			}
			current = current->next;
		}

		if (!flag){
			DLNode<T> *temp = new DLNode<T>(data);
			if (beg == NULL){
				beg = temp;
				end = temp;
			}
			else {
				temp->prev = end;
				end->next = temp;
				end = temp;
			}
			size++;
			aggSize++;
		}
		sort();
	}

	int find(T _data){
		for (int i = 0; i < size; i++){
			if (!comparator(get(i)->data, _data) && !comparator(_data, get(i)->data))
				return i;
		}
		return -1;
	}

	void remove(T _data){
		DLNode<T> *removed;
		int pos = find(_data);

		if (pos >= 0){
			removed = get(pos);

			if (removed == beg || removed == end){
				if (removed == beg){
					beg = removed->next;
					beg->prev = NULL;
				}
				if (removed == end){
					end = get(pos - 1);
					end->next = NULL;
				}
			}
			else {
				get(pos - 1)->next = removed->next;
				removed->next->prev = get(pos - 1);
			}

			delete removed;
			size--;
		}
	}

	void sort(){
		int low;
		
		for (int i = 0; i < size; i++){
			low = i;

			for (int j = i + 1; j < size; j++){
				if (comparator(get(j)->data, get(low)->data))
					low = j;
			}
			swap(get(i), get(low));
		}
	}

	void swap(DLNode<T> *node1, DLNode<T> *node2){
		DLNode<T> temp(*node1);
		
		node1->data = node2->data;
		node2->data = temp.data;

		node1->magnitude = node2->magnitude;
		node2->magnitude = temp.magnitude;
	}

	DLNode<T>* get(int element){
		DLNode<T> *current = beg;

		for (int i = 0; i < element; i++){
			current = current->next;
			if (current == NULL)
				return NULL;
		}

		return current;
	}

	void print() const{
		DLNode<T> *current = beg;
		while (current != NULL){
			current->print();
			std::cout << std::endl;
			current = current->next;
		}
	}

	void clear(){
		DLNode<T> *current = beg, *next;

		while (current != NULL){
			next = current->next;
			delete current;
			current = next;
		}

		beg = NULL;
		end = NULL;
		size = 0;
		aggSize = 0;
	}

	unsigned int getSize() const{
		return size;
	}

	unsigned int getAggSize() const{
		return aggSize;
	}

	DLNode<T>* getBeg() const{
		return beg;
	}

	DLNode<T>* getEnd() const{
		return end;
	}

	Comp getComp() const{
		return comparator;
	}
private:
	Comp comparator;
	DLNode<T> *beg, *end;
	unsigned int size, aggSize;
};


//ACTUAL CLASS
template<class T, typename Comp = std::less<T>>
class DupliCon{
public:
	typedef unsigned int size_type;
	typedef T value_type;

	class iterator{
	public:
		iterator() : ptr(NULL) {}
		iterator(const iterator &src) : ptr(src.ptr), mag(src.ptr->magnitude), pos(0) {}
		iterator operator=(const iterator &src){ 
			ptr = src.ptr;
			mag = src.mag;
			pos = src.pos;
		}

		//Preincrement
		iterator &operator++(){
			mag = ptr->magnitude;
			pos++;
			if (!(pos < mag)){
				ptr = ptr->next;
				pos = 0;
				mag = ptr->magnitude;
			}

			return *this;
		}
		//Postincrement
		iterator operator++(int){
			const iterator tmp = *this;
			++*this;
			return tmp;
		}

		//Predecrement
		iterator &operator--(){
			pos--;
			if (!(pos >= 0)){
				ptr = ptr->prev;
				pos = 0;
				mag = ptr->magnitude;
			}

			return *this;
		}
		//Postdecrement

		iterator operator--(int){
			const iterator tmp = *this;
			--*this;
			return tmp;
		}

		//Indirection
		T &operator*() const{
			return ptr->data;
		}
		//->
		T *operator->() const{
			return &(ptr->data);
		}

		//Inequality
		bool operator==(const iterator &it) const {
			return ptr == it.ptr;
		}
		//Inequality
		bool operator!=(const iterator &it) const {
			return !(*this == it);
		}

	private:
		DLNode<T> *ptr;
		friend class DupliCon;
		iterator(DLNode<T> *_ptr) : ptr(_ptr) {}
		int mag, pos;
	};

	/* Constructors and operator redefinitions */
	DupliCon() : elementList(), max(0){
		max = ~max;
	}
	DupliCon(const DupliCon &src) : max(0){
		max = ~max;
		elementList = src.elementList;
	}
	DupliCon(const T *it1, const T *it2) : elementList(), max(0){
		max = ~max;
		insert(it1, it2);
	}
	DupliCon operator=(const DupliCon &src){
		elementList = src.elementList;
	}
	~DupliCon(){
		elementList.clear();
	}

	/* Mutators */

	void empty(){
		elementList.clear();
	}

	void insert(const T t){
		elementList.append(t);
	}

	void insert(const T *it1, const T *it2){
		while (it1 != it2){
			insert(*it1);
			it1++;
		}
	}

	void erase(const T &val){
		elementList.remove(val);
	}

	void erase(iterator it){
		erase(*it);
	}

	/* Acessors */

	iterator begin(){
		return iterator(elementList.getBeg());
	}

	iterator end(){
		return iterator(elementList.getEnd());
	}

	iterator find(T t){
		int pos = elementList.find(t);
		if (pos >= 0){
			return iterator(elementList.get(pos));
		}
		return iterator(NULL);
	}

	int count(T t){
		int pos = elementList.find(t);
		if (pos >= 0){
			return elementList.get(pos)->magnitude;
		}
		return 0;
	}

	unsigned int size(){
		return elementList.getAggSize();
	}

	unsigned int unique_size(){
		return elementList.getSize();
	}

	unsigned int max_size(){
		unsigned int temp = 0;
		temp = ~temp;
		return temp;
	}

	void print(){
		elementList.print();
	}

private:
	DLList<T, Comp> elementList;
	unsigned int max;
};

#endif
