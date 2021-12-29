#ifndef MIPS_SIMULATEUR_DEFINE
#define MIPS_SIMULATEUR_DEFINE

#include <stdio.h>
#include <stdlib.h>
#include "ByteUtils.h"
#include "InstructionCompiler.h"
#include "operation_code_defines.h"
#include "ByteUtils.h"

#define MEM_SIZE 256

struct SimEnv {
	Byte RAM[MEM_SIZE];
	Byte registers[32][4];
	Byte PC[4];
	Byte HI[4];
	Byte LO[4];
};

typedef struct SimEnv Environment;

/*fonction intrerne au script*/
/*void copy_Byte4(Byte entree[4],Byte sortie[4]);*/
void writeRamADDR(Environment *simulation,Byte adresse[4],Byte value);
Byte readRamADDR(Environment *simulation,Byte adresse[4]);
void afficher_ram(Environment *simulation);
void initSimulation(Environment *simulation);
	
void simulate(Instruction *instruction, Environment *simulation);

#endif