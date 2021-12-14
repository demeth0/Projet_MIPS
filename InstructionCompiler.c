#include "InstructionCompiler.h"


/*-------------------------------------*
 *		Définition méthodes privées	   *
 *-------------------------------------*/

/*
Description:
	met les 6 premiers bits de l'instruction avec opCode en conservant les valeurs d'origine pour le reste
paramètre:
	instruction - l'instruction en paramètre à changer
	opCode - le code instruction à insérer en début
return:
	void
error :
	si instruction n'est pas initialisée
*/
void setNormalOpCode(Instruction *instruction, Byte opCode);

/*
Description:
	met les 6 premiers bits de l'instruction a 0 et les 6 derniers avec opCode en conservant les valeurs d'origine pour le reste
paramètre:
	instruction - l'instruction en paramètre à changer
	opCode - le code instruction à insérer en fin
return:
	void
error :
	si instruction n'est pas initialisée
*/
void setSpecialOpCode(Instruction *instruction, Byte opCode);

/*
Description:
	Récupère tous les chars jusqu'au premier espace dans l'instruction en paramètre
et les met dans res. exemple pour 'addiu $val, $res, 10' va donner 'addiu\0' 
paramètre:
	textInstruction - l'instruction complète dans un tableau de char (fin \0)
	res - tableau de char qui va stocker l'instruction au moins de taille 8 (fin \0)
return:
	void
erreur:
	si res est trop petit ou textInstruction n'est pas valide
*/
void getOperationCodeText(char* textInstruction, char* res);

/*
Description:
	remplie les paramètres bloc de l'instruction
paramètre:
	Instruction* instruction - l'instruction qui va être modifiée
	char bn - les tailles des blocs
return: 
	void
erreur:
	si l'instruction n'est pas initialisée
*/
void setBlocksSize(Instruction* instruction, Byte b0,Byte b1,Byte b2, Byte b3,Byte b4,Byte b5,Byte b6,Byte b7);

/*
Description:
	prend en paramètre une instruction dont les tailles b[n] sont initialisées, et un numéro de bloc
	la valeur value de dim octet sera écrite dans le code de la struct Instruction dans zone qui lui est associée
	avec la taille correspondante dans b[n].
paramètre:
	instruction - instruction initialisée 
	field - le numéro de bloc qu'on souhaite remplir
	value - la valeur que le bloc va prendre codé sur 8*dim bit soit Byte[dim] tel que 8*dim > b[field]
	dim - taille du tableau value en octet
erreurs:
	si instruction n'est pas initialisée, alors il y a un risque de crash, aussi la zone dans le code de 
	l'instruction ou l'on veut écrire doit nécessairement être vide en risque de corrompre tout le code.
	la taille de value ne provoquera pas d'exception, la fonction tronquera le nombre automatiquement.
*/
void pasteValue(Instruction* instruction, int field,Byte* value,int dim);

/*
Description:
	transforme une chaine de caractères en lower case
parametre:
	text - chaine de caractères se finissant par \0
return:
	void
erreur:
	si la chaine n'est pas terminée par \0
*/
void toLowerCase(char *text);

/*
Description:
	place les arguments dans une matrice de caractères
paramètre:
	tab - tableau bidimensionnel de char
	instruction - pointeur vers char
return:
	void
*/
void param_to_tab(char tab[8][16],char *instruction);

/*
Description:
	transforme une chaine de caractère en binaire  -> "11" = 11
paramètre:
	val - chaine de caractères
return:
	Byte
*/
Byte registerToByte(char *val);


/*
Description:
  retourne la valeur du registre et met dans offset la valeur de l'offset.
  exemple si on a "200($1)" => offset=200 et return = 1
paramètre:
	str - chaine de caractères
	offset - chaine de caractère
return:
	Byte
*/
Byte indirectRegisterToByte(char *str, int *offset);

/*
Description:
  enlève les especes de début et de fin d'une chaine de caractère "    ADD $1, $2, $3   " -> "ADD $1, $2, $3"
paramètre:
	instr - chaine de caractères
return:
	void
*/
void del_espace(char *instr);

/*-------------------------------------*
 *				Fonctions			   *
 *-------------------------------------*/

