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
void setNormalOpCode(Instruction *instruction, char opCode){
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
void setSpecialOpCode(Instruction *instruction, char opCode){
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
void setBlocksSize(Instruction* instruction, char b0,char b1,char b2, char b3,char b4,char b5,char b6,char b7){
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
	on passe en parametre un numéro de field et une instruction, l'algo écrit la valeur passée en parametre dans l'espace
préciser par le field, la valeur écrite est écrite en tant que nombre binaire de 5 bits (registre)

parametres:
	instruction - l'objet instruction
	field - le numéro du bloc dans l'instruction
	value - la valeur a écrire de 5bit
*/
void pasteReg(Instruction* instruction, int field,char value){
	int size=instruction->b[field];
	int index,used,pos=0,i;
	int reg = value<<3;
	if(size == 5){
		for(i=0;i<field;i++){
			pos += instruction->b[i];
		}
		/* index du premier octet a éditer */
		index = pos/8;
		
		/*
		pos%8 = nb de bit réservé a d'autres fields dans l'octet
		nb of bit left to write on in instruction */
		used = pos%8;

		if(used == 0){
			instruction->code[index] += reg;
		}else{
			instruction->code[index] += (reg >> used);
			if(used > 3){
				instruction->code[index+1] += reg << (8-used);
			}
		}
	}
}