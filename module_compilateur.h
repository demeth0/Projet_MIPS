#ifndef COMPILER_MIPS_MODULE
#define COMPILER_MIPS_MODULE

#include <stdio.h>
#include "mips_structures.h"
#include "operation_code_defines.h"
#include "module_io.h"
#include "module_byteutils.h"
#include <string.h>

#define LINE_MAX_WIDTH 128

/*
Description:
	première version du programme demandee (voir sujet.html) 
	prend le nom du fichier a compiler en entree et le nom du fichier de sortie.
	Compile la source en language MIPS
parametres:
	source - fichier d'entree
	output - fichier de sortie
return:
	void
*/
int compileFile(const char *source, const char *output);

/*
Description:
	prend une ligne en language assembleur MIPS et remplis la structure Instruction
	avec l'id du code instruction defini dans operation_code_define et la taille de chaque
	blocs de donnees de l'instruction met UNKNOW_ID dans la paramètre ID
	de la structure Instruction si le code operation n'est pas reconnu.
paramètre:
	textInstruction - chaine de caractères contenant l'instruction
	instruction - la struct inistruction qui recoit les informations deja initialisees
return:
	void
erreur:
	si la chaine de caractères ne finit pas par \0, ou si 
	l'espace memoire pour instruction n'est pas creee
*/
int compileline(char *line,Instruction *output);

/*
Description:
	Compile un jeu d'instructions contenu dans un fichier source
	et ecrit les instructions compilees dans le tableau Program
*/
int compile(const char *source,Program prog);

/*
Description:
	met tout les elements de Instruction a 0.
Return:
	void
erreurs:
	si inst NULL
*/
void initInst(Instruction *inst);

#endif