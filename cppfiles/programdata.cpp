#include "programdata.h"

#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>

/**
 * Create symbol data structure.
 */
ProgramData::ProgramData()
{
	outputstr = nullptr;
	inputstr = nullptr;
	error = false;
	end = false;
}

/**
 * Destroy program data.
 */
ProgramData::~ProgramData()
{
	// Deletes output file if error occured.
	if ( error )
	{
		char *cstr = new char[outputfilename.length() + 1];
		strcpy(cstr, outputfilename.c_str());
		remove(cstr);
		delete[] cstr;
	};

	if ( outputstr != nullptr )
	{
		outputstr->close();
		delete outputstr;
	};

	if ( inputstr != nullptr )
	{
		inputstr->close();
		delete inputstr;
	};
}

/**
 * Sets end of assembler work.
 */
void ProgramData::setEnd()
{
	end = true;
}

/**
 * Checks for end of assembler work.
 */
bool ProgramData::isEnd()
{
	return end;
}

