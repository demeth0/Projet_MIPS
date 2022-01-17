#ifndef MODULE_RAM_MIPS
#define MODULE_RAM_MIPS

#include <stdio.h>
#include "mips_structures.h"


/*
Description:
	ecrit une valeur dans la ram a partir d'une adresse donnee
parametre:
	simulation - pour acceder a la ram
	adresse - adresse en ram ou ecrire la valeur
	value - valeur a ecrire en ram
*/
void writeRamADDR(Environment *simulation,DWord adresse,Byte value);

/*
Description:
	lit une valeur dans la ram a partir d'une adresse donnee et retourne le resultat
parametre:
	simulation - pour acceder a la ram
	adresse - adresse en ram ou ecrire la valeur
*/
Byte readRamADDR(Environment *simulation,DWord adresse);

/*
Description:
	affiche la ram
parametre:
	simulation - pour acceder a la ram
*/
void afficher_ram(Environment *simulation);

#endif