#ifndef PROGRAMDATA_H_
#define PROGRAMDATA_H_

#include <fstream>

class ProgramData
{
private:
	friend class Util;
	std::string inputfilename;
	std::string outputfilename;
	std::ifstream *inputstr;
	std::ofstream *outputstr;
	bool error;
	bool end;


public:
	ProgramData();
	~ProgramData();

	void setEnd();
	bool isEnd();


};




#endif
