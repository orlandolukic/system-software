#ifndef _CONTENTWRITER_H_
#define _CONTENTWRITER_H_

#include <cwe_section.h>

class ContentWriter
{
private:
	std::string section;
	ContentWriterElementSection *first, *last;
	unsigned int elements;

public:
	ContentWriter(std::string section);
	~ContentWriter();

	// Setters.
	void addContent(std::string *content);
	void addContent(unsigned int pos, std::string *content);

	// Getters.
	void getContent(std::string** str);
	unsigned int getCounter();

};

#endif
