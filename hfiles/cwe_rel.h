#ifndef CONTENT_WRITER_ELEMENT_REL_H_
#define CONTENT_WRITER_ELEMENT_REL_H_

#include <string>

class ContentWriterElementRel
{
public:
	int offsetval;
	std::string offset,
	            type;
	int reff;
	bool symbol;
	ContentWriterElementRel* next;

	ContentWriterElementRel(int offsetval, std::string offset, std::string type, int reff, bool symbol);
	~ContentWriterElementRel();
};

#endif
