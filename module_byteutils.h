#ifndef BYTE_UTILS_LIB
#define BYTE_UTILS_LIB
#include <stdio.h>
#include <stdlib.h>
#include "mips_structures.h"

#define DEBUG_BYTEUTILS 0

/*
Description:
	Deplace vers la gauche les bits de la chaine values n fois
paramètres:
	values - un tableau d'octets representant le nombre binaire
	n - le nombre de fois qu'on fait le deplacement vers la gauche
	size - la taille en octets de la chaine
erreur:
	si size ne correspond pas a la taille du tableau values
*/
void shiftLDWord(DWord values, int n);

/*
Description:
	Deplace vers la droite les bits de la chaine values n fois
parametres:
	values - un tableau d'octet representant le nombre binaire
	n - le nombre de fois qu'on fait le deplacement vers la droite
	size - la taille en octets de la chaine
erreur:
	si size ne correspond pas a la taille du tableau values
*/
void shiftRDWord(DWord values, int n);

/*
Description:
	convertis une chaine de caractère en un entier sur 8bit non signe et gère les erreurs
return:
	si la chaine de caractère a ete traduite correctement
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
	convertit une chaine de caractère de forme 0xHEXA en un entier signe (int)
paramètre:
	str - la chaine a convertir
	converted - le resultat
return:
	si l'entier a ete convertit avec succes
*/
int HexStringToInteger(char *str,int *converted);

/*
Description:
	convertit une chaine de caractère representant un entier signe en un type C int
paramètre:
	str - la chaine a convertir
	converted - le resultat
return:
	si l'entier a ete convertit avec succes
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
	incremente la valeur stockee dans le tableau de 1
return:
	void
erreurs:
	si la taille ne corespond pas
*/
int incr(DWord word);

/*
Description:
	incremente la valeur stockee dans le tableau de 4
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
Description:
	retourne le signe du DWord -1 ou 1
erreurs:
	si word nul
*/
int signDWord(DWord word);

/*
Description:
	fait l'operation b1 <- b1 + b2 et retourne l'overflow
parametre:
	b1 - premier DWord
	b2 - DWord a ajouter
return:
	0 si pas d'overflow. 1 ou -1 si overflow positif ou negatif
erreurs:
	si b1 ou b2 nul*/
int addDWord(DWord b1,DWord b2);

/*
Description:
	convertit le nombre binaire avec le complement a deux
return:
	si le calcul du complement a deux produit un overflow
erreurs:
	si value nul*/
int twoComplementDWord(DWord value);

/*
Description:
	fait l'operation b1 <- b1 - b2 et retourne l'overflow
parametre:
	b1 - premier DWord
	b2 - DWord a ajouter
return:
	0 si pas d'overflow. 1 ou -1 si overflow positif ou negatif
erreurs:
	si b1 ou b2 nul*/
int subDWord(DWord b1,DWord b2);

/*
retourne si word>0
*/
int greaterThanZeroDWord(DWord word);

/*
retourne le resultat de l'operation b1==b2
*/
int equalsDWord(DWord b1,DWord b2);
/*
Description:
	q ← b1 div b2
	LO ← q
	r ← b1 mod b2
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

/*convertis un DWord en un long*/
long DWordToLong(DWord word);

#endif