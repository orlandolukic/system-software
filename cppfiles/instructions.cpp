#include "../hfiles/instructions.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <regex>
#include <sstream>

#include "../hfiles/functions.h"
#include "../hfiles/instruction.h"
#include "../hfiles/symboltable.h"
#include "../hfiles/util.h"

extern std::regex REGEX_OPERAND_1;
extern SymbolTable* table;

int Instructions::NUM = 25;
Instruction** Instructions::instructions = nullptr;
unsigned int Instructions::numbytes = 0;
unsigned short Instructions::bytes[7] = {0,0,0,0,0,0,0};
unsigned short Instructions::ADDR_IMMEDIATE = 0x0 << 5;
unsigned short Instructions::ADDR_REGISTER_DIRECT = 1 << 5;
unsigned short Instructions::ADDR_REGISTER_INDIRECT = 2 << 5;
unsigned short Instructions::ADDR_REGISTER_INDIRECT_8 = 3 << 5;
unsigned short Instructions::ADDR_REGISTER_INDIRECT_16 = 4 << 5;
unsigned short Instructions::ADDR_MEMORY = 5 << 5;
unsigned short Instructions::IS_ONE_BYTE = 0x0;
unsigned short Instructions::IS_TWO_BYTES = 0x4;
unsigned short Instructions::LOW = 0x0;
unsigned short Instructions::HIGH = 0x1;
bool Instructions::isOneByte = false;

/**
 * Prepares instructions.
 */
void Instructions::prepare()
{
	instructions = new Instruction*[NUM];
	instructions[0] = new Instruction(std::string("halt"), false, 0);
	instructions[1] = new Instruction(std::string("xchg"), true, 2);
	instructions[2] = new Instruction(std::string("int"), false, 1);
	instructions[3] = new Instruction(std::string("mov"), true, 2);
	instructions[4] = new Instruction(std::string("add"), true, 2);
	instructions[5] = new Instruction(std::string("sub"), true, 2);
	instructions[6] = new Instruction(std::string("mul"), true, 2);
	instructions[7] = new Instruction(std::string("div"), true, 2);
	instructions[8] = new Instruction(std::string("cmp"), true, 2);
	instructions[9] = new Instruction(std::string("not"), true, 1);
	instructions[10] = new Instruction(std::string("and"), true, 2);
	instructions[11] = new Instruction(std::string("or"), true, 2);
	instructions[12] = new Instruction(std::string("xor"), true, 2);
	instructions[13] = new Instruction(std::string("test"), true, 2);
	instructions[14] = new Instruction(std::string("shl"), true, 2);
	instructions[15] = new Instruction(std::string("shr"), true, 2);
	instructions[16] = new Instruction(std::string("push"), true, 1);
	instructions[17] = new Instruction(std::string("pop"), true, 1);
	instructions[18] = new Instruction(std::string("jmp"), false, 1);
	instructions[19] = new Instruction(std::string("jeq"), false, 1);
	instructions[20] = new Instruction(std::string("jne"), false, 1);
	instructions[21] = new Instruction(std::string("jgt"), false, 1);
	instructions[22] = new Instruction(std::string("call"), false, 1);
	instructions[23] = new Instruction(std::string("ret"), false, 0);
	instructions[24] = new Instruction(std::string("iret"), false, 0);
}

/*
void Instructions::resetInstructionByte()
{
	instructionCodeByte = 0;
}


void Instructions::prepareInstructionByte(unsigned short byte)
{
	instructionCodeByte |= byte;
}

unsigned short Instructions::getPreparedInstructionByte()
{
	return instructionCodeByte;
}*/

/**
 * Checks if instruction supports b/w additional parameter.
 */
bool Instructions::supportsBW(int ind)
{
	for (int i=0; i<NUM; i++)
		if ( i==ind )
			return instructions[i]->bw;
}

/**
 * Gets instruction based on given str.
 */
int Instructions::getInstructionCode(std::string instr)
{
	for (unsigned int i=0; i<instr.size(); i++)
		instr[i] = tolower(instr[i]);

	for (int i=0; i<NUM; i++)
		if ( instr.compare( instructions[i]->name ) == 0 )
			return i+1;
	return 0;
}

/**
 * Checks if given instruction is valid.
 */
bool Instructions::isValidInstruction(std::string instr)
{
	return getInstructionCode(instr) > 0;
}

/**
 * Parses instruction.
 */
