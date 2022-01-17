#ifndef MANIPULATION_FICHIER_LIB
#define MANIPULATION_FICHIER_LIB

#include <stdio.h>
#include "mips_structures.h"
#include "operation_code_defines.h"
#include "module_byteutils.h"

/*
Description: 
	va mettre dans resultat l'instruction (texte) sur la ligne suivante dans le fichier en paramètre en enlevant les commentaires.
Paramètre:
	f      - le fichier dans lequel se trouve les instructions
	result - la chaine de caractère qui va stocker l'instruction lue
return:
	void
*/
void readInstruction(FILE *f, char *result);

/*
Description:
	ecrit dans le fichier l'instruction en valeur hexadecimale formattee sur 8 char
parametres:
	f - le fichier ou ecrire la donnee
	inst - l'instruction hexadecimale a ecrire dans le fichier
*/
void writeHexInstructionToFile(FILE *f, Instruction inst);

/*
Description:
	ecrit le programme en hexa dans le fichier
parametre:
	prog - le programe a ecrire
	f - le fichier de sortie
erreurs:
	si fichier non ouvert en ecriture/nul ou prog mal defini/nul vide
*/
void ecrireProgram(FILE *f, Program prog);

/*
Description:
	ecrit les registres de la simulation dans le fichier
parametre:
	sim - la simulation a ecrire
	f - le fichier de sortie
erreurs:
	si fichier non ouvert en ecriture/nul ou sim mal defini/nul vide
*/
void ecrireRegistres(FILE *f, Environment *sim);
#endif