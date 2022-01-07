#ifndef MIPS_STRUCTURES_H
#define MIPS_STRUCTURES_H

#define MEM_SIZE 256
#define PROGRAM_MAX_SIZE 128

typedef unsigned char Byte;
typedef Byte DWord[4];

struct SimEnv {
	Byte RAM[MEM_SIZE];
	DWord registers[32];
	DWord PC;
	DWord HI;
	DWord LO;
};

typedef struct SimEnv Environment;

typedef struct Instruction{
	DWord code;
	unsigned int id;
	Byte b[6];
}Instruction;

typedef Instruction Program[PROGRAM_MAX_SIZE];

#endif