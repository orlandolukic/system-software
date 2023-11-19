#include "../hfiles/symboltable.h"

#include <iostream>
#include <bits/stdc++.h>
#include <sstream>
#include <vector>

#include "../hfiles/elem_symtable.h"
#include "../hfiles/util.h"
#include "functions.h"

int SymbolTable::ID = 0;
unsigned short SymbolTable::READ = 1;
unsigned short SymbolTable::WRITE = 2;
unsigned short SymbolTable::EXECUTE = 4;

SymbolTable::SymbolTable() : locationCounter(0), first(nullptr), last(nullptr), cwe(nullptr), cwer(nullptr) {}

/**
 * Destroy list.
 */
SymbolTable::~SymbolTable()
{
	ElemSymtable *curr = first, *f;
	while( curr != nullptr )
	{
		f = curr;
		curr = curr->next;
		delete f;
	};
}

/**
 * Resets location counter.
 */
void SymbolTable::resetLocationCounter()
{
	locationCounter = 0;
}

/**
 * Checks if symbol exists in the table.
 */
bool SymbolTable::exists(std::string name)
{
	if ( first == nullptr )
		return false;

	ElemSymtable *curr = first;
	while( curr != nullptr )
	{
		if ( curr->data->getName().compare(name) == 0 )
			return true;
		curr = curr->next;
	};

	return false;
}

/**
 * Checks if symbol is visible.
 */
bool SymbolTable::isSymbolVisible( std::string name )
{
	ElemSymtable *curr = first;
	while( curr != nullptr )
	{
		if ( curr->data->getName().compare(name) == 0 )
			return curr->data->isVisible();
		curr = curr->next;
	};

	return false;
}

/**
 * Checks if defined symbol is in current section.
 */
bool SymbolTable::inCurrentSection( std::string name )
{
	ElemSymtable *curr = first;
	while( curr != nullptr )
	{
		if ( curr->data->getName().compare(name) == 0 )
			return curr->data->getSection().compare(this->section) == 0;
		curr = curr->next;
	};
	return false;
}

/**
 * Sets symobl sym as global symbol.
 */
void SymbolTable::setGlobal( std::string sym )
{
	if ( !exists(sym) )
		return;

	ElemSymtable *curr = first;
	while( curr != nullptr )
	{
		if ( curr->data->getName().compare(sym) == 0 )
		{
			curr->data->setVisible(true);
			break;
		};
		curr = curr->next;
	};
}

/**
 * Adds symbol in the table.
 *
 * @throws exception if symbol already exists.
 */
void SymbolTable::addSymbol( std::string name )
{
	addSymbol(name, 0, false);
}

/**
 * Adds symbol as global in undefined section.
 */
void SymbolTable::addSymbolUndefinedSection( std::string sym )
{
	SymbolTableData *data = new SymbolTableData( ID++, sym, "UND", 0, true, 0 );
	ElemSymtable *el = new ElemSymtable(data, false);

	// Link list.
	if ( first == nullptr )
		first = last = el;
	else
		last = last->next = el;
}

/**
 * Adds symbol/section in the table.
 *
 * @throws exception if symbol already exists.
 */
void SymbolTable::addSymbol( std::string name, unsigned short rights, bool isSection )
{
	std::stringstream *str = new std::stringstream();
	*str << "Symbol " << name << " already exists in the symbol table.";

	if ( exists(name) )
		Util::exitProgram( str->str(), -1 );

	SymbolTableData *data = new SymbolTableData( ID++, name, section, locationCounter, false, rights );
	ElemSymtable *el = new ElemSymtable(data, isSection);

	// Link list.
	if ( first == nullptr )
		first = last = el;
	else
		last = last->next = el;
}

/**
 * Adds symbol by .equ directive.
 */
void SymbolTable::addEquSymbol( std::string name, int value )
{
	std::stringstream *str = new std::stringstream();
	*str << "Symbol " << name << " already exists in the symbol table.";

	if ( exists(name) )
		Util::exitProgram( str->str(), -1 );

	SymbolTableData *data = new SymbolTableData( ID++, name, section, value, false, 0 );
	ElemSymtable *el = new ElemSymtable(data);

	// Link list.
	if ( first == nullptr )
		first = last = el;
	else
		last = last->next = el;
}

/**
 * Gets number of symbols.
 */
int SymbolTable::symbolNumber()
{
	return ID;
}

/**
 * Adds byte info buffer.
 */
void SymbolTable::addByteIntoBuffer(int byte, int type)
{
	//std::cout << "entered!"<< std::endl;
	if ( cwe == nullptr || cwer == nullptr )
		Util::exitProgram("Cannot add bytes into nullptr buffer.", -1);

	int first, second;
	std::stringstream s1, s2;
	s1 << std::setfill ('0') << std::setw(2) << std::hex;
	s2 << std::setfill ('0') << std::setw(2) << std::hex;
	std::string *str1, *str2;
	if ( type == 2 )
	{
		first = byte;
		first &= 0xFF;
		second = byte >> 8;
		second &= 0xFF;

		s1 << first;
		s2 << second;
		str1 = new std::string( s1.str() );
		str2 = new std::string( s2.str() );
		toUpperCase(str1); toUpperCase(str2);
		cwe->addContent(str1);
		cwe->addContent(str2);

		//std::cout << "first=" << *str1  << " second=" << *str2;

	} else if (type == 1)
	{
		first = byte;
		first &= 0xFF;
		s1 << first;
		str1 = new std::string( s1.str() );
		toUpperCase(str1);
		cwe->addContent(str1);
		//std::cout << "first=" << *str1 << std::endl;
	};
}

