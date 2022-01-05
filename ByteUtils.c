#include "ByteUtils.h"


/*
fait un déplacement binaire vers la gauche de maximum 8bit. 
Opération élémentaire sur un DWord
*/
void shiftL8Bit(DWord values, int n){
	int i;
	values[0]=values[0]<<n;

	for(i=1;i<4;i++){
		values[i-1] += values[i] >> (8-n);
		values[i]=values[i]<<n;
	}
}

void shiftLDWord(DWord values, int n){
	int shift=n,i;
	while(shift > 8){
		for(i=1;i<4;i++){
			values[i-1]=values[i];
		}
		values[i-1]=0;
		shift-=8;
	}

	shiftL8Bit(values, shift);
}

/*
fait un déplacement binaire vers la droite de maximum 8bit. 
Opération sur un DWord
*/
void shiftR8Bit(DWord values, int n){
	int i;
	values[3]=values[3]>>n;

	for(i=3;i>0;i--){
		values[i] += values[i-1] << (8-n);
		values[i-1]=values[i-1]>>n;
	}
}

void shiftRDWord(DWord values, int n){
	int shift=n,i;
	while(shift > 8){
		for(i=3;i>0;i--){
			values[i]=values[i-1];
		}
		values[i]=0;
		shift-=8;
	}

	shiftR8Bit(values, shift);
}

int StringToByte(char *str,Byte *result){
	int success=1,index=0;
	unsigned int sum=0;
	/*si index>=3 alors nombre sur 4 digit trop grand pour un Byte  (255)*/
	while(str[index]>='0' && str[index]<='9' && index<3){
		sum=sum*10+str[index]-'0';
		index++;
	}

	/*si on a pas lus la chaine en entier ou que la somme dépasse le max*/
	if(str[index]!= '\0' || sum>255){
		success = 0;
	}else{
		*result=sum;/*pas d'overflow car <255*/
		if(DEBUG_BYTEUTILS) printf("[conversion] result conversion str %s to int: %d\n", str, sum);
	}

	return success;
}

void IntegerToDWord(int i,DWord res){
	int weight = 0x8000; /*1000 0000 0000 0000*/

	while(weight > 0){
		/*décale vers la gauche le résultat car on ajoute le nouveau bit de poids faible*/
		shiftLDWord(res, 1); 

		/* alors le bit est à 1 sinon 0*/
		if((i&weight) != 0){
			res[3]++;
		}
		
		weight = weight>>1;
	}
}

int StringToSignedInteger(char *str,int *converted){
	char *cp = str;
	int res=0,sign=1,index=0;
	int success = cp[0]!='\0';

	if(cp[0]=='-'){
		sign=-1;
		cp++;
	}

	while(cp[index] != '\0' && success){
		res = res*10 + (cp[index]-'0');
		success = cp[index]>='0' && cp[index]<='9';
		index++;
	}
	if(success){
		*converted=sign*res;
		if(DEBUG_BYTEUTILS) printf("[conversion] result conversion str to int: %d\n", sign*res);
	}
	return success;
}

int HexStringToInteger(char *str,int *converted){
	char *cp = str;
	int index=0;
	int res=0,buffer=0;
	int success=str[0]!='\0';

	while(cp[index] != '\0' && success){
		switch(cp[index]){
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
				if(cp[index]<='9' && cp[index]>='0'){
					buffer=(*cp)-'0';
				}else{
					success=0;
				}
				
				break;
		}
		res = res<<4;
		res += buffer;
		
		index++;
	}
	if(success){
		*converted=res;
	}
	return success;
}

Byte addIntegerToDWord(int integer,DWord word){
	DWord temp;

	IntegerToDWord(integer,temp);
	return addDWord(word,temp);
}

int incr(DWord word){
	DWord one = {0x00,0x00,0x00,0x01};
	return addDWord(word,one);
}

int decr(DWord word){
	DWord minus_one = {0xFF,0xFF,0xFF,0xFF};
	return addDWord(word,minus_one);
}

void incr4(DWord word){
	incr(word);
	incr(word);
	incr(word);
	incr(word);
}

void copyDWord(DWord dst,DWord src){
	src[0]=dst[0];
	src[1]=dst[1];
	src[2]=dst[2];
	src[3]=dst[3];
}

int signDWord(DWord word){
	return (word[0]&0x80)==0?1:-1;
}

/*b1 <- b1+b2*/
int addDWord(DWord b1,DWord b2){
	unsigned int sum=0;
	int i,overflow=0;
	if(signDWord(b1)==signDWord(b2)){
		if(signDWord(b1)==1){
			overflow=1;
		}else{
			overflow=-1;
		}
	}
	for(i=3;i>=0;i--){
		sum += b1[i]+b2[i];
		b1[i] = sum&0xFF;  /*new byte value*/
		if((sum & 0xF00) != 0){ /*overflow on the Byte*/
			sum=1;
		} 
	}
	return overflow * (sum?1:0); /*return si la somme fait un overflow et meme signe*/
}

/*value <- -value*/
int twoComplementDWord(DWord value){
	int i;
	for(i=3;i>=0;i--){
		value[i]=~value[i];
	}
	return incr(value);
}

/*b1 <- b1-b2*/
int subDWord(DWord b1,DWord b2){
	DWord cp;
	copyDWord(cp,b2);
	twoComplementDWord(cp);
	return addDWord(b1,cp);
}

int greaterThanZeroDWord(DWord word){
	int b1greater;

	if((word[0]|word[1]|word[2]|word[3]) == 0){
		/*cas word==0*/
		b1greater=0;
	}else{
		if(signDWord(word)==1){
			/*word>0*/
			b1greater=1;
		}else{
			/*word<0*/
			b1greater=0;
		}
	}

	return b1greater;
}

int equalsDWord(DWord b1,DWord b2){
	int i=0;
	while(b1[i]==b2[i] && i<4){
		i++;
	}
	return i==4;
}

void divideDWord(DWord HI,DWord LO,DWord b1,DWord b2){

}

void multiplyDWord(DWord HI,DWord LO,DWord b1,DWord b2){

}