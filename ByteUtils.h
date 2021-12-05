#ifndef BYTE_UTILS_LIB
#define BYTE_UTILS_LIB
#include <stdio.h>
#include <stdlib.h>
typedef unsigned char Byte;

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
	Déplace vers la droite les bits de la chaine values n fois
parametres:
	values - un tableau d'octet représentant le nombre binaire
	n - le nombre de fois qu'on fait le déplacement vers la droite
	size - la taille en octet de la chaine
*/
void shiftRNBit(Byte *values, int n, int size);

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

#endif