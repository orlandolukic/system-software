#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include "elem_symtable.h"
#include "contentwriter.h"
#include "contentwriterrel.h"

class SymbolTable
{
private:
	static int ID;

	int locationCounter;
	std::string section;
	ContentWriterRel *cwer;
	ContentWriter *cwe;

	ElemSymtable *first, *last;
public:
	static unsigned short READ;
	static unsigned short WRITE;
	static unsigned short EXECUTE;

	SymbolTable();
	~SymbolTable();

	void addSymbol( std::string );
	void addSymbol( std::string, unsigned short, bool isSection );
	void addSymbolUndefinedSection( std::string );
	void addEquSymbol( std::string, int );
	void addByteIntoBuffer(int,int);
	void addRelocationEntry(std::string label, std::string type);
	void addRelocationEntry(std::string label, std::string type, int offsetval);

	// Getters.
	bool exists( std::string name );
	bool isSymbolVisible( std::string name );
	bool inCurrentSection( std::string name );
	int symbolNumber();
	int getSymbolValue( std::string name );
	std::string getSymbolTableAsString();
	int getLocationCounter();
	std::string getSection();
	ContentWriter* getContentWriter();
	ContentWriterRel* getContentWriterRelocation();

	// Setters.
	void setGlobal( std::string );
	void setSection( std::string );
	void incrementCounter(int step);
	bool setContentWriter();
	void resetLocationCounter();
};


#endif /* SYMBOLTABLE_H_ */
