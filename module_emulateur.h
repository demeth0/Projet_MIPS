#ifndef MIPS_SIMULATEUR_DEFINE
#define MIPS_SIMULATEUR_DEFINE

#include <stdio.h>
#include <stdlib.h>
#include "ByteUtils.h"
#include "operation_code_defines.h"
#include "ByteUtils.h"
#include "mips_structures.h"
#include "module_ram.h"
#include "module_compilateur.h"
#include "ManipulationsFichier.h"

void initSimulation(Environment *simulation);
	
void simulate(Instruction *instruction, Environment *simulation);

void simulateFile(const char *filename,Environment *simulation,int sequential);

void printInst(Environment *sim, Instruction inst,char *str_inst);

#endif