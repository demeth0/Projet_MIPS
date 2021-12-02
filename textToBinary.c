#include "textToBinary.h"


/*
On suppose :
	le texte instruction est valide
	la struct Instruction est initialisée
*/
void textInstructionToOpcode(char* textInstruction, Instruction *instruction){
	char opcode[8];
	getOperationCodeText(textInstruction,opcode);

	if(opcode[1]=='\0' && opcode[0]=='j'){
		/* alors forcément instruction 'j' 
			j -> 6 premiers bits a 000010
			donc code[0] = 000010xx
		*/
		instruction->code[0] = (instruction->code[0]&0x03) + (J_CODE<<2);
		instruction->id = J_ID;

		setBlocksSize(instruction,6,26,0,0,0,0,0,0);
	}else if(opcode[2]=='\0'){
		/*
			or
			lw
			jr
			sw
		*/
	}else if(opcode[3]=='\0'){
		/*
		add
		and
		beq
		bne
		div		
		jal
		lui
		nop
		sll
		slt
		srl
		sub
		xor*/
	}else if(opcode[4]=='\0'){
		/*rotr
		addi
		bgtz
		blez
		mfhi
		mflo
		mult*/
	}else{
		/* syscall */
	}
}

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
void setNormalOpCode(Instruction *instruction, Byte opCode){
	instruction->code[0] = (instruction->code[0]&0x03) + (opCode<<2);
}

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
void setSpecialOpCode(Instruction *instruction, Byte opCode){
	instruction->code[3] = (instruction->code[0]&0xC0) + opCode;
	setNormalOpCode(instruction,0);
}

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
void getOperationCodeText(char* textInstruction, char* res){
	int i=0;
	while(textInstruction[i]!=' '){
		res[i]=textInstruction[i];
		i++;
	}
	res[i]='\0';
}


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
void setBlocksSize(Instruction* instruction, Byte b0,Byte b1,Byte b2, Byte b3,Byte b4,Byte b5,Byte b6,Byte b7){
	instruction->b[0]=b0;
	instruction->b[1]=b1;
	instruction->b[2]=b2;
	instruction->b[3]=b3;
	instruction->b[4]=b4;
	instruction->b[5]=b5;
	instruction->b[6]=b6;
	instruction->b[7]=b7;
}

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
void pasteValue(Instruction* instruction, int field,Byte* value,int dim){
	/* size in bit of the value not necessarly a multiple of 8 */
	int size=instruction->b[field];
	/* position wanted on the instruction code */
	int pos = 0,i=0;

	/* temp values */
	Byte val[4];
	Byte mask1[4];
	Byte mask2[4];

	/* calculate position in bit in the 32bit code */
	for(i=0;i<field;i++){
		pos += instruction->b[i];
	}

	/* init mask at 0xFFFFFFFF and val at 0x00000000 */
	for(i=0;i<4;i++){
		mask1[i] = 0xFF;
		mask2[i] = 0xFF;
		val[i]=0;
	}

	/* met value dans val (taille value =< 4) */
	for(i=0;i<dim;i++){
		printf("%02x\n", value[i]);
		/* on doit écrire a la fin car les shift sont appliqués dans le sens 
		inverse aux indices des tableaux */
		val[i+(4-dim)]=value[i];
	}

	/* positionne la valeur a la fin tel que le bit de poid fort soit le premier dans la chaine */
	shiftLNBit(val, 32-size, 4);
	/* décale val pour etre a la bonne position dans le code */
	shiftRNBit(val, pos,4);

	/* calcul les mask pour l'insertion dans le code */
	shiftRNBit(mask1, pos,4);
	shiftLNBit(mask2, 32-(pos+size), 4);

	/* insertion dans le code */
	for(i=0;i<4;i++){
		instruction->code[i] += mask1[i] &mask2[i]&val[i];
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