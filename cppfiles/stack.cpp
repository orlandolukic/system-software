#include "../hfiles/stack.h"

#include <stdlib.h>

Stack::Stack(int cap)
{
	cnt = -1;
	capacity = cap;
	array = (int*) malloc(capacity * sizeof(int));
}

Stack::~Stack()
{
	delete[] array;
}

bool Stack::isEmpty()
{
    return cnt == -1;
}

char Stack::top()
{
    return array[cnt];
}

char Stack::pop()
{
    if (!isEmpty())
        return array[cnt--];
    return '$';
}

void Stack::push(char op)
{
    array[++cnt] = op;
}


