#include "../hfiles/directives.h"

extern void toLowerCase(std::string*);

int Directives::NUM = 16;

std::string Directives::_DIRECTIVES[] = {
	".global",			// 0
	".extern",			// 1
	".equ",				// 2
	".text",			// 3
	".data",			// 4
	".bss",				// 5
	".section",			// 6
	".byte",			// 7
	".word",			// 8
	".align",			// 9
	".skip",			// 10
	".end",				// 11
	".rept",			// 12
	".endr",			// 13
	".asciz",			// 14
	".ascii"			// 15
};

/**
 * Gets directive code by name.
 */
std::string Directives::getDirectiveNameByCode(int dir)
{
	return _DIRECTIVES[dir];
}

/**
 * Gets directive name by code.
 */
int Directives::getDirectiveCodeByName(std::string directive)
{
	toLowerCase(&directive);
	for (int i=0; i<NUM; i++)
		if ( directive.compare(_DIRECTIVES[i]) == 0 )
			return i;
	return -1;
}



