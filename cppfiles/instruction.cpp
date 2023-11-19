#include "instruction.h"

/**
 * Creates instruction.
 */
Instruction::Instruction(std::string name, bool bw, unsigned int op)
{
	this->name = name;
	this->bw = bw;
	this->op = op;
}

Instruction::~Instruction()
{

}

unsigned int Instruction::getOperandNumber()
{
	return op;
}

