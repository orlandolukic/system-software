#include <regex>
#include <iostream>
#include <string>

#include "../hfiles/util.h"
#include "../hfiles/directives.h"
#include "../hfiles/instructions.h"
#include "../hfiles/messagelist.h"
#include "../hfiles/symboltable.h"
#include "../hfiles/programdata.h"
#include "../hfiles/functions.h"

extern ProgramData *progdata;
extern SymbolTable *table;
extern MessageList* messagelist;
extern std::regex REGEX_DIRECTIVE_NAME,
				  REGEX_DIRECTIVE,
				  REGEX_COMMENT,
				  REGEX_ALONE_LABEL,
				  REGEX_INSTRUCTION_OP0,
				  REGEX_INSTRUCTION_OP1,
				  REGEX_INSTRUCTION_OP2,
				  REGEX_LABEL_DIRECTIVE,
				  REGEX_LABEL_INSTRUCTION,
				  REGEX_EMPTY_LINE;


int Util::line = 0;

/**
 * Parse start args.
 */
void Util::parseStartArgs(int argc, const char* argv[])
{
	if ( argc < 3 )
		exitProgram("Program could not start because of insufficient arguments.", -1);

	progdata->inputfilename = argv[1];
	progdata->outputfilename = argv[3];
}

void Util::exitProgram(int code)
{
	// Write all warnings.
	std::cout << *messagelist;

	// Delete all data.
	progdata->error = true;
	destructGlobalData();
	exit(code);
}

/**
 * Exits program.
 */
void Util::exitProgram(std::string mssg, int code)
{
	std::cout << "===============================================================" << std::endl;
	std::cout << "\t\t*** ERROR OCCURED ***" << std::endl;
	std::cout << "===============================================================" << std::endl;

	// Write all warnings.
	std::cout << *messagelist;

	if ( mssg.compare("") != 0 )
	{
		std::cout << mssg << std::endl;
		std::cout << "Error line " << line << "." << std::endl;
	};

	// Delete all data.
	progdata->error = true;
	destructGlobalData();
	exit(code);
}

/**
 * Deletes all global variables.
 */
void Util::destructGlobalData()
{
	delete table;
	delete progdata;
	delete messagelist;
	Instructions::removeData();
}

void Util::exportFile()
{
	progdata->outputstr = new std::ofstream( progdata->outputfilename );
	*progdata->outputstr << table->getSymbolTableAsString();
}

/**
 * Updates current line number.
 */
void Util::updateLineNumber()
{
	line++;
}

/**
 * Gets current line number
 */
unsigned int Util::getLineNumber()
{
	return line;
}

/**
 * Prepare assembler for beginning.
 */
void Util::prepare()
{
	// Open stream for parsing.
	progdata->inputstr = new std::ifstream(progdata->inputfilename);

	// Set undifined section.
	table->setSection("UND");
	table->addSymbol("UND");

	// Prepares instructions.
	Instructions::prepare();
}

/**
 * First pass through assembly file.
 */
