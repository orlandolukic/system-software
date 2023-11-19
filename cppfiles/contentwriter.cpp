#include <iostream>
#include <contentwriter.h>
#include <sstream>

/**
 * Creates content buffer.
 */
ContentWriter::ContentWriter(std::string section)
{
	this->section = section;
	this->first = nullptr;
	this->last = nullptr;
	this->elements = 0;
}

/**
 * Deletes all content within buffer.
 */
ContentWriter::~ContentWriter()
{
	ContentWriterElementSection* elem = first, *t = nullptr;
	while( elem != nullptr )
	{
		t = elem;
		elem = elem->next;
		delete t;
	};
	first = last = nullptr;
	elements = 0;
}

/**
 * Adds content within buffer.
 */
void ContentWriter::addContent(std::string *content)
{
	ContentWriterElementSection* el = new ContentWriterElementSection(content);
	elements++;
	if ( first == nullptr )
		first = last = el;
	else
		last = last->next = el;
}

/**
 * Adds content within buffer.
 */
void ContentWriter::addContent(unsigned int pos, std::string *content)
{
	if ( pos >= elements )
			return;
	return;
	ContentWriterElementSection* el = new ContentWriterElementSection(content);
	unsigned int i = 0;
	ContentWriterElementSection *curr, *last;
	while( i<pos )
	{
		last = curr;
		curr = curr->next;
	};
}

/**
 * Print content for section bytes.
 */
void ContentWriter::getContent(std::string** str)
{
	std::stringstream *stream = new std::stringstream();
	ContentWriterElementSection* el = first;
	*stream << "# .section " << section << std::endl;
	*stream << "# " << elements << "bytes" << std::endl;
	unsigned int del = 0;
	while( el != nullptr )
	{
		*stream << *(el->byte);
		*stream << " ";

		if ( del == 15 )
		{
			*stream << "\n";
			del = 0;
		} else
			del++;
		el = el->next;

	};
	*str = new std::string( stream->str() );
}

/**
 * Gets number of bytes.
 */
unsigned int ContentWriter::getCounter()
{
	return elements;
}
