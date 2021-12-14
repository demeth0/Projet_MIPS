#include "ByteUtils.h"


/*
fait un déplacement binaire vers la gauche de maximum 8bit. 
Opération élémentaire sur un tableau de Byte
*/
void shiftL8Bit(Byte *values, int n, int size){
	int i;
	values[0]=values[0]<<n;

	for(i=1;i<size;i++){
		values[i-1] += values[i] >> (8-n);
		values[i]=values[i]<<n;
	}
}

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
fait un déplacement binaire vers la droite de maximum 8bit. 
Opération élémentaire sur un tableau de Byte
*/
void shiftR8Bit(Byte *values, int n, int size){
	int i;
	values[size-1]=values[size-1]>>n;

	for(i=size-1;i>0;i--){
		values[i] += values[i-1] << (8-n);
		values[i-1]=values[i-1]>>n;
	}
}

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

void IntTo2ByteArray(int i,Byte *res){
	int cp = i;
	int weight = 0x8000; /*1000 0000 0000 0000*/

	while(weight > 0){
		shiftLNBit(res, 1, 2); /*décale vers la gauche le résultat car on ajoute le nouveau bit de poids faible*/
		/* alors le bit est à 1 sinon 0*/
		if((cp&weight) != 0){
			cp=cp&(~weight);
			res[1]++;
		}
		
		weight = weight>>1;
	}
}

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

int ImmediatStrToInteger(char *str){
	int res;
	if(str[0]=='0' && str[1]=='x'){
		res=HexStrIntegerToInt(str+2);
	}else{
		res = SignedStrIntegerToInt(str);
	}
	return res;
} 

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

void setByte(Byte data[4],int value){
	data[0]=0;
	data[0]=0;
	data[0]=0;
	data[0]=0;
	addToByte(data,value);
}

void addToByte(Byte data[4],int value){
	Byte overflow=0;
	Byte extracted = value & 0xFF; /*8 premiers bits*/
	int index=3;
	int cp_value = value;
	unsigned int temp=0;

	/*tant que valeur non null (pas finit d'écrire) et pas dépasser taille d'un
	int (2 ou 4) et pas dépasser taille data*/
	while(index>=0){
		/*on regarde si l'addition des bits de value avec l'overflow précédent et data ne provoquent pas de dépassement*/
		temp = data[index]+extracted+overflow;
		if((temp&(0xF00))!=0){
			data[index]=temp&0xFF;	
			/*au plus égal a 1 car 255+255 fait 1 bit d'overflow*/
			overflow=1;
		}else{
			data[index]+=extracted+overflow;
			overflow = 0;
		}

		index--;
		cp_value = cp_value>>8;
		extracted = cp_value & 0xFF;
	}	
}
