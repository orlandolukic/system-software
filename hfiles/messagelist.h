#ifndef MESSAGELIST_H_
#define MESSAGELIST_H_

#include "messageclass.h"

class MessageList
{
private:
	static MessageList* _instance;

	Message *_first, *_last;
	unsigned int _elems;

	MessageList();
public:
	~MessageList();

	static MessageList* getInstance();

	void addMessage( std::string* );
	void removeMessage( std::string );
	void removeMessage( int );
	unsigned int getMessageNumber();

	friend std::ostream& operator<<(std::ostream& it, const MessageList& c);
};



#endif /* MESSAGELIST_H_ */
