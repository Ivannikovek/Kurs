#ifndef _MY_STACK_HPP_

#define _MY_STACK_HPP_

#include "Stack.h"

template <typename T>
Stack<T>::Stack() :_top(nullptr) {

}

template <typename T>
Stack<T>::Stack(const Stack& other) {
	if (other._top == nullptr)
		_top = nullptr;
	else
		_copy(other._top);
}

template <typename T>
void Stack<T>::_copy(const Stack& other) {
	_top = new element{ other._top->value, nullptr };

	element* last_added = _top;
	element* temp = other._top->next;

	while (temp != nullptr) {
		last_added->next = new element{ temp->value, nullptr };

		last_added = last_added->next;
		temp = temp->next;
	}
}

template <typename T>
Stack<T>::~Stack() {
	_clear();
}

template <typename T>
void Stack<T>::_clear() {
	while (_top != nullptr) {
		element* removing = _top;

		_top = _top->next;

		delete removing;
	}
}

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack& other) {
	if (this != &other) {
		_clear();
		_copy(other);
	}

	return *this;
}

template <typename T>
T Stack<T>::pop() {
	if (_top == nullptr)
		throw std::runtime_error("Stack is empty.");

	T popping_value = _top->value;

	element* removing = _top;

	_top = _top->next;

	delete removing;

	return popping_value;
}

template <typename T>
T Stack<T>::top() const {
	if (_top == nullptr)
		throw std::runtime_error("Stack is empty.");

	return _top->value;
}

template <typename T>
void Stack<T>::push(T value) {
	_top = new element{ value, _top };
}

template <typename T>
bool Stack<T>::is_empty() const {
	return _top == nullptr;
}

#endif //! _MY_STACK_HPP_