unsigned int Instructions::parseInstruction(std::string instruction, std::string *first, std::string *second, unsigned short operands)
{
	unsigned int incr = 1;
	bool found = false;
	std::smatch test;
	std::string tempb, tempw, temp;
	bool taken, isByte;
	//std::cout << "INSTRUCTION >> " << instruction << " scanned >> " << operands << std::endl;
	//std::cout << "first:: " << (first!=nullptr ? *first : "") << " second:: " << (second!=nullptr ? *second : "") << " " << std::endl;
	isByte = isOneByte;
	for (unsigned short i=0; i<NUM && !found; i++)
	{
		taken = false;
		temp = instructions[i]->name;
		tempw = std::string(temp);
		tempb = std::string(temp);
		tempb += "b"; tempw += "w";
		isOneByte = false;


		if ( instructions[i]->bw )
		{
			if ( tempb.compare(instruction) == 0 )
			{
				taken = true;
				isOneByte = true;
			} else if ( tempw.compare(instruction) == 0 )
			{
				taken = true;
			};
		};

		// Check instruction.
		if ( temp.compare(instruction) == 0 || taken )
		{
			found = true;

			// Check b/w suffix.
			if ( taken && !instructions[i]->bw )
				Util::exitProgram("Illegal instruction b/w suffix.", 2);

			// Check operand sizes;
			if ( (operands & 1) && instructions[i]->op == 0 )
			{
				// Do nothing.
			} else if ( (operands & 2) && instructions[i]->op == 1 )
			{
				//std::cout << *first << std::endl;
				incr += getAllocatedBytes(*first);
			} else if ( (operands & 4) && instructions[i]->op == 2 )
			{
				incr += getAllocatedBytes(*first);
				incr += getAllocatedBytes(*second);

				//std::cout << "INCR " << incr << std::endl;
			} else
			{
				Util::exitProgram("Illegal instruction operands.", 3);
			};
		};
	};
	isOneByte = isByte;
	return incr;
}

/**
 * Codes instruction.
 */
void Instructions::codeInstruction(std::string instruction, std::string *first, std::string *second, unsigned short operands)
{
	// Reset byte byffer.
	for (unsigned int i=0; i<7; i++)
		bytes[i] = 0;
	numbytes = 0;

	bool found = false;
	std::smatch test;
	std::string tempb, tempw, temp, x;
	bool taken, isByte;
	//std::cout << "INSTRUCTION >> " << instruction << " scanned >> " << operands << std::endl;
	//std::cout << "first:: " << (first!=nullptr ? *first : "") << " second:: " << (second!=nullptr ? *second : "") << " " << std::endl;
	for (unsigned short i=0; i<NUM && !found; i++)
	{
		taken = false;
		isByte = false;
		temp = instructions[i]->name;
		tempw = std::string(temp);
		tempb = std::string(temp);
		tempb += "b"; tempw += "w";

		if ( tempb.compare(instruction) == 0 )
		{
			isByte = true;
			taken = true;
		} else if ( tempw.compare(instruction) == 0 )
		{
			taken = true;
		};

		// Check instruction.
		if ( temp.compare(instruction) == 0 || taken )
		{
			found = true;

			// Instruction does not support b/w.
			if ( !instructions[i]->bw && taken )
			{
				x = "Instruction '"; x += instruction;

				if ( first != nullptr )
				{
					x += " ";
					x += *first;
				};

				if ( second != nullptr )
				{
					x+= ", ";
					x += *second;
				};

				x += "' does not support b/w suffix.";
				Util::exitProgram(x, 1);
			};

			// Code instruction code.
			bytes[ numbytes ] = (i+1) << 3;
			bytes[ numbytes ] |= isByte ? IS_ONE_BYTE : IS_TWO_BYTES;
			isOneByte = isByte;
			//std::cout << "isOneByte " << (isOneByte ? "yes" : "no") << std::endl;
			numbytes++;

			// Increments location counter.
			//std::cout << temp << " incr=" << parseInstruction(temp, first, second, operands) << std::endl;
			table->incrementCounter(parseInstruction(temp, first, second, operands));
			//std::cout << "loc=" << table->getLocationCounter() << std::endl;

			// Check operand sizes;
			if ( (operands & 1) && instructions[i]->op == 0 )
			{
				// Do nothing.
			} else if ( (operands & 2) && instructions[i]->op == 1 )
			{
				//std::cout << *first << std::endl;
				distinguishInstruction(*first);
			} else if ( (operands & 4) && instructions[i]->op == 2 )
			{
				distinguishInstruction(*first);
				distinguishInstruction(*second);
				//std::cout << "INCR " << incr << std::endl;
			} else
			{
				std:: cout << "else\n";
			};

			// Put instruction into output bytes buffer.
			putIntoBuffer(0, numbytes);
		};
	};
}

