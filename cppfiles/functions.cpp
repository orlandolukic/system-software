#include <iostream>
#include <stdio.h>
#include <string>
#include <regex>
#include <sstream>
#include <bits/stdc++.h>
#include "../hfiles/functions.h"
#include "../hfiles/symboltable.h"
#include "../hfiles/util.h"
#include "../hfiles/programdata.h"
#include "../hfiles/stack.h"
#include "../hfiles/symboltable.h"
#include "../hfiles/messagelist.h"


extern SymbolTable *table;
extern ProgramData *progdata;

extern std::regex REGEX_NUMBER,
				  REGEX_NEGATIVE_NUMBER,
				  REGEX_COMMENT,
				  REGEX_BYTE,
				  REGEX_WORD,
				  REGEX_FIND_ONLY_NUMBER,
				  REGEX_FIND_ONLY_LABEL,
				  REGEX_REMOVE_BLANKS_END,
				  REGEX_ALONE_LABEL,
				  REGEX_LABEL,
				  REGEX_DIRECTIVE_NAME;

/**
 * Converts character to lower case.
 */
char tolower(char in) {
  if(in <= 'Z' && in >= 'A')
    return in - ('Z' - 'z');
  return in;
}

char toupper_change(char in)
{
	if(in <= 'z' && in >= 'a')
	    return in + ('A' - 'a');
	return in;
}

/**
 * Sets string to lower case.
 */
void toLowerCase(std::string* str)
{
	for (int i=0; i<str->size(); i++)
		(*str)[i] = tolower((*str)[i]);
}

/**
 * Converts string into uppercase.
 */
void toUpperCase(std::string* str)
{
	for (int i=0; i<str->size(); i++)
		(*str)[i] = toupper_change((*str)[i]);
}


/**
 * Removes blanks from string.
 *
 */
std::string removeBlanksStart(std::string str)
{
	int sp = 0;
	bool it = false;
	for (int i=0; i < str.length() && !it; i++)
	{
		// Checks if selected character is tab or blank space.
		if ( str[i] == '\t' || str[i] == ' ' )
			sp++;
		else
			it = true;

	};
	return str.substr( sp, str.length() );
}

/**
 * Removes blanks in string.
 */
void removeBlanksInString(std::string** str)
{
	std::regex reg_blanks("\\s*");
	std::string *s = new std::string();
	std::string *g = *str;
	*s = regex_replace(**str, reg_blanks, "");
	*str = s;
	delete g;
}

/**
 * Removes comment from scanned line.
 */
std::string removeComment(std::string str)
{
	return regex_replace(str, REGEX_COMMENT, "");
}

/**
 * Checks if number is regular.
 */
bool checkNumberRegularity(int number, int bw)
{
	if ( bw == 1 )
		return number <= 127 && number >= -128;
	else if ( bw == 2 )
		return number < (2<<15) && number > -(2<<15)+1;
	return false;
}

/**
 * Checks if number is regular.
 */
unsigned int getNumberSpace(int number)
{
	if ( number <= 127 && number >= -128 )
		return 1;
	else if ( number < (2<<15) && number > -(2<<15)+1 )
		return 2;
	return 0;
}

/**
 * Gets parsed instruction as array of string.
 */
unsigned int getParsedInstruction(std::string line, std::string** buff)
{
	std::smatch test;
	std::string instruction, fes, first, second;
	bool found = false, dofirst = true, isOneOperandInstr = false;
	int size = 1;

	regex_search(line, test, std::regex("\\s*([a-z]{2,4}|[A-Z]{2,4})[bw]?"));
	instruction = test[0];
	instruction = regex_replace( instruction, std::regex("\\s*"), "");

	// Get info about instruction.
	fes = regex_replace( line, std::regex(instruction), "" );
	fes = regex_replace(fes, std::regex("\\s*"), "");
	if ( fes != "" )
	{
		for (unsigned int i=0; i<fes.length(); i++)
			if ( fes[i] == ',' )
				found = true;
		first = "";
		second = "";
		for (unsigned int i=0; i<fes.length(); i++)
			if ( fes[i] != ',' )
				if ( dofirst )
					first += fes[i];
				else
					second += fes[i];
			else
				dofirst = false;

		size += found ? 2 : 1;
	};

	*buff = new std::string[size];
	(*buff)[0] = instruction;
	if ( size-1 >= 1 )
		(*buff)[1] = first;
	if ( size-1 == 2 )
		(*buff)[2] = second;

	return size;
}

