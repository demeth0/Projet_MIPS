#include "ByteUtils.h"

/*
Description:
	Déplace vers la gauche les bits de la chaine values n fois avec n entre 0 et 8
parametres:
	values - un tableau d'octet représentant le nombre binaire
	n - le nombre de fois qu'on fait le déplacement vers la gauche
	size - la taille en octet de la chaine
*/
void shiftL8Bit(Byte *values, int n, int size){
	int i;
	values[0]=values[0]<<n;

	for(i=1;i<size;i++){
		values[i-1] += values[i] >> (8-n);
		values[i]=values[i]<<n;
	}
}

/*
Description:
	Déplace vers la gauche les bits de la chaine values n fois
parametres:
	values - un tableau d'octet représentant le nombre binaire
	n - le nombre de fois qu'on fait le déplacement vers la gauche
	size - la taille en octet de la chaine
*/
void shiftLNBit(Byte *values, int n, int size){
	int shift=n,i;
	while(shift > 8){
		for(i=1;i<size;i++){
			values[i-1]=values[i];
		}
		values[i-1]=0;
		shift-=8;
	}

	shiftL8Bit(values, shift, size);
}

/*
Description:
	Déplace vers la droite les bits de la chaine values n fois avec n entre 0 et 8
parametres:
	values - un tableau d'octet représentant le nombre binaire
	n - le nombre de fois qu'on fait le déplacement vers la droite
	size - la taille en octet de la chaine
*/
void shiftR8Bit(Byte *values, int n, int size){
	int i;
	values[size-1]=values[size-1]>>n;

	for(i=size-1;i>0;i--){
		values[i] += values[i-1] << (8-n);
		values[i-1]=values[i-1]>>n;
	}
}

/*
Description:
	Déplace vers la droite les bits de la chaine values n fois
parametres:
	values - un tableau d'octet représentant le nombre binaire
	n - le nombre de fois qu'on fait le déplacement vers la droite
	size - la taille en octet de la chaine
*/
void shiftRNBit(Byte *values, int n, int size){
	int shift=n,i;
	while(shift > 8){
		for(i=size-1;i>0;i--){
			values[i]=values[i-1];
		}
		values[i]=0;
		shift-=8;
	}

	shiftR8Bit(values, shift, size);
}

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
void IntTo2ByteArray(int i,Byte *res){
	int cp = i;
	int weight = 0x8000;

	while(weight > 0){
		shiftLNBit(res, 1, 2); /*décale vers la gauche le resultat car on ajoute le nouveau bit de poid faible*/
		/*printf("weight: %d, cp: %d\n",weight,cp);*/
		/* alors le bi est a 1 sinon 0*/
		if(cp-weight >=0){
			cp=cp-weight;
			res[1]++;
		}
		
		weight = weight>>1;
	}
}

/*
Description:
	convertis un entier signé dans une chaine de caractere en sa valeur décimale
parametre:
	str - la chaine de caractère finissant par \0
return:
	res - la valeur décimale signée
*/
int SignedStrIntegerToInt(char *str){
	char *cp = str;
	int res=0;
	int sign=1;
	if((*cp)=='-'){
		sign=-1;
		cp++;
	}
	while((*cp) != '\0'){
		res = res*10 + ((*cp)-'0');
		cp++;
	}

	return sign*res;
}

/*
Description:
	convertit un entier hexadécimal en sa valeur décimale signée
paramètre:
	str - lower case string of the value
return:
	res - la valeur décimale signée de la chaine
*/
int HexStrIntegerToInt(char *str){
	char *cp = str;
	int res=0;
	int buffer=0;
	while((*cp) != '\0'){
		switch((*cp)){
			case 'a':
				buffer=0xA;
				break;
			case 'b':
				buffer=0xB;
				break;
			case 'c':
				buffer=0xC;
				break;
			case 'd':
				buffer=0xD;
				break;
			case 'e':
				buffer=0xE;
				break;
			case 'f':
				buffer=0xF;
				break;
			default:
				buffer=(*cp)-'0';
				break;
		}
		res = res<<4;
		res += buffer;
		
		cp++;
	}

	return res;
}

/*
Description:
	prend en parametre une chaine de caractère représentant une valeur immédiate
	en hexadécimale ou décimale et la converit en un Integer
parametre:
	str - chaine de caractère finissant par \0
return:
	la valeur décimale signée de la chaine
*/
int ImmediatStrToInteger(char *str){
	int res;
	if(str[0]=='0' && str[1]=='x'){
		res=HexStrIntegerToInt(str+2);
	}else{
		res = SignedStrIntegerToInt(str);
	}
	return res;
} 

/*
Description:
	incrémente le nombre de 1
parametre:
	tab - tableau d'octet représentant un nombre
	size - taille en octet du nombre
return:
	void
*/
void incr(Byte *tab, int size){
	int index=size-1;
	while(tab[index]==0xFF && index >= 0){
		tab[index]=0;
		index--;
	}
	if(index >= 0){
		tab[index]++;
	}
}


void incr4(Byte *tab, int size){
	incr(tab,size);
	incr(tab,size);
	incr(tab,size);
	incr(tab,size);
}