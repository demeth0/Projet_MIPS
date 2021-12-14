#ifndef INSTRUCTION_COMPILER_MIPS
#define INSTRUCTION_COMPILER_MIPS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operation_code_defines.h"
#include "ByteUtils.h"

typedef struct Instruction{
	Byte code[4];
	unsigned int id;
	Byte b[8];
}Instruction;


/*
Description:
	prend une instruction texte d'une ligne et initialise la structure instruction
	avec le type et la taille des blocs de données de l'instruction
paramètre:
	textInstruction - chaine de caractères contenant l'instruction
	instruction - la struct inistruction qui recoit les informations déjà initialisées
return:
	void
erreur:
	si la chaine de caractère ne finit pas par \0
*/
void textInstructionToOpcode(char* textInstruction, Instruction* instruction);

/*
Description:
	prend en paramètre une instruction et la chaine de caractère de l'instruction avec ses
	opérandes et remplit les opérandes dans le code de la struct instruction
paramètre:
	inst - l'instruction à remplir
	inst_str - le texte qui contient l'instruction 
return:
	void
*/
void writeInstructionOperands(Instruction *inst, char *isnt_str);

/*
Description:
  enlève les espaces au début, à la fin et au milieu d'une chaine : "    ADD $1, $2, $3   " -> "ADD $1,$2,$3"
	et met la chaine de caractères en minuscule
paramètre:
	instr - chaine de caractères
return:
	void
*/
void format_instr(char *instr);

/*
Description:
	initialise instruction
paramètre:
	instruction à initialiser
return:
	void
*/
void initInst(Instruction *inst);
#endif