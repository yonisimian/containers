#include "stack.h"

template<class T>
Stack<T>::Stack(int size) : data(new T[size]), size(size), nextIndex(0) {}

template <class T>
Stack<T>::Stack(const Stack<T>& s) :
	data(new T[s.size]), size(s.size), nextIndex(s.nextIndex) {
	for (int i = 0; i < nextIndex; ++i)
		data[i] = s.data[i];
}

template <class T>
Stack<T>::~Stack() { delete[] data; }

template <class T>
Stack<T>& Stack<T>::operator=(const Stack<T>& s) {
	if (this == &s)
		return *this;
	delete[] data;
	data = new T[s.size];
	size = s.size;
	nextIndex = s.nextIndex;
	for (int i = 0; i < nextIndex; ++i)
		data[i] = s.data[i];
	return *this;
}

template <class T>
void Stack<T>::push(const T& t) {
	if (nextIndex >= size)
		throw Full();
	data[nextIndex++] = t;
}

template <class T>
void Stack<T>::pop() {
	if (nextIndex <= 0)
		throw Empty();
	nextIndex--;
}

template <class T>
T& Stack<T>::top() {
	if (nextIndex <= 0)
		throw Empty();
	return data[nextIndex - 1];
}

template <class T>
const T& Stack<T>::top() const {
	if (nextIndex <= 0)
		throw Empty();
	return data[nextIndex - 1];
}

template <class T>
int Stack<T>::getSize() const {
	return size;
}
