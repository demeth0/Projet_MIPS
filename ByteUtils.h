#ifndef BYTE_UTILS_LIB
#define BYTE_UTILS_LIB
#include <stdio.h>
#include <stdlib.h>
typedef unsigned char Byte;

/*
Description:
	Déplace vers la gauche les bits de la chaine values n fois
paramètres:
	values - un tableau d'octets représentant le nombre binaire
	n - le nombre de fois qu'on fait le déplacement vers la gauche
	size - la taille en octets de la chaine
erreur:
	si size ne correspond pas a la taille du tableau values
*/
void shiftLNBit(Byte *values, int n, int size);

/*
Description:
	Déplace vers la droite les bits de la chaine values n fois
parametres:
	values - un tableau d'octet représentant le nombre binaire
	n - le nombre de fois qu'on fait le déplacement vers la droite
	size - la taille en octets de la chaine
erreur:
	si size ne correspond pas a la taille du tableau values
*/
void shiftRNBit(Byte *values, int n, int size);

/*
Description:
	convertit un entier dans un integer en un entier encoder sur un tableau de Byte de taille 2
parametre:
	res - tableau de Byte de taille nécessairement 2
	i - entier sur integer
return:
	void
erreur:
	si res ne fait pas une taille de 2 
*/
void IntTo2ByteArray(int i,Byte *res);

/*
Description:
	prend en paramètre une chaine de caractères représentant une valeur immédiate
	en hexadécimale ou décimale et la converit en un Integer
parametre:
	str - chaine de caractères finissant par \0
return:
	la valeur décimale signée de la chaine
*/
int ImmediatStrToInteger(char *str);

/*
Description:
	prend une valeur encodée sur 4 Byte et y ajoute la valeur value signée.
	prend en compte les débordements.
paramètre:
	data - le tableau de 4 Byte ou serra ajouter value (les octets sont en little-endian)
	value - entier signé a ajouter
erreur:
	aucunes
*/
void addToByte(Byte data[4],int value);

#endif