void textInstructionToOpcode(char* textInstruction, Instruction *instruction){
	char opcode[8];
	getOperationCodeText(textInstruction,opcode);
	instruction->id = UNKNOWN_ID;
	if(opcode[1]=='\0' && opcode[0]=='j'){
		/* alors forcément instruction 'j' 
			j -> 6 premiers bits a 000010
			donc code[0] = 000010xx
		*/
		/*instruction->code[0] = (instruction->code[0]&0x03) + (J_CODE<<2);*/
		setNormalOpCode(instruction, J_CODE);
		instruction->id = J_ID;

		setBlocksSize(instruction,6,26,0,0,0,0,0,0);
	}else if(opcode[2]=='\0'){
		/*
			or
			lw
			jr
			sw
		*/

		if (!strcmp("or",opcode)){
			setSpecialOpCode(instruction, OR_CODE);
			instruction->id = OR_ID;
			setBlocksSize(instruction,6,5,5,5,5,6,0,0);
		}
		else if (!strcmp("lw",opcode)){
			setNormalOpCode(instruction, LW_CODE);
			instruction->id = LW_ID;
			setBlocksSize(instruction,6,5,5,16,0,0,0,0);
		}
		else if (!strcmp("jr",opcode)){
			setSpecialOpCode(instruction, JR_CODE);
			instruction->id = JR_ID;
			setBlocksSize(instruction,6,5,10,5,6,0,0,0);
		}
		else if (!strcmp("sw",opcode)){
			setNormalOpCode(instruction, SW_CODE);
			instruction->id = SW_ID;
			setBlocksSize(instruction,6,5,5,16,0,0,0,0);
		}
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

		if (!strcmp("add",opcode)){
			setSpecialOpCode(instruction, ADD_CODE);
			instruction->id = ADD_ID;
			setBlocksSize(instruction,6,5,5,5,5,6,0,0);
		}
		else if (!strcmp("and",opcode)){
			setSpecialOpCode(instruction, AND_CODE);
			instruction->id = AND_ID;
			setBlocksSize(instruction,6,5,5,5,5,6,0,0);
		}
		else if (!strcmp("beq",opcode)){
			setNormalOpCode(instruction, BEQ_CODE);
			instruction->id = BEQ_ID;
			setBlocksSize(instruction,6,5,5,16,0,0,0,0);
		}
		else if (!strcmp("bne",opcode)){
			setNormalOpCode(instruction, BNE_CODE);
			instruction->id = BNE_ID;
			setBlocksSize(instruction,6,5,5,16,0,0,0,0);
		}
		else if (!strcmp("div",opcode)){
			setSpecialOpCode(instruction, DIV_CODE);
			instruction->id = DIV_ID;
			setBlocksSize(instruction,6,5,5,10,6,0,0,0);
		}
		else if (!strcmp("jal",opcode)){
			setNormalOpCode(instruction, JAL_CODE);
			instruction->id = JAL_ID;
			setBlocksSize(instruction,6,26,0,0,0,0,0,0);
		}
		else if (!strcmp("lui",opcode)){
			setNormalOpCode(instruction, LUI_CODE);
			instruction->id = LUI_ID;
			setBlocksSize(instruction,6,5,5,16,0,0,0,0);
		}
		else if (!strcmp("nop",opcode)){
			setSpecialOpCode(instruction, NOP_CODE);
			instruction->id = NOP_ID;
			setBlocksSize(instruction,6,5,5,5,5,6,0,0);
		}
		else if (!strcmp("sll",opcode)){
			setSpecialOpCode(instruction, SLL_CODE);
			instruction->id = SLL_ID;
			setBlocksSize(instruction,6,5,5,5,5,6,0,0);
		}
		else if (!strcmp("slt",opcode)){
			setSpecialOpCode(instruction, SLT_CODE);
			instruction->id = SLT_ID;
			setBlocksSize(instruction,6,5,5,5,5,6,0,0);
		}
		else if (!strcmp("srl",opcode)){
			setSpecialOpCode(instruction, SRL_CODE);
			instruction->id = SRL_ID;
			setBlocksSize(instruction,6,4,1,5,5,5,6,0);
		}
		else if (!strcmp("sub",opcode)){
			setSpecialOpCode(instruction, SUB_CODE);
			instruction->id = SUB_ID;
			setBlocksSize(instruction,6,5,5,5,5,6,0,0);
		}
		else if (!strcmp("xor",opcode)){
			setSpecialOpCode(instruction, XOR_CODE);
			instruction->id = XOR_ID;
			setBlocksSize(instruction,6,5,5,5,5,6,0,0);
		}
	}else if(opcode[4]=='\0'){
		/*rotr
		addi
		bgtz
		blez
		mfhi
		mflo
		mult*/

		if (!strcmp("rotr",opcode)){
			setSpecialOpCode(instruction, ROTR_CODE);
			instruction->id = ROTR_ID;
			setBlocksSize(instruction,6,4,1,5,5,5,6,0);
		}
		else if (!strcmp("addi",opcode)){
			setNormalOpCode(instruction, ADDI_CODE);
			instruction->id = ADDI_ID;
			setBlocksSize(instruction,6,5,5,16,0,0,0,0);
		}
		else if (!strcmp("bgtz",opcode)){
			setNormalOpCode(instruction, BGTZ_CODE);
			instruction->id = BGTZ_ID;
			setBlocksSize(instruction,6,5,5,16,0,0,0,0);
		}
		else if (!strcmp("blez",opcode)){
			setNormalOpCode(instruction, BLEZ_CODE);
			instruction->id = BLEZ_ID;
			setBlocksSize(instruction,6,5,5,16,0,0,0,0);
		}
		else if (!strcmp("mfhi",opcode)){
			setSpecialOpCode(instruction, MFHI_CODE);
			instruction->id = MFHI_ID;
			setBlocksSize(instruction,6,10,5,5,6,0,0,0);
		}
		else if (!strcmp("mflo",opcode)){
			setSpecialOpCode(instruction, MFLO_CODE);
			instruction->id = MFLO_ID;
			setBlocksSize(instruction,6,10,5,5,6,0,0,0);
		}
		else if (!strcmp("mult",opcode)){
			setSpecialOpCode(instruction, MULT_CODE);
			instruction->id = MULT_ID;
			setBlocksSize(instruction,6,5,5,10,6,0,0,0);
		}
	}else{
		/* syscall */
		if (!strcmp("syscall",opcode)){
			setSpecialOpCode(instruction, SYSCALL_CODE);
			instruction->id = SYSCALL_ID;
			setBlocksSize(instruction,6,20,6,0,0,0,0,0);
		}
	}
}

