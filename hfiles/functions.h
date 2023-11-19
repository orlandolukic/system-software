#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <string>
#include <vector>

void toLowerCase(std::string*);
void toUpperCase(std::string*);
char tolower(char in);
std::string removeBlanksStart(std::string str);
std::string removeComment(std::string str);
void processDirectiveFirstPass(int directive, std::string line);
void processDirectiveSecondPass(int directive, std::string line);
void removeBlanksInString(std::string*);
bool checkNumberRegularity(int number, int type);
char* infixToPostfix(std::string);
int expression(char* str);
unsigned int getNumberSpace(int number);
unsigned int getParsedInstruction(std::string line, std::string** buff);
std::vector<unsigned char> intToBytes(int paramInt);
unsigned int getBytesAsStringForNumber(std::string** first, std::string** second, short num);
unsigned int getBytesForNumber(unsigned short* first, unsigned short* second, short num, bool force = false);




#endif /* FUNCTIONS_H_ */
