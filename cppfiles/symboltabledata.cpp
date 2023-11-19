#include "../hfiles/symboltabledata.h"

/**
 * Constructor for symbol table's data.
 */
SymbolTableData::SymbolTableData( int id, std::string name, std::string section, int value, bool visible, unsigned short rights )
{
	this->id = id;
	this->name = name;
	this->section = section;
	this->value = value;
	this->visible = visible;
	this->rights = rights;
}

SymbolTableData::~SymbolTableData() {}

int SymbolTableData::getID() { return id; }

std::string SymbolTableData::getName() { return name; }
std::string SymbolTableData::getSection() { return section; }
int SymbolTableData::getValue() { return value; }
bool SymbolTableData::isVisible() { return visible; }

void SymbolTableData::setName(std::string str) { name = str; }
void SymbolTableData::setSection(std::string str) { section = str; }
void SymbolTableData::setValue(int val) { value = val; }
void SymbolTableData::setVisible(bool vis) { visible = vis; }
unsigned short SymbolTableData::getAccessRights() { return rights; }




