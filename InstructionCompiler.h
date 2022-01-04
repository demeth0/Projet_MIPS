/******************************************************
 *                                                    *
 *    première implémentation du compilateur MIPS     *
 *                  Deprecated                        *
 *                                                    *
 ******************************************************/

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
	prend une instruction texte d'une ligne et remplis la structure Instruction
	avec l'id du code instruction défini dans operation_code_define et la taille de chaque
	blocs de données de l'instruction met UNKNOW_ID dans la paramètre ID
	de la structure Instruction si le code opération n'est pas reconnu.
paramètre:
	textInstruction - chaine de caractères contenant l'instruction
	instruction - la struct inistruction qui recoit les informations déjà initialisées
return:
	void
erreur:
	si la chaine de caractères ne finit pas par \0, ou si 
	l'espace mémoire pour instruction n'est pas créée
*/
void textInstructionToOpcode(char* textInstruction, Instruction* instruction);

/*
Description:
	prend en paramètre une instruction et la chaine de caractères de l'instruction avec ses
	opérandes et remplit les opérandes dans le code de la struct 
	Instruction (écrit les valeurs des registres et valeurs immédiates dans le code 32bit).
paramètre:
	inst - l'instruction à remplir
	inst_str - le texte qui contient l'instruction 
return:
	void
erreur:
	si inst_str ne finit pas par \0 ou si la struct Instruction n'est pas initialisée
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