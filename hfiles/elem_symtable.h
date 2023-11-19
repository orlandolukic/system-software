#ifndef ELEM_SYMTABLE_H_
#define ELEM_SYMTABLE_H_

#include <symboltabledata.h>
#include <contentwriter.h>
#include <contentwriterrel.h>

class ElemSymtable
{
public:
	ContentWriter *contentWriter;
	ContentWriterRel *contentWriterRel;
	SymbolTableData *data;
	ElemSymtable *next;
	bool isSection;

	ElemSymtable( SymbolTableData *d );
	ElemSymtable( SymbolTableData *d, bool isSection );
	~ElemSymtable();

	bool hasContentWriter();
};


#endif /* ELEM_SYMTABLE_H_ */
