#ifndef SYMBOLTABLEDATA_H_
#define SYMBOLTABLEDATA_H_

#include <string>

class SymbolTableData
{
private:
	int id;
	std::string name;
	std::string section;
	int value;
	bool visible;
	unsigned short rights;


public:
	SymbolTableData( int id, std::string name, std::string section, int value, bool visible, unsigned short rights );
	~SymbolTableData();

	// Getters.
	int getID();
	std::string getName();
	std::string getSection();
	int getValue();
	bool isVisible();
	unsigned short getAccessRights();

	// Setters.
	void setName(std::string);
	void setSection(std::string);
	void setValue(int);
	void setVisible(bool);
};

#endif /* SYMBOLTABLEDATA_H_ */
