#ifndef _CONTENT_WRITER_REL_H_
#define _CONTENT_WRITER_REL_H_

#include <cwe_rel.h>

class ContentWriterRel
{
private:
	std::string section;
	ContentWriterElementRel *first, *last;
	unsigned int elements;

public:
	static std::string PC_16;
	static std::string PC_REL_16;
	static std::string SymTbl;

	ContentWriterRel(std::string section);
	~ContentWriterRel();

	void addContent(int offsetval, std::string offset, std::string type, int value, bool symbol);
	void addContent(unsigned int pos, std::string content, bool symbol);
	void getContent(std::string** str);
	unsigned int getCouter();
};

#endif