/**
 * Refreshes byte for current instruction.
 */
void Instructions::distinguishInstruction( std::string operand )
{
	int regnum, disp, step, start, num, offset;
	std::string tmpstr, temp, temp1;
	std::smatch test, test1, test_1, test_2, test_3, test_4;
	std::stringstream ss;
	bool isRegisterDirect = regex_match( operand, test_1, std::regex("(sp|r[0-7])(h|l)?"));
	bool isRegisterIndirect = regex_match( operand, test_2, std::regex("(\\[(sp|r[0-7])\\]|(sp|r[0-7])\\[-?[1-9][0-9]*\\]|\\$[a-zA-Z_][a-zA-Z_0-9]*|(sp|r[0-7])\\[[a-zA-Z_][a-zA-Z_0-9]*\\])"));
	bool isImmediate = regex_match( operand, test_3, std::regex("(0|-?[1-9][0-9]*|&[a-zA-Z_][a-zA-Z_0-9]*)"));
	bool isMemory = regex_match( operand, test_4, std::regex("([a-zA-Z_][a-zA-Z_0-9]*|\\*0x[0-9a-fA-F]{1,4})") );

	start = numbytes;
	//std::cout << operand << std::endl;

	if ( isRegisterDirect )
	{
		// Set addresing type.
		bytes[numbytes] |= ADDR_REGISTER_DIRECT;
		std::string reg = test_1[0];
		regnum = getRegisterNumber(&reg);

		// Add register.
		bytes[numbytes] |= regnum << 1;

		// Add l/h.
		if ( isOneByte )
		{
			temp = operand.back();
			bytes[numbytes] |= temp == "h" ? HIGH : LOW;
		};

		numbytes++;

	} else if ( isRegisterIndirect )
	{
		tmpstr = test_2[0];
		// Set addresing type.
		if ( regex_search( tmpstr, test, std::regex("\\[(sp|r[0-7])\\]") ) )
		{
			bytes[numbytes] |= ADDR_REGISTER_INDIRECT;
			temp = test[0];
			regnum = getRegisterNumber(&temp);
			bytes[numbytes] |= regnum << 1;
			numbytes++;
		} else if ( regex_search( tmpstr, test, std::regex("(sp|r[0-7])\\[-?[1-9][0-9]*\\]") ) )
		{
			temp = test[0];
			regex_search( temp, test, std::regex("(sp|r[0-7])") );
			std::string reg = test[0];
			regnum = getRegisterNumber(&reg);
			disp = getDisplacement(&tmpstr);
			step = getNumberSpace(disp);

			// Second byte.
			bytes[numbytes] |= step == 1 ? ADDR_REGISTER_INDIRECT_8 : ADDR_REGISTER_INDIRECT_16;
			bytes[numbytes] |= regnum << 1;
			numbytes++;

			// Add third and/or fourth bytes.
			unsigned short f, s;
			unsigned int num = getBytesForNumber(&f, &s, disp);
			for (int i=0; i<num && i<step; i++)
				bytes[numbytes++] = i == 0 ? f : s;

		} else if ( regex_search( tmpstr, test, std::regex("\\$[a-zA-Z_][a-zA-Z_0-9]*") ) )	// $label
		{
			bytes[numbytes] |= ADDR_REGISTER_INDIRECT_16;	// displacement 16 bytes.
			bytes[numbytes] |= 7 << 1;						// pc
			numbytes++;

			// Remove $ from operand.
			temp1 = regex_replace(tmpstr, std::regex("\\$"), "");

			/*
			std::cout << "counter=" << table->getContentWriter()->getCounter() << ", locCounter=" << table->getLocationCounter()
					<< ", numbytes=" << numbytes
					<< ", raz=" << (int)(table->getLocationCounter()-(table->getContentWriter()->getCounter()+numbytes))
					 << std::endl;*/

			// Checks if there is symbol within current section.
			if ( table->exists(temp1) && !table->isSymbolVisible(temp1) && table->inCurrentSection(temp1) )
			{
				disp = table->getSymbolValue(temp1) - table->getLocationCounter();
				//std::cout << "disp = " << disp << std::endl;
			} else
			{
				// Add relocation for this addressing type.
				disp = -(table->getLocationCounter()-(table->getContentWriter()->getCounter()+numbytes));
				table->addRelocationEntry(temp1, ContentWriterRel::PC_REL_16, numbytes);
			};

			// Add third and/or fourth bytes for first/second operand.
			unsigned short f, s;
			getBytesForNumber(&f, &s, disp, true);
			for (unsigned int i=0; i<2; i++)
				bytes[numbytes++] = i == 0 ? f : s;

		} else if ( regex_search( tmpstr, test, std::regex("(sp|r[0-7])\\[[a-zA-Z_][a-zA-Z_0-9]*\\]") ) )
		{
			int reg;
			temp = regex_replace(tmpstr, std::regex("\\[[a-zA-Z_][a-zA-Z_0-9]*\\]"), "");
			reg = getRegisterNumber(&temp);
			bytes[numbytes] |= ADDR_REGISTER_INDIRECT_16;	// displacement 16 bytes.
			bytes[numbytes] |= reg << 1;
			numbytes++;

			temp1 = regex_replace( tmpstr, std::regex("(sp|r[0-7])"), "" );
			temp1 = regex_replace( temp1, std::regex("(\\[|\\])"), "" );
			//std::cout << temp1 << std::endl;
			if ( !table->exists(temp1) )
			{
				temp = "Symbol '"; temp += temp1; temp += "' is not defined!";
				Util::exitProgram(temp, 1);
			};
			num = table->getSymbolValue(temp1);

			// Add relocation entry.
			table->addRelocationEntry(temp1, ContentWriterRel::PC_16, numbytes);

			// Add third and/or fourth bytes.
			unsigned short f, s;
			getBytesForNumber(&f, &s, num);
			for (unsigned int i=0; i<2; i++)
				bytes[numbytes++] = i == 0 ? f : s;

		};
	} else if ( isImmediate )
	{
		bool isnum = true;
		bytes[numbytes] |= ADDR_IMMEDIATE;
		numbytes++;

		if ( regex_search( operand, test1, std::regex("[a-zA-Z_][a-zA-Z_0-9]*") ) )
		{
			isnum = false;
			temp = test1[0];
			if ( !table->exists(temp) )
			{
				temp1 = "Symbol '"; temp1 += temp; temp1 += "' is not defined!";
				Util::exitProgram(temp1, 1);
			};
			num = table->getSymbolValue(temp);

			// Add relocation here.
			table->addRelocationEntry(temp, ContentWriterRel::PC_16, numbytes);
		} else
			num = stoi(test_3[0]);

		// Get number space (1/2 bytes?)
		step = getNumberSpace(num);

		//std::cout << num << std::endl;
		if ( isOneByte && step == 2 )
		{
			ss << "Operand " << num << " could not be represented within one byte.";
			Util::exitProgram(ss.str(), 1);
		};

		step = isOneByte ? 1 : 2;
		// Add third and/or fourth bytes.
		unsigned short f, s;
		getBytesForNumber(&f, &s, num, true);
		if ( isnum )
		{
			//std::cout << "isOneByte here = " << (isOneByte ? "yes" : "no") << std::endl;
			//std::cout << step << std::endl;
			for (int i=0; i<step; i++)
				bytes[numbytes++] = i == 0 ? f : s;
		} else
		{
			for (unsigned int i=0; i<2; i++)
				bytes[numbytes++] = i == 0 ? f : s;
		};

	} else if ( isMemory )
	{
		bytes[numbytes] |= ADDR_MEMORY;
		numbytes++;
		if ( regex_search( operand, test, std::regex("\\*0x[0-9a-fA-F]{1,4}") ) )
		{
			temp = test[0];
			temp = regex_replace( temp, std::regex("\\*0x"), "" );

			//std::cout << "temp=" << temp << std::endl;

			unsigned int x;
			std::stringstream ss;
			ss << std::hex << temp;
			ss >> x;
			step = getNumberSpace(x);
			num = x;

			//std::cout << x << std::endl;

			unsigned short f, s;
			getBytesForNumber(&f, &s, num);
			for (unsigned int i=0; i<2; i++)
				bytes[numbytes++] = i == 0 ? f : s;
		} else if ( regex_search( operand, test, std::regex("[a-zA-Z_][a-zA-Z_0-9]*") ) )
		{
			temp = test[0];
			if ( !table->exists(temp) )
			{
				temp1 = "Symbol '"; temp1 += temp; temp1 += "' is not defined!";
				Util::exitProgram(temp1, 1);
			};
			num = table->getSymbolValue(temp);
			step = getNumberSpace(num);

			// Add relocation here.
			table->addRelocationEntry(temp, ContentWriterRel::PC_16, numbytes);

			// Add third and/or fourth bytes.
			unsigned short f, s;
			getBytesForNumber(&f, &s, num);
			for (unsigned int i=0; i<2; i++)
				bytes[numbytes++] = i == 0 ? f : s;

		} else
		{
			std::cout << "function:distinguishInstruction => nothing!" << std::endl;
		}
	};
}

