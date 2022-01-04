#ifndef MODULE_RAM_MIPS
#define MODULE_RAM_MIPS

#include <stdio.h>
#include "mips_structures.h"

void writeRamADDR(Environment *simulation,DWord adresse,Byte value);
Byte readRamADDR(Environment *simulation,DWord adresse);
void afficher_ram(Environment *simulation);

#endif