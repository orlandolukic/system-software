#include <iostream>
#include <string>
#include <sstream>
#include <contentwriterrel.h>

std::string ContentWriterRel::PC_16 = std::string("R_16");
std::string ContentWriterRel::PC_REL_16 = std::string("R_PC16");
std::string ContentWriterRel::SymTbl = std::string("R_SYMTBL");

/**
 * Creates content buffer.
 */
ContentWriterRel::ContentWriterRel(std::string section)
{
	this->section = section;
	this->first = nullptr;
	this->last = nullptr;
	this->elements = 0;
}

/**
 * Deletes all content within buffer.
 */
ContentWriterRel::~ContentWriterRel()
{
	ContentWriterElementRel* elem = first, *t = nullptr;
	while( elem != nullptr )
	{
		t = elem;
		elem = elem->next;
		delete t;
	};
	first = last = nullptr;
}

/**
 * Adds content within buffer.
 */
void ContentWriterRel::addContent(int offsetval, std::string offset, std::string type, int value, bool symbol)
{
	ContentWriterElementRel* el = new ContentWriterElementRel(offsetval, offset, type, value, symbol);

	if ( first == nullptr )
		first = last = el;
	else
		last = last->next = el;
}

/**
 * Print content for relocation.
 */
void ContentWriterRel::getContent(std::string** str)
{
	ContentWriterElementRel* el = first;
	std::stringstream ss;
	ss << "# .rel.";
	ss << section;
	ss << "\n";
	ss << "# offset\ttype\tval\tsym/sec\n";
	if ( el != nullptr )
	{
		while( el != nullptr )
		{
			ss << "  0x";
			ss << el->offset;
			ss << "(" << el->offsetval << ")";
			ss << "\t";
			ss << el->type;
			ss << "\t";
			ss << el->reff;
			ss << "\t";
			ss << (el->symbol ? "symbol" : "section");
			ss << "\n";
			el = el->next;
		};
	} else
	{
		ss << "# Relocation table empty." << std::endl;
	};
	*str = new std::string(ss.str());
}

/**
 * Adds content within buffer.
 */
void ContentWriterRel::addContent(unsigned int pos, std::string content, bool symbol)
{
	if ( pos >= elements )
			return;
	return;
	//ContentWriterElementRel* el = new ContentWriterElementRel(content);
	unsigned int i = 0;
	ContentWriterElementRel *curr, *last;
	while( i<pos )
	{
		last = curr;
		curr = curr->next;
	};
}

/**
 * Gets counter.
 */
unsigned int ContentWriterRel::getCouter()
{
	return elements;
}