/**
 * Processes directive.
 */
void processDirectiveFirstPass(int directive, std::string line)
{
	MessageList* messagelist = MessageList::getInstance();
	std::smatch test;
	std::string g;
	switch( directive )
	{
	// global & extern.
	case 0:
	case 1:
	{
		// Ignore.
		break;
	}

	// Equivalent directive.
	case 2:
	{
		std::string restequ = regex_replace( line, REGEX_ALONE_LABEL, "" );
		restequ = regex_replace( restequ, REGEX_DIRECTIVE_NAME, "" );
		std::string *equ = new std::string(restequ);
		removeBlanksInString(&equ);
		//std::cout << *equ << std::endl;

		regex_search( *equ, test, std::regex("^[a-zA-Z_][a-zA-Z_0-9]*") );
		std::string label = test[0];
		//std::cout << label << std::endl;

		std::string rest = regex_replace(*equ, std::regex("[a-zA-Z_][a-zA-Z0-9_]*,"), "");
		//std::cout << rest << std::endl;

		std::string found;
		std::string replace;
		int val;
		while( regex_search( rest, test, std::regex("[a-zA-Z_][a-zA-Z0-9_]*") ) )
		{
			found = test[0];
			if ( table->exists(found) )
			{
				val = table->getSymbolValue(found);
				replace = std::to_string(val);
				rest = regex_replace(rest, std::regex(found), replace);
			} else
			{
				std::string f("Symbol ");
				f += found;
				f+= " is not defined!";
				Util::exitProgram(f, 1);
			};
		};

		char g[rest.length()];
		strcpy(g, rest.c_str());

		// Adds symbol into symbol table.
		table->addEquSymbol(label, expression(g));
		break;
	}

	// Text section.
	case 3:
	{
		table->setSection("text");
		table->addSymbol(".text", SymbolTable::EXECUTE | SymbolTable::READ, true );
		break;
	}

	// Data section.
	case 4:
	{
		table->setSection("data");
		table->addSymbol(".data", SymbolTable::READ | SymbolTable::WRITE, true );
		break;
	}

	// BSS section.
	case 5:
	{
		table->setSection("bss");
		table->addSymbol(".bss", SymbolTable::READ | SymbolTable::WRITE, true );
		break;
	}

	// Section.
	case 6:
	{
		std::string str = regex_replace(line, REGEX_ALONE_LABEL, "");
		str = regex_replace( str, std::regex("\\s*.section\\s*"), "" );
		// "\\s*,\\s*\"(R|-)(W|-)(X|-)\" "
		std::string secname = regex_replace( str, std::regex("\\s*,\\s*[a-zA-Z\"-]*"), "" );
		std::string access;
		unsigned short accessNum = 0;
		if ( regex_search( str, test, std::regex("(R|-)(W|-)(X|-)") ) )
		{
			access = test[0];
			//std::cout << secname << "\n";
			accessNum |= access[0] == 'R' ? SymbolTable::READ : 0;
			accessNum |= access[1] == 'W' ? SymbolTable::WRITE : 0;
			accessNum |= access[2] == 'X' ? SymbolTable::EXECUTE : 0;
		} else
		{
			accessNum = SymbolTable::READ | SymbolTable::WRITE;
		};

		table->setSection(secname);
		table->addSymbol(secname, accessNum, true);
		break;
	}

	// Byte & word.
	case 7:
	case 8:
	{
		std::regex* reg = nullptr;
		int step = directive == 7 ? 1 : 2;
		if ( directive == 7 )
			reg = &REGEX_BYTE;
		else
			reg = &REGEX_WORD;

		//std::cout << "LINE::" << line << std::endl;
		std::string str = regex_replace( line, REGEX_DIRECTIVE_NAME, "" );
		str = removeBlanksStart(str);
		//std::cout << "str::" << str << std::endl;

		if ( !regex_match( str, test, *reg ) )
				Util::exitProgram("Byte/Word directive parse error.", -1);

		//std::cout << "DIRECTIVE REST: " << test[0] << std::endl;
		std::string *numbers = new std::string(test[0]);
		removeBlanksInString(&numbers);
		std::smatch sd;
		std::string searchstr = *numbers;
		unsigned int parser = 0;
		bool found = false;
		int num;
		char* d;

		// Determine which regex should be used.
		if ( regex_search( searchstr, sd, REGEX_FIND_ONLY_NUMBER ) )
			reg = &REGEX_FIND_ONLY_NUMBER;
		else
			reg = &REGEX_FIND_ONLY_LABEL;

		// Iterate through the string.
		while ( regex_search( searchstr, sd, *reg ) )
		{
			//std::cout << searchstr << std::endl;
			if ( reg == &REGEX_FIND_ONLY_LABEL )
			{
				// Check if byte directive is current.
				if ( directive == 7 )
					Util::exitProgram("Byte directive does not support labels.", 4);

				std::string label = sd[0];

				num = !table->exists(label) ? 0 : table->getSymbolValue(label);
			} else
				// Get number.
				try {
					num = stoi( sd[0] );
				} catch( std::exception& e )
				{
					std::string mssg("Number "); mssg += sd[0]; mssg += " could not be parsed.";
					Util::exitProgram(mssg, 5);
				};

			if ( !checkNumberRegularity( num, step ) )
			{
				char*f;
				sprintf(f, "%d", num);
				std::string *mssg = new std::string("Number ");
				*mssg += f;
				*mssg += " is out of range.";
				messagelist->addMessage(mssg);
			};

			// Increment location counter for symbol table.
			table->incrementCounter(step);

			// Scan another number.
			found = false;
			parser = 0;
			for (unsigned int i=parser; i<searchstr.size() && !found; i++)
				if ( searchstr[i] != ',' )
					parser++;
				else
					found = true;
			parser++;
			if ( parser > searchstr.size() )
				break;
			searchstr = searchstr.substr(parser, searchstr.size()-parser);

			if ( regex_search( searchstr, sd, REGEX_FIND_ONLY_NUMBER ) )
				reg = &REGEX_FIND_ONLY_NUMBER;
			else
				reg = &REGEX_FIND_ONLY_LABEL;
		};

		// Check warning messages.
		if ( messagelist->getMessageNumber() > 0 )
			Util::exitProgram(1);

		break;
	}

	// Align directive.
	case 9:
	{
		if ( regex_search( line, test, REGEX_NEGATIVE_NUMBER ) )
			Util::exitProgram("Parse error on align directive. Format: \".align positive_number\"", 1);
		if ( !regex_search( line, test, REGEX_NUMBER ) )
			Util::exitProgram("Parse error on align directive. Format: .align positive_number", 1);
		int number = stoi( test[0] );
		int checknum = 1 << number;
		for (int i=table->getLocationCounter(); true; i++)
			if ( i % checknum == 0 )
				break;
			else
				table->incrementCounter(1);
		break;
	}


	// Skip directive.
	case 10:
	{
		regex_search( line, test, REGEX_NUMBER );
		table->incrementCounter( stoi(test[0]) );
		break;
	}

	// End directive.
	case 11:
		progdata->setEnd();
		break;

	// asciz & ascii directives.
	case 14:
	case 15:
	{
		bool incr = directive == 14;
		std::string str = regex_replace( line, std::regex("\\s*.asci(i|z)\\s+"), "" );
		str = regex_replace( str, REGEX_REMOVE_BLANKS_END, "" );
		str = regex_replace( str, REGEX_ALONE_LABEL, "" );
		table->incrementCounter( str.size() - 2 + (incr ? 1 : 0) );
		break;
	}

	}
}

