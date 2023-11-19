#include "../hfiles/messageclass.h"

Message::Message( std::string *str ) : _mssg(str), next(nullptr) {}

Message::~Message()
{
	delete _mssg;
}

/**
 * Gets message.
 */
std::string* Message::getMessage()
{
	return _mssg;
}

/**
 * Gets next element.
 */
Message* Message::getNext()
{
	return next;
}




