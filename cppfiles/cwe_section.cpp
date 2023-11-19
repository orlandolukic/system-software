#include "../hfiles/cwe_section.h"

ContentWriterElementSection::ContentWriterElementSection(std::string *byte)
{
	this->byte = byte;
	this->next = nullptr;
}

ContentWriterElementSection::~ContentWriterElementSection() {}



