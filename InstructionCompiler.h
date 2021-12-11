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
	avec le type et la taille des blocks de donnée de l'instruction
parametre:
	textInstruction - chaine de caractère contenan l'instruction
	instruction - la struct inistruction qui recoit les informations déja initialiser
return:
	void
erreur:
	si la chaine de caractère ne fini pas par \0
*/
void textInstructionToOpcode(char* textInstruction, Instruction* instruction);

/*
Description:
	prend en paramètre une instruction et la chaine de caractère de l'instruction avec ses
	opérandes et remplis les opérandes dans le code de la struct instruction
parametre:
	inst - l'instruction a remplir
	inst_str - le texte qui continen l'instruction 
return:
	void
*/
void writeInstructionOperands(Instruction *inst, char *isnt_str);

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
void setNormalOpCode(Instruction *instruction, Byte opCode);

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
void setSpecialOpCode(Instruction *instruction, Byte opCode);

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
void setBlocksSize(Instruction* instruction, Byte b0,Byte b1,Byte b2, Byte b3,Byte b4,Byte b5,Byte b6,Byte b7);

/*
Description:
	prend en parametre une instruction dont les tailles b[n] sont initialisées, et un numéro de block
	la valeur value de dim octet sera écrite dans le code de la struct Instruction dans zone qui luis est associée
	avec la taille correspondante dans b[n].
parametre:
	instruction - l'insqtruction dont les valeurs b[n] sont intialisée ainsi que le code ou la zone dédiée
	a la valeur qu'on veut écrire soit vide de données
	field - le numéro de block qu'on souhaite remplire
	value - la valeur que le block va prendre tableau de 8*dim bit soit dim char tell que 8*dim > b[field]
	dim - taille du tableau value en octet
erreurs:
	si instruction n'est pas initialisée, alors il y a risque de crash aussi la zone dans le code de 
	l'instruction ou l'on veut écrire doit nécéssairement etre vide en risque de corrompre tout le code.
	la taille de value ne provoquera pas d'exeption, la fonction tronquera le nombre automatiquement.
*/
void pasteValue(Instruction* instruction, int field,Byte* value,int dim);

/*
Description:
	transforme une chaine de caractères en lower case
parametre:
	text - chaine de caractère se finissant par \0
return:
	void
erreur:
	si la chaine n'est pas terminée par \0
*/
void toLowerCase(char *text);

/*
Description:
	place les arguments dans une matrice de caracteres
parametre:
	tab - tableu bidimensionnel de char
	instruction - pointeur vers char
return:
	void
*/
void param_to_tab(char tab[8][16],char *instruction);

/*
Description:
	transforme une chaine de caractere en binaire  -> "11" = 11
parametre:
	val - chai,e de caractere
return:
	Byte
*/
Byte registerToByte(char *val);


/*
Description:
  retourne la valeur du registre et met dans offset la valeur de l'offset.
  example si on a "200($1)" => offset=200 et return = 1
parametre:
	str - chaine de caractere
	offset - chaine de caractere
return:
	Byte
*/
Byte indirectRegisterToByte(char *str, int *offset);

/*
Description:
  enleve les especes de debut de de fin d'une chaine de caractere "    ADD $1, $2, $3   " -> "ADD $1, $2, $3"
parametre:
	instr - chaine de caractere
return:
	void
*/
void del_espace(char *instr);

/*
Description:
  enleve les especes l'operation et les espaces dans  "    ADD $1, $2, $3   " -> "ADD $1,$2,$3"

parametre:
	instr - chaine de caractere
return:
	void
*/
void format_instr(char *instr);

/*
Description:
	initialise instruction
parametre:
	instruction a initialiser
return:
	void
*/
void initInst(Instruction *inst);
#endif