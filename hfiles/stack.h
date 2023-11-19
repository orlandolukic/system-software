#ifndef STACK_H_
#define STACK_H_

class Stack
{
private:
	int cnt;
	unsigned int capacity;
	int* array;

public:
	Stack(int cap);
	~Stack();

	bool isEmpty();
	char top();
	char pop();
	void push(char ch);
};



#endif /* STACK_H_ */
