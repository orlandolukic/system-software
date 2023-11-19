#ifndef CONTENT_WRITER_ELEMENT_H_
#define CONTENT_WRITER_ELEMENT_H_

#include <string>

class ContentWriterElementSection
{
public:
	std::string *byte;
	ContentWriterElementSection* next;

	ContentWriterElementSection(std::string *byte);
	~ContentWriterElementSection();
};

#endif
