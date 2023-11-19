#include "../hfiles/messagelist.h"

#include <iostream>

#include "../hfiles/messageclass.h"

MessageList* MessageList::_instance = nullptr;

/**
 * Creates one and only list.
 */
MessageList::MessageList()
{
	_first = nullptr;
	_last = nullptr;
	_elems = 0;
}

/**
 * Deletes all messages.
 */
MessageList::~MessageList()
{
	Message* elem = _first, *t;
	while( elem != nullptr )
	{
		t = elem;
		elem = elem->next;
		delete t;
	};
	_elems = 0;
}

/**
 * Override operator <<.
 */
std::ostream& operator<<(std::ostream& it, const MessageList& lst)
{
	// Check number of elements.
	if ( lst._elems == 0 )
		return it;
	it << "Warnings: \n";
	unsigned short f = 1;
	Message* elem = lst._first;
	while( elem != nullptr )
	{
		it << "W" << f << ": " << *elem->getMessage() << std::endl;
		f++;
		elem = elem->getNext();
	};
	return it;
}

/**
 * Adds message within the list.
 */
void MessageList::addMessage( std::string* str )
{
	if ( _first == nullptr )
		_first = _last = new Message(str);
	else
		_last = _last->next = new Message(str);
	_elems++;
}

/**
 * Gets instance of message list.
 */
MessageList* MessageList::getInstance()
{
	if ( _instance == nullptr )
		_instance = new MessageList();
	return _instance;
}

/**
 * Gets number of warning messages.
 */
unsigned int MessageList::getMessageNumber()
{
	return _elems;
}

/**
 * Deletes message by index.
 */
void MessageList::removeMessage(int index)
{
	if ( index < 0 || index >= _elems )
		return;

	Message* elem = _first, *last = nullptr;
	int i = 0;
	while( elem != nullptr )
	{
		if ( i == index )
		{
			if ( last == nullptr )
				_first = elem->next;
			else
			{
				last->next = elem->next;
				if ( elem == _last )
					_last = last;
			};
			delete elem;
			_elems--;
		};

		i++;
		last = elem;
		elem = elem->next;
	};
}

/**
 * Deletes message by content.
 */
void MessageList::removeMessage(std::string str)
{
	Message* elem = _first, *last = nullptr;
	int i = 0;
	while( elem != nullptr )
	{
		if ( elem->_mssg->compare(str) == 0 )
		{
			if ( last == nullptr )
				_first = elem->next;
			else
			{
				last->next = elem->next;
				if ( elem == _last )
					_last = last;
			};
			delete elem;
			_elems--;
		};

		i++;
		last = elem;
		elem = elem->next;
	};
}


