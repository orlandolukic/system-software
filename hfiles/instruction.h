#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <string>

class Instructions;

class Instruction
{
private:
	friend class Instructions;
	std::string name;
	bool bw;
	unsigned int op;

public:
	Instruction(std::string, bool bw, unsigned int op);
	~Instruction();

	unsigned int getOperandNumber();
};

#endif /* INSTRUCTION_H_ */
