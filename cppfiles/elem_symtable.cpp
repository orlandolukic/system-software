#include "../hfiles/elem_symtable.h"
#include <iostream>

ElemSymtable::ElemSymtable( SymbolTableData *d )
{
	this->data = d;
	this->next = nullptr;
	this->contentWriter = nullptr;
	this->contentWriterRel = nullptr;
	this->isSection = false;
}

/*
 * Creates symbol table element with content writers.
 */
ElemSymtable::ElemSymtable( SymbolTableData *d, bool isSection )
{
	this->data = d;
	this->contentWriter = isSection ? new ContentWriter(d->getSection()) : nullptr;
	this->contentWriterRel = isSection ? new ContentWriterRel(d->getSection()) : nullptr;
	this->next = nullptr;
	this->isSection = isSection;
}

/**
 * Destroys symbol table element.
 */
ElemSymtable::~ElemSymtable()
{
	delete data;
	if ( contentWriter != nullptr )
		delete contentWriter;
	if ( contentWriterRel != nullptr )
		delete contentWriterRel;
}

/**
 * Checks if element has content writers.
 */
bool ElemSymtable::hasContentWriter()
{
	return contentWriter != nullptr && contentWriterRel != nullptr;
}


