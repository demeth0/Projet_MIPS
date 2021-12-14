#ifndef MIPS_SIMULATEUR_DEFINE
#define MIPS_SIMULATEUR_DEFINE

#include <stdio.h>
#include <stdlib.h>
#include "ByteUtils.h"

#define MEM_SIZE 256

struct SimEnv {
	Byte RAM[MEM_SIZE];
	Byte registers[32][4];
	Byte PC[4];
	Byte HI[4];
	Byte LO[4];
};

typedef struct SimEnv *Environment;




	


#endif