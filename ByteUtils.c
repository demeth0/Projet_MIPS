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
	dst[0]=src[0];
	dst[1]=src[1];
	dst[2]=src[2];
	dst[3]=src[3];
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
		} else{
			sum=0;
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

int sign64bits(unsigned char word[8]){
	return (word[0]&0x80)==0?1:-1;
}
/*b1 <- b1+b2*/
int add64bits(unsigned char b1[8],unsigned char b2[8]){
	unsigned int sum=0;
	int i,overflow=0;
	if(sign64bits(b1)==sign64bits(b2)){
		if(sign64bits(b1)==1){
			overflow=1;
		}else{
			overflow=-1;
		}
	}
	for(i=7;i>=0;i--){
		sum += b1[i]+b2[i];
		b1[i] = sum&0xFF;  /*new byte value*/
		if((sum & 0xF00) != 0){ /*overflow on the Byte*/
			sum=1;
		} 
		else
			sum=0;
	}
	return overflow * (sum?1:0); /*return si la somme fait un overflow et meme signe*/
}

int decalage_64bits_par1(unsigned char tableau[8]){ /*decale de 1 tous les bits sur un tableau de 64 bits*/

	int compteur =7;
	int poids_fort =0;
	int overflow =0;
	unsigned char temp;
	while(compteur>=0){
		temp = tableau[compteur];
		tableau[compteur] = (tableau[compteur]<<1) + poids_fort;
		poids_fort = (temp & 0x80)>>7;
		compteur--;
	}

	if (poids_fort!=0)
		overflow=1;

	return overflow;
}

void multiplyDWord(DWord HI,DWord LO,DWord b1,DWord b2){
	unsigned char bit;
	int  parseur=0;
	int  compteur=3;
	unsigned char result[8];
	unsigned char addition[8];

	int i =0;
	char masque;
	int x;
	int decalage =64;
	
	for (i = 0;i<8;i++){
		result[i]=0;
		addition[i] =0;
	}
	while(compteur>=0){ /*on parcourt le premier nombre*/
		parseur =7;
		while(parseur>=0){ /*on parcourt chaque bit*/
			masque = 0x01<<parseur;
			bit = (b1[compteur] & masque)==0?0:1;
			if(bit == 1){
				/*on applique le decalage sur b1*/
				/*on recopie b1 dans quelque chose de plus grand*/
				addition[7] = b2[3];
				addition[6] = b2[2];
				addition[5] = b2[1];
				addition[4] = b2[0];
				addition[3] = addition[2] = addition[1]=addition[0] = 0;
				/*on decale*/
				x =0;
				while (x<(parseur + (3-compteur)*8)){
					decalage_64bits_par1(addition);
					x++;
				}
				add64bits(result,addition);
			}
			decalage--;
			parseur--;
		}
		compteur--;		
	}
}
void divideDWord(DWord HI,DWord LO,DWord b1,DWord b2){
	int sign_b1,sign_b2,i;
	DWord num,den;
	DWord tester;

	tester[0]=0;
	tester[1]=0;
	tester[2]=0;
	tester[3]=0;

	LO[0]=0;
	LO[1]=0;
	LO[2]=0;
	LO[3]=0;
	HI[0]=0;
	HI[1]=0;
	HI[2]=0;
	HI[3]=0;

	sign_b1=signDWord(b1);
	sign_b2=signDWord(b2);

	copyDWord(num,b1);

	if(sign_b1==-1){
		twoComplementDWord(num);
	}

	copyDWord(den,b2);
	if(sign_b2==1){
		twoComplementDWord(den);
	}/*on obtient un denominateur négatif*/
	 /*pour faire la soustraction*/
	
	/*on aura dans num=abs(b1) et den=-abs(b2)*/
	/*printf("num: %02x%02x %02x%02x\n", num[0],num[1],num[2],num[3]);
	printf("den: %02x%02x %02x%02x\n", den[0],den[1],den[2],den[3]);*/

	for(i=0;i<32;i++){
		/*printf("bit %d\n", i);
		printf("tester: %02x%02x %02x%02x\n", tester[0],tester[1],tester[2],tester[3]);*/

		/*on récupere le plus gros bit de poid fort*/
		tester[3]+=((num[0]&0x80)==0)?0:1;

		/*on soustrait */
		/*overflow impossible val positive - val positive ne peut pas dépasser -255 si les deux valeurs sont bornées a 255*/
		addDWord(tester,den);
		
		/*si bits de poid forts-abs(b2) est supérieur a zero alors*/
		if(greaterThanZeroDWord(tester) || (tester[0]|tester[1]|tester[2]|tester[3])==0){
			/*rajoute 1 au résultat*/
			shiftLDWord(LO,1);
			LO[3]+=1;
			
		}else{
			/*rajoute zero au résultat*/
			shiftLDWord(LO,1);
			/*restaure la valeur de départ extraite pour la garder positive*/
			twoComplementDWord(den);
			addDWord(tester,den);
			twoComplementDWord(den);
		}
		/*printf("LO: %02x%02x %02x%02x\n", LO[0],LO[1],LO[2],LO[3]);*/
		/*passe au digit de poid fort suivant*/
		shiftLDWord(num,1);
		shiftLDWord(tester,1);
	}

	/*a la fin de la boucle, dans tester=r<<1*/
	/*printf("reste: %02x%02x %02x%02x\n", tester[0],tester[1],tester[2],tester[3]);*/
	shiftRDWord(tester,1);
	copyDWord(HI,tester);
	

	/*si -a/b ou a/-b alors q est négatif*/
	if(sign_b1*sign_b2 == -1){
		twoComplementDWord(LO);
	}
}