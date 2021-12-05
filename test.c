#include <stdio.h>
#include "textToBinary.h"

int main(int argc, char const *argv[])
{
	unsigned char val[2];
	Instruction inst;
	inst.code[0]=0x0;
	inst.code[1]=0x0;
	inst.code[2]=0x0;
	inst.code[3]=0x0;
	setBlocksSize(&inst, 6,5,16,5,0,0,0,0);

	val[0]=0x15;
	val[1]=0x92;

	printf("%02x %02x %02x %02x\n", inst.code[0],inst.code[1],inst.code[2],inst.code[3]);
	pasteValue(&inst, 2,val,2);
	printf("%02x %02x %02x %02x\n", inst.code[0],inst.code[1],inst.code[2],inst.code[3]);
	return 0;
}