void Util::makeFirstPass()
{
	std::ifstream *file = progdata->inputstr;
	std::string ln,
				parsed;
	std::smatch match,
				match_label,
				match_instruction, m0, m1, m2,
				match_directive,
				match_lbl_dir,
				match_lbl_instr,
				test;

	// Sets current line to 0;
	line = 0;

	while( getline( *file, ln ) && !progdata->isEnd() )
	{
		// Updates line number;
		updateLineNumber();

		// Scans for comment.
		if ( regex_match( ln, match, REGEX_COMMENT ) )
			continue;

		// Check if scanned line is empty.
		if ( regex_match( ln, match, REGEX_EMPTY_LINE ) )
			continue;

		// Remove comment if exists.
		ln = removeComment(ln);
		ln = regex_replace( ln, std::regex("\\s*$"), "" );

		// Searches for directive.
		bool isOnlyLabel = regex_match( ln, match_label, REGEX_ALONE_LABEL );
		bool isOnlyDirective = regex_match( ln, match_directive, REGEX_DIRECTIVE );
		bool op2 = regex_match( ln, m2, REGEX_INSTRUCTION_OP2 );
		bool op1 = regex_match( ln, m1, REGEX_INSTRUCTION_OP1 );
		bool op0 = regex_match( ln, m0, REGEX_INSTRUCTION_OP0 );
		bool isOnlyInstruction = op0 || op1 || op2;
		match_instruction = op2 ? m2 : (op1 ? m1 : m0);

		bool isLabelAndDirective = regex_match( ln, match_lbl_dir, REGEX_LABEL_DIRECTIVE );
		bool isLabelAndInstruction = regex_match( ln, match_lbl_instr, REGEX_LABEL_INSTRUCTION );

		// Perform check.
		if ( isOnlyLabel )
		{
			std::string label = regex_replace(ln, std::regex(":"), "");
			table->addSymbol(label);

		} else if ( isOnlyInstruction )
		{
			std::string instr = removeBlanksStart( match_instruction[0] );
			unsigned short scanned = op0 ? 1 : 0;
			scanned |= op1 ? 2 : 0;
			scanned |= op2 ? 4 : 0;

			std::string *temp;
			unsigned int _i = getParsedInstruction(instr, &temp);
			_i--;

			//std::cout << "here\n";
			//std::cout << (_i >= 1 ? temp[1] : "0") << " " << (_i==2 ? temp[2] : "0");

			unsigned int incr = Instructions::parseInstruction(temp[0], _i >= 1 ? &temp[1] : nullptr, _i==2 ? &temp[2] : nullptr, scanned);

			// Increment location counter.
			table->incrementCounter(incr);

		} else if ( isOnlyDirective )	// Perform check for directive.
		{
			parsed = removeBlanksStart( match_directive[0] );
			regex_search( ln, test, REGEX_DIRECTIVE_NAME );
			parsed = test[0];
			int directive = Directives::getDirectiveCodeByName(parsed);

			processDirectiveFirstPass(directive, ln);


		} else if ( isLabelAndDirective )
		{
			ln = removeBlanksStart(ln);
			regex_search(ln, test, REGEX_ALONE_LABEL);
			std::string label = test[0];
			label = regex_replace(label, std::regex(":\\s*$"), "");

			// Add symbol into table.
			table->addSymbol(label);

			regex_search( ln, test, REGEX_DIRECTIVE_NAME );
			std::string directive = test[0];

			// Remove
			unsigned int ind = -1;
			bool found = false;
			for (unsigned int i=0; !found; i++)
			{
				if ( ln[i] == '.' )
				{
					found = true;
					ind = i;
				};
			};
			std::string rest = ln.substr(ind, ln.length());

			processDirectiveFirstPass(Directives::getDirectiveCodeByName(directive), rest);

		} else if ( isLabelAndInstruction )
		{
			std::string instruction;
			ln = removeBlanksStart(ln);
			regex_search(ln, test, REGEX_ALONE_LABEL);
			std::string label = test[0];
			label = regex_replace(label, std::regex(":"), "");

			instruction = regex_replace(ln, REGEX_ALONE_LABEL, "");
			instruction = regex_replace(instruction, std::regex("^\\s*"), "");
			op2 = regex_match( instruction, m2, REGEX_INSTRUCTION_OP2 );
			op1 = regex_match( instruction, m1, REGEX_INSTRUCTION_OP1 );
			op0 = regex_match( instruction, m0, REGEX_INSTRUCTION_OP0 );

			//std::cout << "LABEL + DIRECTIVE, label: " << label << ", instruction: " << instruction <<std::endl;

			// Adds symbol into symbol table.
			table->addSymbol(label);

			unsigned short scanned = op0 ? 1 : 0;
			scanned |= op1 ? 2 : 0;
			scanned |= op2 ? 4 : 0;

			std::string *temp;
			unsigned int _i = getParsedInstruction(instruction, &temp);
			_i--;

			unsigned int incr = Instructions::parseInstruction(temp[0], _i >= 1 ? &temp[1] : nullptr, _i==2 ? &temp[2] : nullptr, scanned);

			// Increment location counter.
			table->incrementCounter(incr);

		} else
		{
			Util::exitProgram("Error occured on first passage.", -1);
		}
	};
}

/**
 * Prepares for second pass.
 */
void Util::prepareForSecondPass()
{
	// Try to close file!
	if ( progdata->inputstr )
		progdata->inputstr->close();

	progdata->end = false;
	table->resetLocationCounter();

	// Reopen file.
	progdata->inputstr = new std::ifstream(progdata->inputfilename);
}

/**
 * Second pass through assembly file.
 */
