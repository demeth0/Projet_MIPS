#ifndef TEXT_TO_BINARY
#define TEXT_TO_BINARY

#include <stdio.h>
#include <stdlib.h>
#include "operation_code_defines.h"

typedef struct Instruction{
	unsigned char code[4];
	unsigned int id;
	unsigned char b[8];
}Instruction;


void textInstructionToOpcode(char* textInstruction, Instruction* instruction);

/*
Description:
	met les 6 premiers bits de l'instruction avec opCode en conservant les valeurs d'origine pour le reste
parametre:
	instruction - l'instruction en paramtre a changer
	opCode - le code instruction a insérer en début
return:
	void
error :
	si instruction pas initialiser
*/
void setNormalOpCode(Instruction *instruction, char opCode);

/*
Description:
	met les 6 premiers bits de l'instruction a 0 et les 6 derniers avec opCode en conservant les valeurs d'origine pour le reste
parametre:
	instruction - l'instruction en paramtre a changer
	opCode - le code instruction a insérer en fin
return:
	void
error :
	si instruction pas initialiser
*/
void setSpecialOpCode(Instruction *instruction, char opCode);

/*
Description:
	Récupere tout les char jusqu'au premier espace dan l'instruction en  parametre
et les mets dans res. exemple pour 'addiu $val, $res, 10' va donner 'addiu\0' 
parametre:
	textInstruction - l'instruction complete dans un tableau de char (fin \0)
	res - tableau de char qui va stocker l'instruction au moin de taille 8 (fin \0)
return:
	void
erreur:
	si res est trop petit ou textInstruction n'est pas valide
*/
void getOperationCodeText(char* textInstruction, char* res);

/*
Description:
	remplie les paramètres block de l'instruction
parametre:
	Instruction* instruction - l'instruction qui va etre modifiée
	char bn - les tailles des blocks
return: 
	void
erreur:
	si l'instruction pas initialisée
*/
void setBlocksSize(Instruction* instruction, char b0,char b1,char b2, char b3,char b4,char b5,char b6,char b7);

/*
Description:
	Prend deux octets et une position entre 0 et 8, le résultat est les positions bits de poid fort de b1 plus les 8-position 
	bits de poid fort de b2 
exemple:
	b1 = 10011101
	b2 = 11111111
	pos = 5

	donc b1 : 10011
         b2 :      111
         res: 10011111
parametres:
	b1 - premier octet
	b2 - deuxieme octet
	pos - l'index de compositon
return:
	le résultat de l'opération
*/
void pasteReg(Instruction* instruction, int field,char value);

#endif