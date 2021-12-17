#ifndef _MY_STACK_

#define _MY_STACK_

#include <stdexcept>

template <typename T>
class Stack
{
private:
	struct element;
public:
	Stack();
	Stack(const Stack& other);
	~Stack();

	Stack& operator=(const Stack& other);

	T pop();
	T top() const;
	void push(T value);
	bool is_empty() const;
private:
	element* _top;

	void _copy(const Stack& other);
	void _clear();
};

template <typename T>
struct Stack<T>::element {
	T value;
	element* next;
};

#endif //! _MY_STACK_