/**
 * Sets content writer for current section.
 */
bool SymbolTable::setContentWriter()
{
	ElemSymtable *curr = first;
	while( curr != nullptr )
	{
		if ( curr->isSection && curr->data->getSection().compare(section) == 0 )
		{
			cwe = curr->contentWriter;
			cwer = curr->contentWriterRel;
			return true;
		};
		curr = curr->next;
	};
	return false;
}

/**
 * Gets current section.
 */
std::string SymbolTable::getSection()
{
	return section;
}

int SymbolTable::getLocationCounter()
{
	return locationCounter;
}

/**
 * Gets value of the symbol.
 */
int SymbolTable::getSymbolValue( std::string name )
{
	ElemSymtable *curr = first;
	while( curr != nullptr )
	{
		if ( curr->data->getName().compare(name) == 0 )
			return curr->data->getValue();
		curr = curr->next;
	};
	return -1;
}

/**
 * Sets current section.
 */
void SymbolTable::setSection( std::string name )
{
	this->section = name;
	this->locationCounter = 0;
}

/**
 * Increments location counter.
 */
void SymbolTable::incrementCounter( int step )
{
	this->locationCounter += step;
}

/**
 * Gets current content writer for bytes.
 */
ContentWriter* SymbolTable::getContentWriter()
{
	return cwe;
}

/**
 * Gets current content writer for relocation.
 */
ContentWriterRel* SymbolTable::getContentWriterRelocation()
{
	return cwer;
}

/**
 * Adds relocation entry based on offset.
 */
void SymbolTable::addRelocationEntry(std::string label, std::string type, int offsetval)
{
	// Checks if symbol exists in the table.
	if ( !exists(label) )
	{
		std::string f = "Symbol '"; f += label; f += "' is not defined!";
		Util::exitProgram(f, 1);
	};

	if ( cwer == nullptr || cwe == nullptr )
		throw new std::runtime_error("Content writer(s) are not initialized!");
	std::string offset;
	if ( offsetval == -1 )
		offsetval = cwe->getCounter();
	else
		offsetval = cwe->getCounter() + offsetval;
	int index = -1;
	char t[5];
	sprintf( t, "%.4X", offsetval );
	offset = std::string(t);

	bool searchForSection = false;
	std::string section;
	ElemSymtable *curr = first;
	while( curr != nullptr )
	{
		if ( curr->data->getName().compare(label) == 0 )
		{
			if ( !curr->data->isVisible() )
			{
				section = curr->data->getSection();
				searchForSection = true;
				index = curr->data->getID();
			} else
				index = curr->data->getID();
		};
		curr = curr->next;
	};
	if ( searchForSection )
	{
		index = -1;
		curr = first;
		while( curr != nullptr )
		{
			if ( curr->isSection && curr->data->getSection().compare(section) == 0 )
			{
				index = curr->data->getID();
				break;
			};
			curr = curr->next;
		};
	};

	if ( index == -1 )
	{
		std::string f("Could not make relocation entry for symbol ");
		f += label;
		f += ".";
		Util::exitProgram(f, 4);
	};

	cwer->addContent(offsetval, offset, type, index, !searchForSection);
}

/**
 * Adds relocation entry into current content writer for relocation.
 */
void SymbolTable::addRelocationEntry(std::string label, std::string type)
{
	addRelocationEntry(label, type, -1);
}

/**
 * Gets table represented as string.
 */
std::string SymbolTable::getSymbolTableAsString()
{
	std::stringstream os;
	os << "# Symbol Table\n";
	os << "# Access rights: [R-read, W-write, X-execute]\n";
	os << "# \n";
	os << "# id\tname\t\tsec\tval\tvis\tis_sec\tRWX\n";

	if ( ID == 0 )
	{
		os << "# Symbol table empty.";
		return os.str();
	};

	ElemSymtable *curr = first;
	std::string access;
	unsigned short accessRights;
	while( curr != nullptr )
	{
		accessRights = curr->data->getAccessRights();
		if ( accessRights > 0 )
		{
			bool canRead = (accessRights & SymbolTable::READ) == SymbolTable::READ;
			bool canWrite = (accessRights & SymbolTable::WRITE) == SymbolTable::WRITE;
			bool canExecute = (accessRights & SymbolTable::EXECUTE) == SymbolTable::EXECUTE;
			//std::cout << accessRights << " \n";
			char str[4];
			sprintf( str, "%c%c%c", canRead ? 'R' : '-', canWrite ? 'W' : '-', canExecute ? 'X' : '-' );
			str[3] = '\0';
			access = std::string(str);
		} else
			access = "---";

		os << "  " << curr->data->getID() << "\t";
		os << curr->data->getName() << "\t\t";
		os << curr->data->getSection() << "\t";
		os << curr->data->getValue() << "\t";
		os << ( !curr->data->isVisible() ? "local" : "global" ) << "\t";
		os << ( accessRights > 0 ? "1" : "0" ) << "\t";
		os << access;
		os << std::endl;

		curr = curr->next;
	};

	// Print relocation and content.
	curr = first;
	std::string *str;
	while( curr != nullptr )
	{
		if ( curr->isSection )
		{
			os << std::endl;
			curr->contentWriter->getContent(&str);
			os << *str << std::endl << std::endl;
			curr->contentWriterRel->getContent(&str);
			os << *str << std::endl;
			delete str;
		};
		curr = curr->next;
	};

	return os.str();
}


