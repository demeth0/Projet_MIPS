#ifndef BYTE_UTILS_LIB
#define BYTE_UTILS_LIB
#include <stdio.h>
#include <stdlib.h>
#include "mips_structures.h"

#define DEBUG_BYTEUTILS 1

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
	convertis une chaine de caractère en un entier sur 8bit non signé et gère les erreurs
return:
	si la chaine de caractère a été traduite correctement
erreurs:
	si str ou result sont NULL
*/
int StringToByte(char *str,Byte *result);

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
void IntegerTo2ByteArray(int i,Byte *res);

/*
Description:
	convertit une chaine de caractère de forme 0xHEXA en un entier signé (int)
paramètre:
	str - la chaine a convertir
	converted - le résultat
return:
	si l'entier a été convertit avec succés
*/
int HexStrIntegerToInt(char *str,int *converted);

/*
Description:
	convertit une chaine de caractère représentant un entier signé en un type C int
paramètre:
	str - la chaine a convertir
	converted - le résultat
return:
	si l'entier a été convertit avec succés
*/
int StringToSignedInteger(char *str,int *converted);

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

/*
Description:
	écrit la valeur dans le nombre de 32 bit codé dans la tableau data
parametre:
	data - tableau ou le nombre est encodé
	value - valeur a encoder dans le tableau
*/
void setByte(Byte data[4],int value);

/*
Description:
	incrémente la valeur stockée dans le tableau de 1
return:
	void
erreurs:
	si la taille ne corespond pas
*/
void incr(Byte *tab, int size);

/*
Description:
	incrémente la valeur stockée dans le tableau de 4
return:
	void
erreurs:
	si la taille ne corespond pas
*/
void incr4(Byte *tab, int size);

#endif