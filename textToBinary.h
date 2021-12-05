#ifndef TEXT_TO_BINARY
#define TEXT_TO_BINARY

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operation_code_defines.h"

typedef unsigned char Byte;

typedef struct Instruction{
	Byte code[4];
	unsigned int id;
	Byte b[8];
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

void writeInstructionOperands(Instruction *inst, char *isnt_str);

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
	Déplace vers la gauche les bits de la chaine values n fois
parametres:
	values - un tableau d'octet représentant le nombre binaire
	n - le nombre de fois qu'on fait le déplacement vers la gauche
	size - la taille en octet de la chaine
*/
void shiftLNBit(Byte *values, int n, int size);

/*
Description:
	Déplace vers la gauche les bits de la chaine values n fois avec n entre 0 et 8
parametres:
	values - un tableau d'octet représentant le nombre binaire
	n - le nombre de fois qu'on fait le déplacement vers la gauche
	size - la taille en octet de la chaine
*/
void shiftL8Bit(Byte *values, int n, int size);

/*
Description:
	Déplace vers la droite les bits de la chaine values n fois
parametres:
	values - un tableau d'octet représentant le nombre binaire
	n - le nombre de fois qu'on fait le déplacement vers la droite
	size - la taille en octet de la chaine
*/
void shiftRNBit(Byte *values, int n, int size);

/*
Description:
	Déplace vers la droite les bits de la chaine values n fois avec n entre 0 et 8
parametres:
	values - un tableau d'octet représentant le nombre binaire
	n - le nombre de fois qu'on fait le déplacement vers la droite
	size - la taille en octet de la chaine
*/
void shiftR8Bit(Byte *values, int n, int size);

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
	convertis un entier dans un integer en un entier encoder sur un tableu de Byte de taille 2
parametre:
	res - tableu de Byte de taille nécéssairement 2
	i - entier sur integer
return:
	void
erreur:
	si res ne fait pas une taille de 2 
*/
void IntTo2ByteArray(int i,Byte *res);




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
	prend en parametre une chaine de caractère représentant une valeur immédiate
	en hexadécimale ou décimale et la converit en un Integer
parametre:
	str - chaine de caractère finissant par \0
return:
	la valeur décimale signée de la chaine
*/
int ImmediatStrToInteger(char *str);


/*
Description:
	transforme une chaine de caractere en binaire  -> "11" = 11
parametre:
	val - chai,e de caractere
return:
	Byte
*/
Byte registerToByte(char *val);
#endif