void writeInstructionOperands(Instruction *inst, char *inst_str){
	/*pasteValue(Instruction* instruction, int field,Byte* value,int dim) */
	char operandes[8][16];
	Byte imm[2];
	Byte reg;
	int int_immediat;

	param_to_tab(operandes,inst_str);
	switch(inst->id){
		case ADD_ID:
			/*ADD rd(0), rs(1), rt(2)*/
			/*SPECIAL | rs(1) | rt(2) | rd(0) | 0 | ADD*/
			reg = registerToByte(operandes[1]);
			pasteValue(inst,/*flag*/1,&reg,1);
						
			reg = registerToByte(operandes[2]);
			pasteValue(inst,/*flag*/2,&reg,1);

			reg = registerToByte(operandes[0]);
			pasteValue(inst,/*flag*/3,&reg,1);
			break;
		case ADDI_ID:
			/*ADDI rt(0), rs(1), imm(2)*/
			/*ADDI | rs(1) | rt(0) | imm(2)*/

			reg = registerToByte(operandes[1]);
			pasteValue(inst,/*flag*/1,&reg,1);
						
			reg = registerToByte(operandes[0]);
			pasteValue(inst,/*flag*/2,&reg,1);

			/* write imm */
			IntTo2ByteArray(ImmediatStrToInteger(operandes[2]),imm);
			pasteValue(inst,/*flag*/3,imm,2);
			break;
		case AND_ID:
			/*AND rd(0), rs(1), rt(2)*/
			/*SPECIAL | rs(1) | rt(2) | rd(0) | 0 | AND*/

			reg = registerToByte(operandes[1]);
			pasteValue(inst,/*flag*/1,&reg,1);
						
			reg = registerToByte(operandes[2]);
			pasteValue(inst,/*flag*/2,&reg,1);

			reg = registerToByte(operandes[0]);
			pasteValue(inst,/*flag*/3,&reg,1);
			break;

		case BEQ_ID:
			/*BEQ rs(0), rt(1), offset(2)*/
			/*BEQ | rs(0) | rt(1) | offset(2)*/
			reg = registerToByte(operandes[0]);
			pasteValue(inst,/*flag*/1,&reg,1);
						
			reg = registerToByte(operandes[1]);
			pasteValue(inst,/*flag*/2,&reg,1);

			IntTo2ByteArray(ImmediatStrToInteger(operandes[2]),imm);
			pasteValue(inst,/*flag*/3,imm,2);
			break;

		case BGTZ_ID:
			/*BGTZ rs(0), offset(1)*/
			/*BGTZ | rs(0) | 0 | offset(1)*/
			reg = registerToByte(operandes[0]);
			pasteValue(inst,1,&reg,1);
			printf("immediat \"%s\" to %d\n", operandes[1],ImmediatStrToInteger(operandes[1]));
			IntTo2ByteArray(ImmediatStrToInteger(operandes[1]),imm);
			pasteValue(inst,/*flag*/3,imm,2);
			break;

		case BLEZ_ID:
			/*BGTZ rs(0), rt(1), offset(2)*/
			/*BGTZ | rs(0) | 0 | offset(2)*/
			reg = registerToByte(operandes[0]);
			pasteValue(inst,/*flag*/1,&reg,1);

			IntTo2ByteArray(ImmediatStrToInteger(operandes[1]),imm);
			pasteValue(inst,/*flag*/3,imm,2);
			break;

		case BNE_ID:
			/* BNE rs(0) ,rt(1) ,offset(2) */
			/*BNE | rs(0) | rt(1) | offset(2)*/
			reg = registerToByte(operandes[0]);
			pasteValue(inst,/*flag*/1,&reg,1);

			reg = registerToByte(operandes[1]);
			pasteValue(inst,/*flag*/2,&reg,1);

			IntTo2ByteArray(ImmediatStrToInteger(operandes[2]),imm);
			pasteValue(inst,/*flag*/3,imm,2);
			break;

		case DIV_ID:
			/*DIV rs(0),rt(1)*/
			/*SPECIAL | rs(0) | rt(1) | 0 | DIV*/
			reg = registerToByte(operandes[0]);
			pasteValue(inst,/*flag*/1,&reg,1);

			reg = registerToByte(operandes[1]);
			pasteValue(inst,/*flag*/2,&reg,1);
			break;

		case J_ID:
			printf("Instruction J a implementer\n");
			break;

		case JAL_ID:
			printf("Instruction JAL a implementer\n");
			break;

		case JR_ID:
			/* JR rs */
			/*SPECIAL | rs | 0 | hint | JR*/
			reg = registerToByte(operandes[0]);
			pasteValue(inst,/*flag*/1,&reg,1);
			break;

		case LUI_ID:
			/*LUI rt, imm*/
			/*LUI | 0 | rt(0) | imm(1)*/
			reg = registerToByte(operandes[0]);
			pasteValue(inst,/*flag*/2,&reg,1);

			IntTo2ByteArray(ImmediatStrToInteger(operandes[1]),imm);
			pasteValue(inst,/*flag*/3,imm,2);
			break;
		case LW_ID:
			/*LW rt(0), offset(base)(1)*/
			/*LW | base | rt | offset*/
			reg = registerToByte(operandes[0]);
			pasteValue(inst,/*flag*/2,&reg,1);

			reg = indirectRegisterToByte(operandes[1],&int_immediat);
			pasteValue(inst,/*flag*/1,&reg,1);

			IntTo2ByteArray(int_immediat,imm);
			pasteValue(inst,/*flag*/3,imm,2);
			break;

		case MFHI_ID:
			/*MFHI rd(0)*/
			/*SPECIAL | 0 | rd | 0 | MFHI*/
			reg = registerToByte(operandes[0]);
			pasteValue(inst,/*flag*/2,&reg,1);
			break;

		case MFLO_ID:
			/*MFLO rd(0)*/
			/*SPECIAL | 0 | rd(0) | 0 | MFLO*/
			reg = registerToByte(operandes[0]);
			pasteValue(inst,/*flag*/2,&reg,1);
			break;

		case MULT_ID:
			/*MULT rs(0),rt(1)*/
			/*SPECIAL | rs(0) | rt(1) | 0 | MULT*/
			reg = registerToByte(operandes[0]);
			pasteValue(inst,/*flag*/1,&reg,1);

			reg = registerToByte(operandes[1]);
			pasteValue(inst,/*flag*/2,&reg,1);			
			break;

		case NOP_ID:
			/*Do nothing*/
			break;

		case OR_ID:
			/*OR rd(0),rs(1),rt(2)*/
			/*SPECIAL | rs(1) | rt (2) | rd(0) | OR*/
			reg = registerToByte(operandes[1]);
			pasteValue(inst,/*flag*/1,&reg,1);
						
			reg = registerToByte(operandes[2]);
			pasteValue(inst,/*flag*/2,&reg,1);

			reg = registerToByte(operandes[0]);
			pasteValue(inst,/*flag*/3,&reg,1);
			break;

		case ROTR_ID:
			/*ROTR rd(0),rt(1),sa(2)*/
			/*SPECIAL | 0 | R | rt(1) | rd(0) | sa(2) | ROTR*/
			reg = 1;
			pasteValue(inst,/*flag*/2,&reg,1);
			
			reg = registerToByte(operandes[1]);
			pasteValue(inst,/*flag*/3,&reg,1);
						
			reg = registerToByte(operandes[0]);
			pasteValue(inst,/*flag*/4,&reg,1);

			reg = registerToByte(operandes[2]);
			pasteValue(inst,/*flag*/5,&reg,1);
			break;

		case SLL_ID:
			/*SLL rd(0),rt(1),sa(2)*/
			/*SPECIAL | 0 | rt(1) | rd(0) | sa(2) | SLL*/
			reg = registerToByte(operandes[1]);
			pasteValue(inst,/*flag*/2,&reg,1);
						
			reg = registerToByte(operandes[0]);
			pasteValue(inst,/*flag*/3,&reg,1);

			reg = registerToByte(operandes[2]);
			pasteValue(inst,/*flag*/4,&reg,1);
			break;

		case SLT_ID:
			/*SLT rd(0),rs(1),rt(2)*/
			/*SPECIAL | rs(1) | rt(2) | rd(0) | 0 | SLT*/
			reg = registerToByte(operandes[1]);
			pasteValue(inst,/*flag*/1,&reg,1);
						
			reg = registerToByte(operandes[2]);
			pasteValue(inst,/*flag*/2,&reg,1);

			reg = registerToByte(operandes[0]);
			pasteValue(inst,/*flag*/3,&reg,1);
			break;

		case SRL_ID:
			/*SRL rd(0),rt(1),sa(2)*/
			/*SPECIAL | 0 | R | rt(1) | rd(0) | sa(2) | SRL*/
			reg = registerToByte(operandes[1]);
			pasteValue(inst,/*flag*/3,&reg,1);
						
			reg = registerToByte(operandes[0]);
			pasteValue(inst,/*flag*/4,&reg,1);

			reg = registerToByte(operandes[2]);
			pasteValue(inst,/*flag*/5,&reg,1);
			break;

		case SUB_ID:
			/*SUB rd(0),rs(1),rt(2)*/
			/*SPECIAL | rs(1) | rt(2) | rd(0) | 0 | SUB*/
			reg = registerToByte(operandes[1]);
			pasteValue(inst,/*flag*/1,&reg,1);
						
			reg = registerToByte(operandes[2]);
			pasteValue(inst,/*flag*/2,&reg,1);

			reg = registerToByte(operandes[0]);
			pasteValue(inst,/*flag*/3,&reg,1);
			break;

		case SW_ID:
			/*SW rt(0), offset(base)(1)*/
			/*SW | base | rt | offset*/
			reg = registerToByte(operandes[0]);
			pasteValue(inst,/*flag*/2,&reg,1);

			reg = indirectRegisterToByte(operandes[1],&int_immediat);
			pasteValue(inst,/*flag*/1,&reg,1);

			IntTo2ByteArray(int_immediat,imm);
			pasteValue(inst,/*flag*/3,imm,2);
			break;

		case SYSCALL_ID:
			/*code sur 20 bit pas de valeur donnée*/
			break;

		case XOR_ID:
			/*XOR rd(0),rs(1),rt(2)*/
			/*SPECIAL | rs(1) | rt(2) | rd(0) | 0 | XOR*/
			reg = registerToByte(operandes[1]);
			pasteValue(inst,/*flag*/1,&reg,1);
						
			reg = registerToByte(operandes[2]);
			pasteValue(inst,/*flag*/2,&reg,1);

			reg = registerToByte(operandes[0]);
			pasteValue(inst,/*flag*/3,&reg,1);
			break;

	}
}

