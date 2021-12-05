#include <stdio.h>
#include "textToBinary.h"
#include "ManipulationsFichier.h"
#include "operation_code_defines.h"
#include "register_defines.h"

int SignedStrIntegerToInt(char *str){
	char *cp = str;
	int res=0;
	int sign=1;
	if((*cp)=='-'){
		sign=-1;
		cp++;
	}
	while((*cp) != '\0'){
		printf("%d\n", res);
		res = res*10 + ((*cp)-'0');
		cp++;
	}

	return sign*res;
}

/*
lower case string
*/
int HexStrIntegerToInt(char *str){
	char *cp = str;
	int res=0;
	int buffer=0;
	while((*cp) != '\0'){
		switch((*cp)){
			case 'a':
				buffer=0xA;
				break;
			case 'b':
				buffer=0xB;
				break;
			case 'c':
				buffer=0xC;
				break;
			case 'd':
				buffer=0xD;
				break;
			case 'e':
				buffer=0xE;
				break;
			case 'f':
				buffer=0xF;
				break;
			default:
				buffer=(*cp)-'0';
				break;
		}
		res = res<<4;
		res += buffer;
		
		cp++;
	}

	return res;
}

int ImmediatStrToInteger(char *str){
	int res;
	if(str[0]=='0' && str[1]=='x'){
		res=HexStrIntegerToInt(str+2);
	}else{
		res = SignedStrIntegerToInt(str);
	}
	return res;
} 

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

	printf("test: %d\n", ImmediatStrToInteger("0x9fb"));

	return 0;
}