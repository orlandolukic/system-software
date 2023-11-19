#include <cwe_rel.h>

ContentWriterElementRel::ContentWriterElementRel(int offsetval, std::string offset, std::string type, int reff, bool symbol)
{
	this->offsetval = offsetval;
	this->offset = offset;
	this->type = type;
	this->reff = reff;
	this->next = nullptr;
	this->symbol = symbol;
}

ContentWriterElementRel::~ContentWriterElementRel() {}


