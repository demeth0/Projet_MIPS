#include <stdio.h>
#include <stdlib.h>

typedef struct Instruction{
	unsigned char [4] code;
	unsigned char id;
	unsigned char b0,b1,b2,b3,b4,b5,b6,b7;
} Instruction;

void textInstructionToOpcode(char* textInstruction, Instruction *instruction);

/*
Description:
	Récupere tout les char jusqu'au premier espace dan sl'instruction en  parametre
et les mets dans res. exemple pour 'addiu $val, $res, 10' va donner 'addiu\0' 
return:
	void
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