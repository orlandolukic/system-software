#include <iostream>
#include <stack>

#include "../hfiles/functions.h"
#include "../hfiles/messagelist.h"
#include "../hfiles/programdata.h"
#include "../hfiles/symboltable.h"
#include "../hfiles/util.h"

extern ProgramData *progdata = new ProgramData();
extern SymbolTable *table = new SymbolTable();
MessageList *messagelist = MessageList::getInstance();

#include<iostream>
#include<string>

int main(int argc, const char* argv[])
{
	try {
		// Parse entry parameters.
		Util::parseStartArgs(argc, argv);

		// Prepare assmebler.
		Util::prepare();

		// Goes first time through input file.
		Util::makeFirstPass();

		// Prepares for second pass.
		Util::prepareForSecondPass();

		// Makes second pass through assembly file.
		Util::makeSecondPass();

		// Creates file.
		Util::exportFile();

		// Deletes all global data.
		Util::destructGlobalData();
	} catch( std::exception& e )
	{
		std::cout << "\n*** EXCEPTION ***\n" << e.what();
	};

	return 0;
}



