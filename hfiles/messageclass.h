#ifndef MESSAGECLASS_H_
#define MESSAGECLASS_H_

#include <string>

class MessageList;

class Message
{
private:
	friend class MessageList;
	std::string* _mssg;
	Message* next;

public:
	Message(std::string*);
	~Message();

	Message* getNext();
	std::string* getMessage();
};



#endif /* MESSAGECLASS_H_ */
