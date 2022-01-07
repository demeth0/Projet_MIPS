#ifndef COMPILER_MIPS_MODULE
#define COMPILER_MIPS_MODULE

#include <stdio.h>
#include "mips_structures.h"
#include "operation_code_defines.h"
#include "ManipulationsFichier.h"
#include "ByteUtils.h"
#include <string.h>

#define LINE_MAX_WIDTH 128

/*
Description:
	première version du programme demandée (voir sujet.html) 
	prend le nom du fichier a compiler en entrée et le nom du fichier de sortie.
	Compile la source en language MIPS
parametres:
	source - fichier d'entrée
	output - fichier de sortie
return:
	void
*/
int compile(const char *source, const char *output);

/*
Description:
	prend une ligne en language assembleur MIPS et remplis la structure Instruction
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
int compileline(char *line,Instruction *output);

/*
Description:
	met tout les éléments de Instruction a 0.
Return:
	void
erreurs:
	si inst NULL
*/
void initInst(Instruction *inst);

#endif