/**
 * Puts into buffer rest of the instructions.
 */
void Instructions::putIntoBuffer(unsigned int st, unsigned int end)
{
	for (unsigned int i=st; i<end; i++)
		table->addByteIntoBuffer(bytes[i], 1);
}

/**
 * Gets register number.
 */
unsigned short Instructions::getRegisterNumber(std::string* reg)
{
	std::smatch test;
	if ( regex_search( *reg, test, std::regex("[0-7]") ) )
	{
		return stoi(test[0]);
	} else	// For SP.
		return 6;
}

/**
 * Gets displacement for given string.
 */
int Instructions::getDisplacement(std::string* disp)
{
	std::string temp;
	std::smatch test;
	if ( regex_search( *disp, test, std::regex("\\[-?[1-9][0-9]*\\]") ) )
	{
		temp = test[0];
		temp = regex_replace(temp, std::regex("\\["), "");
		temp = regex_replace(temp, std::regex("\\]"), "");
		return stoi(temp);
	}
	return 0;
}

/**
 * Remove global data.
 */
void Instructions::removeData()
{
	for (int i=0; i<NUM; i++)
		delete instructions[i];
	delete[] instructions;
}

/**
 * Gets how many bytes in allocation.
 */
unsigned int Instructions::getAllocatedBytes(std::string operand)
{
	std::smatch test, test1, test_1, test_2, test_3, test_4;
	bool isRegisterDirect = regex_match( operand, test_1, std::regex("(sp|r[0-7])"));
	bool isRegisterIndirect = regex_match( operand, test_2, std::regex("(\\[(sp|r[0-7])\\]|(sp|r[0-7])\\[-?[1-9][0-9]*\\]|\\$[a-zA-Z_][a-zA-Z_0-9]*|(sp|r[0-7])\\[[a-zA-Z_][a-zA-Z_0-9]*\\])"));
	bool isImmediate = regex_match( operand, test_3, std::regex("(0|-?[1-9][0-9]*|&[a-zA-Z_][a-zA-Z_0-9]*)"));
	bool isMemory = regex_match( operand, test_4, std::regex("([a-zA-Z_][a-zA-Z_0-9]*|\\*0x[0-9a-fA-F]{1,4})") );

	//std::cout << *operand;

	if ( isRegisterDirect )
	{
		//std::cout << "\tregister direct\n";
		return 1;
	} else if ( isRegisterIndirect )
	{
		std::string disp, temp;
		//std::cout << "\tregister indirect\n";
		int num;
		if ( regex_search(operand, test1, std::regex("\\[-?[1-9][0-9]*\\]")) )
		{

			disp = test1[0];
			disp = regex_replace(disp, std::regex("\\["), "");
			disp = regex_replace(disp, std::regex("\\]"), "");
			num = stoi(disp);
			return 1 + getNumberSpace(num);

		} else if ( regex_search( operand, test1, std::regex("\\$[a-zA-Z_][a-zA-Z_0-9]*") ) )
		{
			disp = test1[0];
			temp = regex_replace(operand, std::regex("\\$"), "");
			return 3;
		} else if ( regex_search( operand, test1, std::regex("\\[(sp|r[0-7])\\]") ) )
		{
			return 1;
		} else if ( regex_search( operand, test1, std::regex("(sp|r[0-7])\\[[a-zA-Z_][a-zA-Z_0-9]*\\]") ) )
		{
			return 3;
		};
	} else if ( isImmediate )
	{
		int num = 0;
		//std::cout << "\timmediate\n";
		if ( regex_search( operand, test1, std::regex("[a-zA-Z_][a-zA-Z_0-9]*") ) )
		{
			return 3;
		} else
			num = stoi(test_3[0]);
		return 1 + (isOneByte ? 1 : 2);
	} else if ( isMemory )
	{
		//std::cout << "\tmemory direct\n";
		return 3;
	};
	return 0;

}




