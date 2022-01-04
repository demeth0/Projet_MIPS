#ifndef MIPS_STRUCTURES_H
#define MIPS_STRUCTURES_H

#define MEM_SIZE 256

typedef unsigned char Byte;

struct SimEnv {
	Byte RAM[MEM_SIZE];
	Byte registers[32][4];
	Byte PC[4];
	Byte HI[4];
	Byte LO[4];
};

typedef struct SimEnv Environment;

typedef struct Instruction{
	Byte code[4];
	unsigned int id;
	Byte b[6];
}Instruction;

#endif