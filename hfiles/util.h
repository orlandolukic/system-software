#ifndef UTIL_H_
#define UTIL_H_

#include <stdio.h>
#include <string>

#include "programdata.h"

class Util
{
private:
	static int line;
public:
	static void parseStartArgs(int, const char*[]);
	static void exitProgram(std::string mssg, int code);
	static void exitProgram(int code);
	static void destructGlobalData();
	static void exportFile();
	static void closeAllData();
	static void prepare();
	static void prepareForSecondPass();
	static void makeFirstPass();
	static void makeSecondPass();
	static void updateLineNumber();
	static unsigned int getLineNumber();
};



#endif /* UTIL_H_ */