/**
 * Processes directive in second pass.
 */
void processDirectiveSecondPass(int directive, std::string line)
{
	MessageList* messagelist = MessageList::getInstance();
	std::smatch test;
	std::string g;
	switch( directive )
	{
	// global & extern.
	case 0:
	case 1:
	{
		// Set variable as global.
		std::string rest = regex_replace(line, REGEX_DIRECTIVE_NAME, ""), temp;
		std::smatch test;
		rest = removeBlanksStart(rest);
		rest = regex_replace( rest, std::regex("\\s*"), "" );
		//std::cout << rest << std::endl;

		while( regex_search( rest, test, REGEX_LABEL ) )
		{
			temp = test[0];
			//std::cout << "here: " << temp << std::endl;
			if ( table->exists(temp) )
			{
				if ( directive == 1 )	// .extern
				{
					std::string f(""); f += "Symbol '"; f += rest; f+= "' is local symbol. Could not be set as extern.";
					Util::exitProgram(f, 4);
				}
				table->setGlobal(temp);
			} else
			{
				if ( directive == 0 ) // .global
				{
					std::string f(""); f += "Symbol '"; f += rest; f+= "' could not be set to global. Undefined symbol.";
					Util::exitProgram(f, 4);
				};
				table->addSymbolUndefinedSection(temp);
			};
			rest = regex_replace( rest, std::regex(temp), "" );
			rest = regex_replace( rest, std::regex("^,"), "");
		};
		break;
	}

	// Equivalent directive.
	case 2:
	{
		std::string restequ = regex_replace( line, REGEX_ALONE_LABEL, "" );
		restequ = regex_replace( restequ, REGEX_DIRECTIVE_NAME, "" );
		std::string *equ = new std::string(restequ);
		removeBlanksInString(&equ);
		//std::cout << *equ << std::endl;

		regex_search( *equ, test, std::regex("^[a-zA-Z_][a-zA-Z_0-9]*") );
		std::string label = test[0];
		//std::cout << label << std::endl;

		std::string rest = regex_replace(*equ, std::regex("[a-zA-Z_][a-zA-Z0-9_]*,"), "");
		//std::cout << rest << std::endl;

		std::string found;
		std::string replace;
		int val;
		while( regex_search( rest, test, std::regex("[a-zA-Z_][a-zA-Z0-9_]*") ) )
		{
			found = test[0];
			//table->addRelocationEntry(found, ContentWriterRel::SymTbl);
			rest = regex_replace(rest, std::regex(found), replace);
		};
/*
		char g[rest.length()];
		strcpy(g, rest.c_str());
*/
		// Adds symbol into symbol table.
		//table->addEquSymbol(label, expression(g));
		break;
	}

	// Text section.
	case 3:
	{
		table->setSection("text");
		table->setContentWriter();
		break;
	}

	// Data section.
	case 4:
	{
		table->setSection("data");
		table->setContentWriter();
		break;
	}

	// BSS section.
	case 5:
	{
		table->setSection("bss");
		table->setContentWriter();
		break;
	}

	// Section.
	case 6:
	{
		std::string str = regex_replace(line, REGEX_ALONE_LABEL, "");
		str = regex_replace( str, std::regex("\\s*.section\\s*"), "" );
		std::string secname = regex_replace( str, std::regex("\\s*,\\s*[a-zA-Z\"-]*"), "" );

		// Sets section.
		table->setSection(secname);
		table->setContentWriter();
		break;
	}

	// Byte & word.
	case 7:
	case 8:
	{
		std::regex* reg = nullptr;
		int step = directive == 7 ? 1 : 2;
		if ( directive == 7 )
			reg = &REGEX_BYTE;
		else
			reg = &REGEX_WORD;

		//std::cout << "LINE::" << line << std::endl;
		std::string str = regex_replace( line, REGEX_DIRECTIVE_NAME, "" );
		str = removeBlanksStart(str);
		//std::cout << "str::" << str << std::endl;

		if ( !regex_match( str, test, *reg ) )
			Util::exitProgram(".byte/.word directive parse error.", -1);

		//std::cout << "DIRECTIVE REST: " << test[0] << std::endl;
		std::string *numbers = new std::string(test[0]);
		removeBlanksInString(&numbers);
		std::smatch sd;
		std::string searchstr = *numbers;
		unsigned int parser = 0;
		bool found = false;
		int num;
		char* d;

		// Determine which regex should be used.
		if ( regex_search( searchstr, sd, REGEX_FIND_ONLY_NUMBER ) )
			reg = &REGEX_FIND_ONLY_NUMBER;
		else
			reg = &REGEX_FIND_ONLY_LABEL;

		// Iterate through the string.
		while ( reg != nullptr && regex_search( searchstr, sd, *reg ) )
		{
			//std::cout << "heree" << searchstr << std::endl;
			//std::cout << searchstr << std::endl;
			if ( reg == &REGEX_FIND_ONLY_LABEL )
			{
				// Check if byte directive is current.
				if ( directive == 7 )
					Util::exitProgram("Byte directive does not support labels.", 4);

				std::string label = sd[0];

				if ( !table->exists(label) )
				{
					std::string f("Symbol "); f+= label; f+= " is not defined";
					Util::exitProgram(f, 4);
				};

				num = table->getSymbolValue(label);

				table->addRelocationEntry(label, ContentWriterRel::PC_16);
			} else
				// Get number.
				try {
					num = stoi( sd[0] );

				} catch( std::exception& e )
				{
					std::string mssg("Number "); mssg += sd[0]; mssg += " could not be parsed.";
					Util::exitProgram(mssg, 5);
				};

			//std::cout << num << std:: endl;

			if ( !checkNumberRegularity( num, step ) )
			{
				std::string *mssg = new std::string("Number");
				*mssg += num;
				*mssg += " is out of range.";
				Util::exitProgram(*mssg,-1);
			};

			// Append byte into buffer.
			table->addByteIntoBuffer(num, step);

			//std::cout << searchstr << std::endl;

			// Scan another number.
			found = false;
			parser = 0;
			for (unsigned int i=parser; i<searchstr.size() && !found; i++)
				if ( searchstr[i] != ',' )
					parser++;
				else
					found = true;
			parser++;
			//std::cout << (parser > searchstr.size() ? 1 : 0) << std::endl;
			if ( parser > searchstr.size() )
			{
				reg = nullptr;
				continue;
			};
			searchstr = searchstr.substr(parser, searchstr.size()-parser);

			if ( regex_search( searchstr, sd, REGEX_FIND_ONLY_NUMBER ) )
				reg = &REGEX_FIND_ONLY_NUMBER;
			else if ( regex_search( searchstr, sd, REGEX_FIND_ONLY_LABEL ) )
				reg = &REGEX_FIND_ONLY_LABEL;
			else
				reg = nullptr;
		};

		break;
	}

	// Align directive.
	case 9:
	{
		if ( regex_search( line, test, REGEX_NEGATIVE_NUMBER ) )
			Util::exitProgram("Parse error on align directive. Format: \".align positive_number\"", 1);
		if ( !regex_search( line, test, REGEX_NUMBER ) )
			Util::exitProgram("Parse error on align directive. Format: .align positive_number", 1);
		int number = stoi( test[0] );
		int checknum = 1 << number;
		bool f = false;
		for (unsigned int i=table->getContentWriter()->getCounter(); !f; i++)
			if ( i % checknum == 0 )
				f = true;
			else
			{
				table->incrementCounter(1);
				table->getContentWriter()->addContent(new std::string("00"));
			};
		break;
	}


	// Skip directive.
	case 10:
	{
		int num;
		regex_search( line, test, REGEX_NUMBER );
		num = stoi(test[0]);
		for ( unsigned int i=0; i<num; i++ )
			table->getContentWriter()->addContent(new std::string("00"));
		table->incrementCounter(num);
		break;
	}

	// End directive.
	case 11:
		progdata->setEnd();
		break;

	// asciz & ascii directives.
	case 14:
	case 15:
	{
		bool incr = directive == 14;
		std::string str = regex_replace( line, std::regex("\\s*.asci(i|z)\\s+"), "" );
		str = regex_replace( str, REGEX_REMOVE_BLANKS_END, "" );
		str = regex_replace( str, REGEX_ALONE_LABEL, "" );
		table->incrementCounter( str.size() - 2 + (incr ? 1 : 0) );
		str = regex_replace( str, std::regex("\""), "" );
		str = regex_replace( str, std::regex("\\\\n"), "\n" );
		str = regex_replace( str, std::regex("\\\\r"), "\r" );
		//std::cout << str << std::endl;

		char t[3];
		std::string *temp;
		for (unsigned int i=0; i<str.size(); i++)
		{
			sprintf(t, "%.2X", str[i]);
			temp = new std::string(t);
			table->getContentWriter()->addContent(temp);
			table->incrementCounter(1);
		};

		if ( incr )
		{
			table->incrementCounter(1);
			table->getContentWriter()->addContent(new std::string("00"));
		};
		break;
	}

	}
}

