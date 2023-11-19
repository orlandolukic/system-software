#ifndef DIRECTIVES_H_
#define DIRECTIVES_H_

#include <string>

class Directives
{
private:
	static std::string _DIRECTIVES[16];
public:
	static int NUM;
	static void fp_word();
	static std::string getDirectiveNameByCode( int );
	static int getDirectiveCodeByName( std::string );
};


#endif /* DIRECTIVES_H_ */
