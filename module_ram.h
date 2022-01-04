#ifndef MODULE_RAM_MIPS
#define MODULE_RAM_MIPS

#include <stdio.h>
#include "mips_structures.h"

void writeRamADDR(Environment *simulation,Byte adresse[4],Byte value);
Byte readRamADDR(Environment *simulation,Byte adresse[4]);
void afficher_ram(Environment *simulation);

#endif