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
	met les 6 premiers bits de l'instruction avec opCode en conservant les valeurs d'origine pour le reste
paramètre:
	instruction - l'instruction en paramètre à changer
	opCode - le code instruction à insérer en début
return:
	void
error :
	si instruction n'est pas initialisée
*/
void setNormalOpCode(Instruction *instruction, Byte opCode);

/*
Description:
	met les 6 premiers bits de l'instruction a 0 et les 6 derniers avec opCode en conservant les valeurs d'origine pour le reste
paramètre:
	instruction - l'instruction en paramètre à changer
	opCode - le code instruction à insérer en fin
return:
	void
error :
	si instruction n'est pas initialisée
*/
void setSpecialOpCode(Instruction *instruction, Byte opCode);

/*
Description:
	Récupère tous les chars jusqu'au premier espace dans l'instruction en paramètre
et les met dans res. exemple pour 'addiu $val, $res, 10' va donner 'addiu\0' 
paramètre:
	textInstruction - l'instruction complète dans un tableau de char (fin \0)
	res - tableau de char qui va stocker l'instruction au moins de taille 8 (fin \0)
return:
	void
erreur:
	si res est trop petit ou textInstruction n'est pas valide
*/
void getOperationCodeText(char* textInstruction, char* res);

/*
Description:
	remplie les paramètres bloc de l'instruction
paramètre:
	Instruction* instruction - l'instruction qui va être modifiée
	char bn - les tailles des blocs
return: 
	void
erreur:
	si l'instruction n'est pas initialisée
*/
void setBlocksSize(Instruction* instruction, Byte b0,Byte b1,Byte b2, Byte b3,Byte b4,Byte b5,Byte b6,Byte b7);

/*
Description:
	prend en paramètre une instruction dont les tailles b[n] sont initialisées, et un numéro de bloc
	la valeur value de dim octet sera écrite dans le code de la struct Instruction dans zone qui lui est associée
	avec la taille correspondante dans b[n].
paramètre:
	instruction - instruction initialisée 
	field - le numéro de bloc qu'on souhaite remplir
	value - la valeur que le bloc va prendre codé sur 8*dim bit soit Byte[dim] tel que 8*dim > b[field]
	dim - taille du tableau value en octet
erreurs:
	si instruction n'est pas initialisée, alors il y a un risque de crash, aussi la zone dans le code de 
	l'instruction ou l'on veut écrire doit nécessairement être vide en risque de corrompre tout le code.
	la taille de value ne provoquera pas d'exception, la fonction tronquera le nombre automatiquement.
*/
void pasteValue(Instruction* instruction, int field,Byte* value,int dim);

/*
Description:
	transforme une chaine de caractères en lower case
parametre:
	text - chaine de caractères se finissant par \0
return:
	void
erreur:
	si la chaine n'est pas terminée par \0
*/
void toLowerCase(char *text);

/*
Description:
	place les arguments dans une matrice de caractères
paramètre:
	tab - tableau bidimensionnel de char
	instruction - pointeur vers char
return:
	void
*/
void param_to_tab(char tab[8][16],char *instruction);

/*
Description:
	transforme une chaine de caractère en binaire  -> "11" = 11
paramètre:
	val - chaine de caractères
return:
	Byte
*/
Byte registerToByte(char *val);


/*
Description:
  retourne la valeur du registre et met dans offset la valeur de l'offset.
  exemple si on a "200($1)" => offset=200 et return = 1
paramètre:
	str - chaine de caractères
	offset - chaine de caractère
return:
	Byte
*/
Byte indirectRegisterToByte(char *str, int *offset);

/*
Description:
  enlève les especes de début et de fin d'une chaine de caractère "    ADD $1, $2, $3   " -> "ADD $1, $2, $3"
paramètre:
	instr - chaine de caractères
return:
	void
*/
void del_espace(char *instr);

/*
Description:
  enlève les espaces au début, à la fin et au milieu d'une chaine : "    ADD $1, $2, $3   " -> "ADD $1,$2,$3"

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