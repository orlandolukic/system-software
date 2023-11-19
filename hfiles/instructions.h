#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

class Instruction;
#include <string>

class Instructions
{
private:
	static int NUM;
	static unsigned int numbytes;
	static unsigned short bytes[7];
	static Instruction** instructions;
	static bool isOneByte;

	static void distinguishInstruction(std::string);
	static unsigned short getRegisterNumber(std::string* reg);
	static int getDisplacement(std::string* disp);
	static void putIntoBuffer(unsigned int st, unsigned int end);

public:
	static unsigned short ADDR_IMMEDIATE;
	static unsigned short ADDR_REGISTER_DIRECT;
	static unsigned short ADDR_REGISTER_INDIRECT;
	static unsigned short ADDR_REGISTER_INDIRECT_8;
	static unsigned short ADDR_REGISTER_INDIRECT_16;
	static unsigned short ADDR_MEMORY;
	static unsigned short IS_ONE_BYTE;
	static unsigned short IS_TWO_BYTES;
	static unsigned short LOW;
	static unsigned short HIGH;

	static void prepare();
	static void removeData();

	static bool supportsBW(int);
	static int getInstructionCode(std::string);
	static bool isValidInstruction(std::string);
	static unsigned int parseInstruction(std::string instruction, std::string *first, std::string *second, unsigned short operands);
	static void codeInstruction(std::string instruction, std::string *first, std::string *second, unsigned short operands);
	static unsigned int getAllocatedBytes(std::string operand);
	static void codeBytes();

	/*
	static void prepareInstructionByte(unsigned short byte);
	static unsigned short getPreparedInstructionByte();
	static void resetInstructionByte();*/
};

#endif /* INSTRUCTIONS_H_ */