void setNormalOpCode(Instruction *instruction, Byte opCode){
	instruction->code[0] = (instruction->code[0]&0x03) + (opCode<<2);
}

void setSpecialOpCode(Instruction *instruction, Byte opCode){
	instruction->code[3] = (instruction->code[0]&0xC0) + opCode;
	setNormalOpCode(instruction,0);
}

void getOperationCodeText(char* textInstruction, char* res){
	int i=0;
	while(textInstruction[i]!=' '){
		res[i]=textInstruction[i];
		i++;
	}
	res[i]='\0';
}

void toLowerCase(char *text){
	char *cp = text;
	while((*cp)!='\0'){
		if((*cp)>= 'A' && (*cp)<='Z'){
			(*cp)=(*cp)+32;
		}
		cp++;
	}
}

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

	/* init le masque à 0xFFFFFFFF et val à 0x00000000 */
	for(i=0;i<4;i++){
		mask1[i] = 0xFF;
		mask2[i] = 0xFF;
		val[i]=0;
	}

	/* met value dans val (taille value =< 4) */
	for(i=0;i<dim;i++){
		/* on doit écrire à la fin car les shift sont appliqués dans le sens 
		inverse aux indices des tableaux */
		val[i+(4-dim)]=value[i];
	}

	/* positionne la valeur à la fin tel que le bit de poids fort soit le premier dans la chaine */
	shiftLNBit(val, 32-size, 4);
	/* décale val pour être à la bonne position dans le code */
	shiftRNBit(val, pos,4);

	/* calcul les mask pour l'insertion dans le code */
	shiftRNBit(mask1, pos,4);
	shiftLNBit(mask2, 32-(pos+size), 4);

	/* insertion dans le code */
	for(i=0;i<4;i++){
		instruction->code[i] += mask1[i] &mask2[i]&val[i];
	}
}

