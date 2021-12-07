#include <stdio.h>
#include "InstructionCompiler.h"

void printInst(char *str_inst,Instruction inst){
	printf("Instruction : \n\t\"%s\"\n\tID: %d\n\tHex: %02x%02x %02x%02x\n\tBlocks size: %d %d %d %d %d %d %d %d\n", 
				str_inst,
				inst.id,
				inst.code[0],
				inst.code[1],
				inst.code[2],
				inst.code[3],
				inst.b[0],
				inst.b[1],
				inst.b[2],
				inst.b[3],
				inst.b[4],
				inst.b[5],
				inst.b[6],
				inst.b[7]);
}

int main(int argc, char const *argv[])
{
	char str_inst[] = "ADD $1,$2,$3";
	Instruction inst;

	format_instr(str_inst);
	initInst(&inst);

	textInstructionToOpcode(str_inst, &inst);
	writeInstructionOperands(&inst, str_inst);
	printInst(str_inst,inst);

	return 0;
}