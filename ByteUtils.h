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
void shiftLDWord(DWord values, int n);

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
void shiftRDWord(DWord values, int n);

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
	convertit un entier dans un integer en un entier encoder sur un DWord
parametre:
	res - DWord qui va stocker la valeur de i
	i - entier a convertir
return:
	void
erreurs:
	si res nul
*/
void IntegerToDWord(int i,DWord res);

/*
Description:
	convertit une chaine de caractère de forme 0xHEXA en un entier signé (int)
paramètre:
	str - la chaine a convertir
	converted - le résultat
return:
	si l'entier a été convertit avec succés
*/
int HexStringToInteger(char *str,int *converted);

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
	fait word <- word + DWord(integer)
paramètre:
	integer - valeur a ajouter
	word - dest
return:
	l'overflow ou la carry
erreurs:
	si word nul
*/
Byte addIntegerToDWord(int integer,DWord word);

/*
Description:
	fait word-1
erreurs:
	si word nul
*/
int decr(DWord word);

/*
Description:
	incrémente la valeur stockée dans le tableau de 1
return:
	void
erreurs:
	si la taille ne corespond pas
*/
int incr(DWord word);

/*
Description:
	incrémente la valeur stockée dans le tableau de 4
return:
	void
erreurs:
	si la taille ne corespond pas
*/
void incr4(DWord word);

/*
Description:
	copie src dans dst
parametre:
	dst - DWord de destination
	src - DWord a copier
erreurs:
	si dst ou src nul
*/
void copyDWord(DWord dst,DWord src);

/*
Descccription:
	retourne le signe du DWord -1 ou 1
erreurs:
	si word nul
*/
int signDWord(DWord word);

/*
Description:
	fait l'opération b1 <- b1 + b2 et retourne l'overflow
parametre:
	b1 - premier DWord
	b2 - DWord a ajouter
return:
	0 si pas d'overflow. 1 ou -1 si overflow positif ou négatif
erreurs:
	si b1 ou b2 nul*/
int addDWord(DWord b1,DWord b2);

/*value <- -value*/
int twoComplementDWord(DWord value);

/*
Description:
	fait l'opération b1 <- b1 - b2 et retourne l'overflow
parametre:
	b1 - premier DWord
	b2 - DWord a ajouter
return:
	0 si pas d'overflow. 1 ou -1 si overflow positif ou négatif
erreurs:
	si b1 ou b2 nul*/
int subDWord(DWord b1,DWord b2);

/*
retourne si word>0
*/
int greaterThanZeroDWord(DWord word);

/*
retourne le résultat de l'opération b1==b2
*/
int equalsDWord(DWord b1,DWord b2);
/*
Description:
	q ← GPR[rs]31..0 div GPR[rt]31..0
	LO ← q
	r ← GPR[rs]31..0 mod GPR[rt]31..0
	HI ← r
erreurs:
	si un des paramètres est nul
*/
void divideDWord(DWord HI,DWord LO,DWord b1,DWord b2);

/*
Description:
	prod ← GPR[rs]31..0 × GPR[rt]31..0
	LO ← prod31..0
	HI ← prod63..32
erreurs:
	si un des paramètres est nul
*/
void multiplyDWord(DWord HI,DWord LO,DWord b1,DWord b2);


#endif