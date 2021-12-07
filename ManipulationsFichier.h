#ifndef MANIPULATION_FICHIER_LIB
#define MANIPULATION_FICHIER_LIB

#include <stdio.h>
#include "InstructionCompiler.h"

/*
Description: 
	va metre dans resultat l'instruction (texte) sur la ligne suivante dans le fichier en paramètre en enlevant les commentaires.
Paramètre:
	f      - le fichier dans lequel se trouve les instructions
	result - la chaine de caractère qui va stocker l'instruction lue
return:
	void
*/
void readInstruction(FILE *f, char *result);

/*
Description:
	écrit dans le fichier l'instruction en valeur hexadécimale formattée sur 8 char
parametres:
	f - le fichier ou écrire la donnée
	inst - l'instruction hexa a écrire dans le fichier
*/
void writeHexInstructionToFile(FILE *f, Instruction inst);



#endif