void param_to_tab(char tab[8][16],char *instruction){
	int index = 0;
	int i =0;
	int j=0;
	int taille = strlen(instruction);
	while(instruction[index] != ' ')
		index++;
	index++;
	/*tant qu'on est pas à la fin de l'instruction alors il reste des paramètres*/
	while(index<taille){  
		j=0;
		/*on récupère le paramètre jusqu'au prochain ','*/
		while(index < taille && instruction[index] != ','){   
			tab[i][j] = instruction[index];
			j++;
			index++;
		}
		tab[i][j] ='\0';
		/*on saute la virgule*/
		if (instruction[index]==',')
			index++;
		i++;
	}
	/*on définit le reste des lignes avec un simple '\0' */
	while(i<8){
		tab[i][0]='\0';
		i++;
	}
}

void del_espace(char *instr){
	int index =0;
	int taille = strlen(instr);
	int i =0;

	while(instr[index] == ' ')
		index++;

	while(instr[taille-1] ==' ')
		taille--;

	while(index<taille){
		instr[i] = instr[index];
		i++;
		index++;
	}
	instr[i] = '\0';

}

void format_instr(char *instr){
	char *cursor=instr;
	int i =0;
	del_espace(instr);
	while((*cursor)!=' ' && (*cursor)!='\0'){
		instr[i]=(*cursor);
		cursor++;
		i++;
	}
	instr[i] = ' ';
	i++;
	cursor++; /*on skip l'opération*/
	while((*cursor) != '\0'){
		if ((*cursor)!= ' '){
			instr[i] = (*cursor);
			i++;
		}
		cursor++;
	}
	instr[i] = '\0';
	toLowerCase(instr);
}