int counter = 0;

char peek(char* str)
{
    return *(str+counter);
}

char get(char* str)
{
    char f = *(str+counter);
    counter++;
    return f;
}

int number(char *str)
{
    int result = get(str) - '0';
    while (peek(str) >= '0' && peek(str) <= '9')
    {
        result = 10*result + get(str) - '0';
    }
    return result;
}

int factor(char* str)
{
    if (peek(str) >= '0' && peek(str) <= '9')
        return number(str);
    else if (peek(str) == '(')
    {
        get(str); // '('
        int result = expression(str);
        get(str); // ')'
        return result;
    }
    else if (peek(str) == '-')
    {
        get(str);
        return -factor(str);
    }
    return 0; // error
}

int term(char* str)
{
    int result = factor(str);
    while (peek(str) == '*' || peek(str) == '/')
        if (get(str) == '*')
            result *= factor(str);
        else
            result /= factor(str);
    return result;
}

int expression(char* str)
{
    int result = term(str);
    while (peek(str) == '+' || peek(str) == '-')
        if (get(str) == '+')
            result += term(str);
        else
            result -= term(str);
    return result;
}

/**
 * Gets bytes for number.
 */
unsigned int getBytesAsStringForNumber(std::string** first, std::string** second, short num)
{
	int f, s, type;
	std::stringstream s1, s2;
	s1 << std::setfill('0') << std::setw(2) << std::hex;
	s2 << std::setfill('0') << std::setw(2) << std::hex;
	std::string *str1, *str2;
	type = getNumberSpace(num);
	if ( type == 2 )
	{
		f = num;
		f &= 0xFF;
		s = num >> 8;
		s &= 0xFF;

		s1 << first;
		s2 << second;
		*first = new std::string(s1.str());
		*second = new std::string(s2.str());
		toUpperCase(*first); toUpperCase(*second);
		return 2;
	} else
	{
		f = num;
		f &= 0xFF;
		s1 << first;
		*first = new std::string(s1.str());
		toUpperCase(*first);
		*second = nullptr;
		return 1;
	};
}

/**
 * Gets bytes for number.
 */
unsigned int getBytesForNumber(unsigned short* first, unsigned short* second, short num, bool force)
{
	unsigned short f, s, type;
	type = getNumberSpace(num);
	if ( type == 2 || force )
	{
		f = num;
		f &= 0xFF;
		s = num >> 8;
		s &= 0xFF;

		*first = f;
		*second = s;

		return 2;
	} else
	{
		f = num;
		f &= 0xFF;
		*first = f;
		*second = 0;
		return 1;
	};
}





