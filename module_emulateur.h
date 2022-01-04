#ifndef MIPS_SIMULATEUR_DEFINE
#define MIPS_SIMULATEUR_DEFINE

#include <stdio.h>
#include <stdlib.h>
#include "ByteUtils.h"
#include "operation_code_defines.h"
#include "ByteUtils.h"
#include "mips_structures.h"
#include "module_ram.h"

void initSimulation(Environment *simulation);
	
void simulate(Instruction *instruction, Environment *simulation);

#endif