Byte registerToByte(char *val){
	char *cp = val;
    Byte resultat = 0;
    if ((*cp) == '$')
    	cp++;
    /*tant qu'on a pas finit de lire le nombre*/
    while((*cp)!='\0' && (*cp) != ')'){
    	/*on ajoute au résultat la valeur décimale correspondante au caractère fois la bonne dizaine*/
        resultat=resultat * 10 + (*cp)-'0';
        cp++;
    }
    return resultat;
}

Byte indirectRegisterToByte(char *str, int *offset){
	/*on recupère l'offset*/
	int taille =strlen(str);
	int pos =0;


    int dizaine = 1;
    int index =0;
    char *ptr;
    *offset = 0;

    while(pos<taille && str[pos] != '(')
    	pos++;

    /*tant qu'on a pas finit de lire l'offset*/
    while(index<pos){
  
        *offset+=dizaine*(str[pos -1 -index]-48);

        dizaine *=10;
        index++;
    }
    index++;

    /*on se place au numéro de registre*/
    ptr = str + index;

    return (registerToByte(ptr));
}

void initInst(Instruction *inst){
	inst->code[0]=0;
	inst->code[1]=0;
	inst->code[2]=0;
	inst->code[3]=0; 

	inst->b[0]=0;
	inst->b[1]=0;
	inst->b[2]=0;
	inst->b[3]=0; 
	inst->b[4]=0;
	inst->b[5]=0;
	inst->b[6]=0;
	inst->b[7]=0;

	inst->id = 0xFF;
}