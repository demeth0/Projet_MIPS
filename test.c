#include <stdio.h>
#include "textToBinary.h"
#include "ManipulationsFichier.h"
#include "operation_code_defines.h"
#include "register_defines.h"

int main(int argc, char const *argv[])
{
	unsigned char val[2];
	Byte res[2];
	int toConvert = 25;
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

	IntTo2ByteArray(toConvert,res);
	printf("test: %02x %02x\n", res[0],res[1]);
	return 0;
}