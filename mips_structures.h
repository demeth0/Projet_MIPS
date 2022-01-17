#ifndef MIPS_STRUCTURES_H
#define MIPS_STRUCTURES_H

/*taille memoire en octets*/
#define MEM_SIZE 256
/*nombre max d'instruction dans un programme+1*/
#define PROGRAM_MAX_SIZE 128

/*défini la structure d'un octet*/
typedef unsigned char Byte;

/*defini la structure d'un mot de 32 bits*/
typedef Byte DWord[4];

/*defini environment pour la simulation mips*/
struct SimEnv {
	Byte RAM[MEM_SIZE];
	DWord registers[32];
	DWord PC;
	DWord HI;
	DWord LO;
};

typedef struct SimEnv Environment;

/*defini la structure pour une instruction*/
typedef struct Instruction{
	char text_instr[64];
	DWord code;
	unsigned int id;
	Byte b[6];
}Instruction;

typedef Instruction Program[PROGRAM_MAX_SIZE];

#endif