void Util::makeSecondPass()
{
	std::ifstream *file = progdata->inputstr;
	std::string ln,
				parsed;
	std::smatch match,
				match_label,
				match_instruction, m0, m1, m2,
				match_directive,
				match_lbl_dir,
				match_lbl_instr,
				test;

	// Sets current line to 0;
	line = 0;

	while( getline( *file, ln ) && !progdata->isEnd() )
	{
		// Updates line number;
		updateLineNumber();

		// Scans for comment.
		if ( regex_match( ln, match, REGEX_COMMENT ) )
			continue;

		// Check if scanned line is empty.
		if ( regex_match( ln, match, REGEX_EMPTY_LINE ) )
			continue;

		// Remove comment if exists.
		ln = removeComment(ln);
		ln = regex_replace( ln, std::regex("\\s*$"), "" );

		//std::cout << ln << std::endl << std::endl;

		// Searches for all possibilities.
		bool isOnlyLabel = regex_match( ln, match_label, REGEX_ALONE_LABEL );
		bool isOnlyDirective = regex_match( ln, match_directive, REGEX_DIRECTIVE );
		bool op2 = regex_match( ln, m2, REGEX_INSTRUCTION_OP2 );
		bool op1 = regex_match( ln, m1, REGEX_INSTRUCTION_OP1 );
		bool op0 = regex_match( ln, m0, REGEX_INSTRUCTION_OP0 );
		bool isOnlyInstruction = op0 || op1 || op2;
		match_instruction = op2 ? m2 : (op1 ? m1 : m0);

		bool isLabelAndDirective = regex_match( ln, match_lbl_dir, REGEX_LABEL_DIRECTIVE );
		bool isLabelAndInstruction = regex_match( ln, match_lbl_instr, REGEX_LABEL_INSTRUCTION );

		// Perform check.
		if ( isOnlyLabel )
		{
			// Do nothing.
		} else if ( isOnlyInstruction )
		{
			std::string instr = removeBlanksStart( match_instruction[0] );
			unsigned short scanned = op0 ? 1 : 0;
			scanned |= op1 ? 2 : 0;
			scanned |= op2 ? 4 : 0;

			std::string *temp;
			unsigned int _i = getParsedInstruction(instr, &temp);
			_i--;

			//std::cout << "here\n";
			//std::cout << (_i >= 1 ? temp[1] : "0") << " " << (_i==2 ? temp[2] : "0");

			Instructions::codeInstruction(temp[0], _i >= 1 ? &temp[1] : nullptr, _i==2 ? &temp[2] : nullptr, scanned);

		} else if ( isOnlyDirective )	// Perform check for directive.
		{
			parsed = removeBlanksStart( match_directive[0] );
			regex_search( ln, test, REGEX_DIRECTIVE_NAME );
			parsed = test[0];
			int directive = Directives::getDirectiveCodeByName(parsed);

			processDirectiveSecondPass(directive, ln);
			//std::cout << "here finished " << parsed << std::endl;



		} else if ( isLabelAndDirective )
		{
			ln = removeBlanksStart(ln);
			regex_search(ln, test, REGEX_ALONE_LABEL);
			std::string label = test[0];
			label = regex_replace(label, std::regex(":\\s*$"), "");

			regex_search( ln, test, REGEX_DIRECTIVE_NAME );
			std::string directive = test[0];

			// Remove
			unsigned int ind = -1;
			bool found = false;
			for (unsigned int i=0; !found; i++)
			{
				if ( ln[i] == '.' )
				{
					found = true;
					ind = i;
				};
			};
			std::string rest = ln.substr(ind, ln.size());

			//std::cout << rest << std::endl;
			processDirectiveSecondPass(Directives::getDirectiveCodeByName(directive), rest);

		} else if ( isLabelAndInstruction )
		{
			/*
			std::string instruction;
			ln = removeBlanksStart(ln);
			regex_search(ln, test, REGEX_ALONE_LABEL);
			std::string label = test[0];
			label = regex_replace(label, std::regex(":"), "");

			instruction = regex_replace(ln, REGEX_ALONE_LABEL, "");
			instruction = regex_replace(instruction, std::regex("^\\s*"), "");
			op2 = regex_match( instruction, m2, REGEX_INSTRUCTION_OP2 );
			op1 = regex_match( instruction, m1, REGEX_INSTRUCTION_OP1 );
			op0 = regex_match( instruction, m0, REGEX_INSTRUCTION_OP0 );

			//std::cout << "LABEL + DIRECTIVE, label: " << label << ", instruction: " << instruction <<std::endl;

			// Adds symbol into symbol table.
			table->addSymbol(label);

			unsigned short scanned = op0 ? 1 : 0;
			scanned |= op1 ? 2 : 0;
			scanned |= op2 ? 4 : 0;

			std::string *temp;
			unsigned int _i = getParsedInstruction(instruction, &temp);
			_i--;

			unsigned int incr = Instructions::parseInstruction(temp[0], _i >= 1 ? &temp[1] : nullptr, _i==2 ? &temp[2] : nullptr, scanned);

			// Increment location counter.
			table->incrementCounter(incr);*/

		} else
		{
			Util::exitProgram("Error occured on first passage.", -1);
		}
	};
}
