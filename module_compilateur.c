#include "module_compilateur.h"

/******************************************
 *      Section traitement de texte       *
 ******************************************/

/*
prend en parametre une liste de caractère et coupe au premier #
Req: non nul
*/
void removecomments(char *line){
	char *cp=line;
	while((*cp) != '#' && *cp != '\0'){
		cp++;
	}
	*cp = '\0';
}

/*
enlève les espaces de debut et de fin
ex: "    ADD $1, $2, $3   " -> "ADD $1, $2, $3"
Req: non nul
*/
void del_espaces_bords(char *line){
	int index = 0;
	int taille = strlen(line);
	int i =0;

	/*on supprime les espaces du debut*/
	while(line[index] == ' ')
		index++;

	/*on supprime les espaces de la fin*/
	while(line[taille-1] ==' ')
		taille--;

	/*redeplace la chaine de caractère au debut*/
	while(index<taille){
		line[i] = line[index];
		i++;
		index++;
	}
	/*coupe les espaces de la fin*/
	line[i] = '\0';
}

/*
enlève les espaces au debut, a la fin et au milieu d'une chaine : "    ADD $1, $2, $3   " -> "ADD $1,$2,$3"
Req : non nul
*/
void del_espaces(char *line){
	char *cursor=line;
	int i =0;

	/*supprimes espaces aux extremitees*/
	del_espaces_bords(line);
	/*avance jusqu'a trouver un premier espace*/
	while((*cursor)!=' ' && (*cursor)!='\0'){
		line[i]=(*cursor);
		cursor++;
		i++;
	}

	/*
	CHANGE: 
		' '=>(*cursor) 
		dans le cas ou la chaine de caractères ne comporte pas d'espaces causais des problemes*/
	line[i] = (*cursor);

	i++;
	/*
	CHANGE:
		cursor++;=>X
		dans le cas ou *cursor pointais vers \0 faire ++ skip le char de fin, boucle infini ou presque
		non necessaire car deja gerer par le while juste en dessous si *cursor==' ' 
	*/

	/*copie de la chaine restante en supprimant les espaces*/
	while((*cursor) != '\0'){
		if ((*cursor)!= ' '){
			line[i] = (*cursor);
			i++;
		}
		cursor++;
	}

	/*fin de la chaine*/
	line[i] = '\0';
}

/*
transforme une chaine de caractères en lower case 
Require: non nul et \0
*/
void tolowercase(char *line){
	char *cp = line;
	while((*cp)!='\0'){
		if((*cp)>= 'A' && (*cp)<='Z'){
			(*cp)=(*cp)+32;
		}
		cp++;
	}
}

/*
Description:
	prend une chaine de caractère en paramètre et la prepare pour compilation
Req: chaine de caractère valide (fin avec \0), peut être nul
*/
void tokenize(char *line){
	if(line != NULL){
		removecomments(line);
		del_espaces(line);
		tolowercase(line);
	}
}

/***********************************************************************
 *                         Section Compilation                         *
 ***********************************************************************/
/*
CHANGE:
	changement d'implementation, prend avantage des trois types d'instruction defini dans la doc.
	R-instruction $rd,$rs,$rt
	I-instruction $rt,$rs,immediate
	J-instruction target
	+ prise en charge des erreurs
*/

/*remplace le char toreplace avec with dans line
Req : line non nul*/
void replace(char *line,char toreplace, char with){
	while(*line != '\0'){
		if(*line == toreplace){
			*line = with;
		}
		line++;
	}
}

/*decoupe en bloc la ligne pour la compilation si trop de paramètre ou espaces mal places va retourner un echec
Req   : line non null
return: si la ligne a ete consumee*/
int extraction(char tab[4][16],char *line){
	int continue_loop=1;
	int index = 0;
	int i = 0;
	int j = 0;
	char cp_line[LINE_MAX_WIDTH];

	do{
		cp_line[index]=line[index];
	}while(line[index++] != '\0');
	index=0;

	/*remplace virgules avec espaces
	ex: add 1,2,3 => add 1 2 3*/
	replace(cp_line,',',' ');
	
	continue_loop = cp_line[index]!='\0';
	while(continue_loop){
		if(cp_line[index]==' '){
			tab[i][j]='\0';
			i++;
			j=0;
		}else{
			tab[i][j]=cp_line[index];
			j++;
		}

		index++;
		/*tant que reste des elements dans line et reste de la place dans tab
		cas particulier si un des arguments depasse 15 en taille alors s'arrete*/
		continue_loop = cp_line[index]!='\0' && i<4 && j<15;
	}
	tab[i][j]='\0';
	i++;
	while(i<4){
		tab[i][0]='\0'; /*remplis les arguments vides avec des string vides*/
		i++;
	}

	return cp_line[index]=='\0';
}

/*applique les mask pour les opcodes*/
void setNormalOpCode(Instruction *instruction, Byte opCode){
	instruction->code[0] = (instruction->code[0]&0x03) + (opCode<<2);
}

/*applique les mask pour les opcodes*/
void setSpecialOpCode(Instruction *instruction, Byte opCode){
	instruction->code[3] = (instruction->code[0]&0xC0) + opCode;
	setNormalOpCode(instruction,0);
}

/*initialise les 6 fields bloc de l'instruction*/
void setBlocksSize(Instruction* instruction, Byte b0,Byte b1,Byte b2, Byte b3,Byte b4,Byte b5){
	instruction->b[0]=b0;
	instruction->b[1]=b1;
	instruction->b[2]=b2;
	instruction->b[3]=b3;
	instruction->b[4]=b4;
	instruction->b[5]=b5;
}

/*detecte et ecrit le code operation de la chaine de caractère dans l'instruction*/
int mapOpCode(char* opcode, Instruction *instruction){
	instruction->id = UNKNOWN_ID;
	if(opcode[1]=='\0' && opcode[0]=='j'){
		/* alors forcement instruction 'j' 
			j -> 6 premiers bits a 000010
			donc code[0] = 000010xx
		*/
		setNormalOpCode(instruction, J_CODE);
		instruction->id = J_ID;
	}else if(opcode[2]=='\0'){
		/*or lw jr sw */

		if (!strcmp("or",opcode)){
			setSpecialOpCode(instruction, OR_CODE);
			instruction->id = OR_ID;
		}
		else if (!strcmp("lw",opcode)){
			setNormalOpCode(instruction, LW_CODE);
			instruction->id = LW_ID;
		}
		else if (!strcmp("jr",opcode)){
			setSpecialOpCode(instruction, JR_CODE);
			instruction->id = JR_ID;
		}
		else if (!strcmp("sw",opcode)){
			setNormalOpCode(instruction, SW_CODE);
			instruction->id = SW_ID;
		}
	}else if(opcode[3]=='\0'){
		/*add and beq bne div jal lui nop sll slt srl sub xor*/

		if (!strcmp("add",opcode)){
			setSpecialOpCode(instruction, ADD_CODE);
			instruction->id = ADD_ID;
		}
		else if (!strcmp("and",opcode)){
			setSpecialOpCode(instruction, AND_CODE);
			instruction->id = AND_ID;
		}
		else if (!strcmp("beq",opcode)){
			setNormalOpCode(instruction, BEQ_CODE);
			instruction->id = BEQ_ID;
		}
		else if (!strcmp("bne",opcode)){
			setNormalOpCode(instruction, BNE_CODE);
			instruction->id = BNE_ID;
		}
		else if (!strcmp("div",opcode)){
			setSpecialOpCode(instruction, DIV_CODE);
			instruction->id = DIV_ID;
		}
		else if (!strcmp("jal",opcode)){
			setNormalOpCode(instruction, JAL_CODE);
			instruction->id = JAL_ID;
		}
		else if (!strcmp("lui",opcode)){
			setNormalOpCode(instruction, LUI_CODE);
			instruction->id = LUI_ID;
		}
		else if (!strcmp("nop",opcode)){
			setSpecialOpCode(instruction, NOP_CODE);
			instruction->id = NOP_ID;
		}
		else if (!strcmp("sll",opcode)){
			setSpecialOpCode(instruction, SLL_CODE);
			instruction->id = SLL_ID;
		}
		else if (!strcmp("slt",opcode)){
			setSpecialOpCode(instruction, SLT_CODE);
			instruction->id = SLT_ID;
		}
		else if (!strcmp("srl",opcode)){
			setSpecialOpCode(instruction, SRL_CODE);
			instruction->id = SRL_ID;
		}
		else if (!strcmp("sub",opcode)){
			setSpecialOpCode(instruction, SUB_CODE);
			instruction->id = SUB_ID;
		}
		else if (!strcmp("xor",opcode)){
			setSpecialOpCode(instruction, XOR_CODE);
			instruction->id = XOR_ID;
		}
	}else if(opcode[4]=='\0'){
		/*rotr addi bgtz blez mfhi mflo mult*/

		if (!strcmp("rotr",opcode)){
			setSpecialOpCode(instruction, ROTR_CODE);
			instruction->id = ROTR_ID;
		}
		else if (!strcmp("addi",opcode)){
			setNormalOpCode(instruction, ADDI_CODE);
			instruction->id = ADDI_ID;
		}
		else if (!strcmp("bgtz",opcode)){
			setNormalOpCode(instruction, BGTZ_CODE);
			instruction->id = BGTZ_ID;
		}
		else if (!strcmp("blez",opcode)){
			setNormalOpCode(instruction, BLEZ_CODE);
			instruction->id = BLEZ_ID;
		}
		else if (!strcmp("mfhi",opcode)){
			setSpecialOpCode(instruction, MFHI_CODE);
			instruction->id = MFHI_ID;
		}
		else if (!strcmp("mflo",opcode)){
			setSpecialOpCode(instruction, MFLO_CODE);
			instruction->id = MFLO_ID;
		}
		else if (!strcmp("mult",opcode)){
			setSpecialOpCode(instruction, MULT_CODE);
			instruction->id = MULT_ID;
		}
	}else{
		/* syscall */
		if (!strcmp("syscall",opcode)){
			setSpecialOpCode(instruction, SYSCALL_CODE);
			instruction->id = SYSCALL_ID;
		}
	}

	switch((instruction->id)&OPCODE_TYPE_MASK){
		case OPCODE_TYPE_R:
			setBlocksSize(instruction,6,5,5,5,5,6);
			break;
		case OPCODE_TYPE_I:
			setBlocksSize(instruction,6,5,5,16,0,0);
			break;
		case OPCODE_TYPE_J:
			setBlocksSize(instruction,6,26,0,0,0,0);
			break;
		default:
			setBlocksSize(instruction,0,0,0,0,0,0);
			break;
	}

	return instruction->id != UNKNOWN_ID;
}

/*ecrit une valeur de taille N sur une chaine de Byte
convention des octets*/
void pasteValue(Instruction* instruction, int field,Byte* value,int dim){
	/* size in bit of the value not necessarly a multiple of 8 */
	int size=instruction->b[field];
	/* position wanted on the instruction code */
	int pos = 0,i=0;

	/* temp values */
	DWord val;
	DWord mask1;
	DWord mask2;

	/* calculate position in bit in the 32bit code */
	for(i=0;i<field;i++){
		pos += instruction->b[i];
	}

	/* init le masque a 0xFFFFFFFF et val a 0x00000000 */
	for(i=0;i<4;i++){
		mask1[i] = 0xFF;
		mask2[i] = 0xFF;
		val[i]=0;
	}

	/* met value dans val (taille value =< 4) */
	for(i=0;i<dim;i++){
		/* on doit ecrire a la fin car les shift sont appliques dans le sens 
		inverse aux indices des tableaux */
		val[i+(4-dim)]=value[i];
	}

	/* positionne la valeur a la fin tel que le bit de poids fort soit le premier dans la chaine */
	shiftLDWord(val, 32-size);
	/* decale val pour être a la bonne position dans le code */
	shiftRDWord(val, pos);

	/* calcul les mask pour l'insertion dans le code */
	shiftRDWord(mask1, pos);
	shiftLDWord(mask2, 32-(pos+size));

	/* insertion dans le code */
	for(i=0;i<4;i++){
		instruction->code[i] += mask1[i] &mask2[i]&val[i];
	}
}

/*ecrit les bloc necessaire pour une instruction type R*/
void mapTypeR(Byte rs,Byte rt,Byte rd,Byte sa,Instruction *output){
	pasteValue(output,1,&rs,1);
	pasteValue(output,2,&rt,1);
	pasteValue(output,3,&rd,1);
	pasteValue(output,4,&sa,1);
}

/*ecrit les bloc necessaire pour une instruction type I*/
void mapTypeI(Byte rs,Byte rt,DWord immediat,Instruction *output){
	pasteValue(output,1,&rs,1);
	pasteValue(output,2,&rt,1);
	pasteValue(output,3,immediat,4);
}

/*ecrit les bloc necessaire pour une instruction type J*/
void mapTypeJ(DWord target,Instruction *output){
	pasteValue(output,1,target,4);
}

/*converti une chaine de caractère de type $X en une valeur sur un Byte
return: succes ?*/
int StringToRegistre(char *registre,Byte *converted){
	int success=1;
	const char REGISTERS[32][6]={
		"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3",
		"$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7",
		"$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7",
		"$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra",
	};
	int i=0;
	if(*registre=='\0'){
		success=0;
	}else if(*registre=='$'){
		success=StringToByte(registre+1,converted);
		if(success==0){
			success=0;
			/*gros if else if strcmp registres en lettre*/
			while(i<32 && success==0){
				if(strcmp(REGISTERS[i],registre)==0){
					success=1;
					*converted=i;
				}
				i++;
			}
		}else{
			/*valeur du registre trop grande*/
			if(*converted>=32){
				success=0;
			}
		}
	}else{
		success=0;
	}

	return success;
}

/*converti une chaine de caractère representant un entier signe en hexa ou decimale sur 2 Byte
return: succes ?*/
int StringToSignedIntByteArray(char *entier,DWord converted){
	int success=1;
	int value;

	if(entier[0]=='\0'){
		success=0;
	}else if(entier[0]=='0' && entier[1]=='x'){
		success=HexStringToInteger(&entier[2],&value);
	}else{
		success=StringToSignedInteger(entier,&value);
	}
	if(success){
		IntegerToDWord(value,converted);
	}
	return success;
}

/*converti une chaine de caractère offset(registre) en Byte
return: succes ?*/
int StringToRegistreWithOffset(char *line,Byte *registre,DWord offset){
	int success=1;
	int debut,fin;
	if(*line=='\0'){
		success=0;
	}else{
		debut=0;
		fin=0;
		/*cherche ouverture parenthese*/
		while(line[debut]!='(' && line[fin]!='\0'){
			debut++;
		}
		/*si pas ouverture echec*/
		if(line[debut]=='\0'){
			success=0;
		}else{
			/*cherche fermeture parenthese*/
			while(line[fin]!=')' && line[fin]!='\0'){
				fin++;
			}
			if(line[fin]=='\0'){
				/*pas de closure de la parenthese*/
				success=0;
			}else{
				/*recupere la valeur registre dans la zone definie par ($..)*/
				line[fin]='\0';

				/*remplace ( par \0 car offset($..) -> offset\0$..\0*/

				line[debut]='\0';
				if(debut==0){
				  debut++;
				  success = StringToByte(&line[debut+1],registre);
				  offset[0] = offset[1] = offset[2] = offset[3] = 0;

				}else{
				  debut++;
				  success = StringToByte(&line[debut+1],registre) && 
				            StringToSignedIntByteArray(line,offset);
				}
			}
		}
	}

	return success;
}

/*
attention pour les valeurs de rt rs et rd on attend des registres
donc sois $X soit un nom defini dans register_defines. Pour la valeur de 
sa par contre c'est une entière cad sans le $
return: succes ?
*/
int mapOperandes(char *operande1,char *operande2,char *operande3,Instruction *output){
	int success=1;
	Byte rs,rt,rd,sa;
	DWord immediat; /*16bits integer*/

	switch(output->id){
		case ADD_ID:
			/*ADD rd(0), rs(1), rt(2)*/
			/*SPECIAL | rs(1) | rt(2) | rd(0) | 0 | ADD*/
			if( StringToRegistre(operande1,&rd) &&
			    StringToRegistre(operande2,&rs) && 
			    StringToRegistre(operande3,&rt)){
				mapTypeR(rs,rt,rd,0,output);
			}else{
				success=0;
			}
			break;
		case ADDI_ID:
			/*ADDI rt(0), rs(1), imm(2)*/
			/*ADDI | rs(1) | rt(0) | imm(2)*/
			if( StringToRegistre(operande1,&rt) && 
			    StringToRegistre(operande2,&rs) &&
			    StringToSignedIntByteArray(operande3,immediat)){
				mapTypeI(rs,rt,immediat,output);
			}else{
				success=0;
			}
			break;
		case AND_ID:
			/*AND rd(0), rs(1), rt(2)*/
			/*SPECIAL | rs(1) | rt(2) | rd(0) | 0 | AND*/
			if( StringToRegistre(operande1,&rd) &&
			    StringToRegistre(operande2,&rs) && 
			    StringToRegistre(operande3,&rt)){
				mapTypeR(rs,rt,rd,0,output);
			}else{
				success=0;
			}
			break;

		case BEQ_ID:
			/*BEQ rs(0), rt(1), offset(2)*/
			/*BEQ | rs(0) | rt(1) | offset(2)*/
			if( StringToRegistre(operande1,&rs) && 
			    StringToRegistre(operande2,&rt) &&
			    StringToSignedIntByteArray(operande3,immediat)){
				mapTypeI(rs,rt,immediat,output);
			}else{
				success=0;
			}
			break;

		case BGTZ_ID:
			/*BGTZ rs(0), offset(1)*/
			/*BGTZ | rs(0) | 0 | offset(1)*/
			if( StringToRegistre(operande1,&rs) && 
			    StringToSignedIntByteArray(operande2,immediat)){
				mapTypeI(rs,0,immediat,output);
			}else{
				success=0;
			}
			break;

		case BLEZ_ID:
			/*BGTZ rs(0), rt(1), offset(2)*/
			/*BGTZ | rs(0) | 0 | offset(2)*/
			if( StringToRegistre(operande1,&rs) && 
			    StringToSignedIntByteArray(operande2,immediat)){
				mapTypeI(rs,0,immediat,output);
			}else{
				success=0;
			}
			break;

		case BNE_ID:
			/* BNE rs(0) ,rt(1) ,offset(2) */
			/*BNE | rs(0) | rt(1) | offset(2)*/
			if( StringToRegistre(operande1,&rs) && 
			    StringToRegistre(operande2,&rt) &&
			    StringToSignedIntByteArray(operande3,immediat)){
				mapTypeI(rs,rt,immediat,output);
			}else{
				success=0;
			}
			break;

		case DIV_ID:
			/*DIV rs(0),rt(1)*/
			/*SPECIAL | rs(0) | rt(1) | 0 | DIV*/
			if( StringToRegistre(operande1,&rs) &&
			    StringToRegistre(operande2,&rt)){
				mapTypeR(rs,rt,0,0,output);
			}else{
				success=0;
			}
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
			if( StringToRegistre(operande1,&rs)){
				mapTypeR(rs,0,0,0,output);
			}else{
				success=0;
			}
			break;

		case LUI_ID:
			/*LUI rt, imm*/
			/*LUI | 0 | rt(0) | imm(1)*/
			if( StringToRegistre(operande1,&rt) &&
			    StringToSignedIntByteArray(operande2,immediat)){
				mapTypeI(0,rt,immediat,output);
			}else{
				success=0;
			}
			break;

		case LW_ID:
			/*LW rt(0), offset(base)(1)*/
			/*LW | base | rt | offset*/
			if( StringToRegistre(operande1,&rt) &&
				StringToRegistreWithOffset(operande2,/*base*/&rs,immediat)){
				mapTypeI(rs,rt,immediat,output);
			}else{
				success=0;
			}
			break;

		case MFHI_ID:
			/*MFHI rd(0)*/
			/*SPECIAL | 0 | rd | 0 | MFHI*/
			if(StringToRegistre(operande1,&rd)){
				mapTypeR(0,0,rd,0,output);
			}else{
				success=0;
			}
			break;

		case MFLO_ID:
			/*MFLO rd(0)*/
			/*SPECIAL | 0 | rd(0) | 0 | MFLO*/
			if(StringToRegistre(operande1,&rd)){
				mapTypeR(0,0,rd,0,output);
			}else{
				success=0;
			}
			break;

		case MULT_ID:
			/*MULT rs(0),rt(1)*/
			/*SPECIAL | rs(0) | rt(1) | 0 | MULT*/
			if( StringToRegistre(operande1,&rs) &&
			    StringToRegistre(operande2,&rt)){
				mapTypeR(rs,rt,0,0,output);
			}else{
				success=0;
			}		
			break;

		case NOP_ID:
			/*Do nothing*/
			break;

		case OR_ID:
			/*OR rd(0),rs(1),rt(2)*/
			/*SPECIAL | rs(1) | rt (2) | rd(0) | OR*/
			if( StringToRegistre(operande1,&rd) &&
			    StringToRegistre(operande2,&rs) && 
			    StringToRegistre(operande3,&rt)){
				mapTypeR(rs,rt,rd,0,output);
			}else{
				success=0;
			}
			break;

		case ROTR_ID:
			/*ROTR rd(0),rt(1),sa(2)*/
			/*SPECIAL | 0 | R | rt(1) | rd(0) | sa(2) | ROTR*/
			if( StringToRegistre(operande1,&rd) &&
			    StringToRegistre(operande2,&rt) && 
			    StringToByte(operande3,&sa)){
				mapTypeR(1,rt,rd,sa,output);
			}else{
				success=0;
			}
			break;

		case SLL_ID:
			/*SLL rd(0),rt(1),sa(2)*/
			/*SPECIAL | 0 | rt(1) | rd(0) | sa(2) | SLL*/
			if( StringToRegistre(operande1,&rd) &&
			    StringToRegistre(operande2,&rt) && 
			    StringToByte(operande3,&sa)){
				mapTypeR(0,rt,rd,sa,output);
			}else{
				success=0;
			}
			break;

		case SLT_ID:
			/*SLT rd(0),rs(1),rt(2)*/
			/*SPECIAL | rs(1) | rt(2) | rd(0) | 0 | SLT*/
			if( StringToRegistre(operande1,&rd) &&
			    StringToRegistre(operande2,&rs) && 
			    StringToRegistre(operande3,&rt)){
				mapTypeR(rs,rt,rd,0,output);
			}else{
				success=0;
			}
			break;

		case SRL_ID:
			/*SRL rd(0),rt(1),sa(2)*/
			/*SPECIAL | 0 | R | rt(1) | rd(0) | sa(2) | SRL*/
			if( StringToRegistre(operande1,&rd) &&
			    StringToRegistre(operande2,&rt) && 
			    StringToByte(operande3,&sa)){
				mapTypeR(0,rt,rd,sa,output);
			}else{
				success=0;
			}
			break;

		case SUB_ID:
			/*SUB rd(0),rs(1),rt(2)*/
			/*SPECIAL | rs(1) | rt(2) | rd(0) | 0 | SUB*/
			if( StringToRegistre(operande1,&rd) &&
			    StringToRegistre(operande2,&rs) && 
			    StringToRegistre(operande3,&rt)){
				mapTypeR(rs,rt,rd,0,output);
			}else{
				success=0;
			}
			break;

		case SW_ID:
			/*SW rt(0), offset(base)(1)*/
			/*SW | base | rt | offset*/
			if( StringToRegistre(operande1,&rt) &&
				StringToRegistreWithOffset(operande2,/*base*/&rs,immediat)){
				mapTypeI(rs,rt,immediat,output);
			}else{
				success=0;
			}
			break;

		case SYSCALL_ID:
			/*code sur 20 bit pas de valeur donnee*/
			break;

		case XOR_ID:
			/*XOR rd(0),rs(1),rt(2)*/
			/*SPECIAL | rs(1) | rt(2) | rd(0) | 0 | XOR*/
			if( StringToRegistre(operande1,&rd) &&
			    StringToRegistre(operande2,&rs) && 
			    StringToRegistre(operande3,&rt)){
				mapTypeR(rs,rt,rd,0,output);
			}else{
				success=0;
			}
			break;
		default:
			success=0;
			break;

	}
	return success;
}



/*desc dans .h*/
int compileline(char *line,Instruction *output){
	char operandes[4][16];
	int state=0;
	if(line != NULL && *line!='\0'){
		/*printf("tokenizing '%s'\n", line);*/
		tokenize(line);
		/*if(DEBUG_COMPILER) printf("=> %s\n", line);*/
		/*printf("extracting %s\n", line);*/
		state = extraction(operandes,line);
		/*printf("extraction: %d\n", state);*/
		/*printf("=> [%s,%s,%s,%s]\n", operandes[0],operandes[1],operandes[2],operandes[3]);*/
		/*if line entirely consumed*/
		if(state){
			state = mapOpCode(operandes[0], output);

			/*if operation known*/
			if(state){
				/*printf("compiling [%s,%s,%s,%s]\n", operandes[0],operandes[1],operandes[2],operandes[3]);*/
				state = mapOperandes(operandes[1],operandes[2],operandes[3],output);
				/*printf("=> %02X%02X %02X%02X\n", output->code[0], output->code[1], output->code[2], output->code[3]);*/
				strcpy(output->text_instr,line);
			}
		}
	}
	return state;
}

int compile(const char *source,Program prog){
	FILE *fichier_source=NULL;
	char line[LINE_MAX_WIDTH];
	int state=1,i=0;
	fichier_source = fopen(source,"rb");
	if(fichier_source!=NULL){
		while(!feof(fichier_source) && state){
			/*met a zero la case*/
			initInst(prog+i);
			readInstruction(fichier_source,line);
			if(*line!='\0'){
				/*ecrit les resultats de compilations*/
				printf("%s",line);
				state=compileline(line, prog+i);
				if(state){
					/*passe a la case suivante*/

					i++;
					if(i>=PROGRAM_MAX_SIZE-1){
						state=0;
						printf("memoire programme insuffisante\n");
					}
				}else{
					printf("echec de compilation\n");
				}
			}
		}
		/*indique fin du programme*/
		initInst(prog+i);
		prog[i].id = HALT_ID;
	}else{
		printf("impossible d'ouvrir ou de creer les fichiers\n");
	}

	if(fichier_source!=NULL){
		fclose(fichier_source);
	}

	return state;
}

/*a implementer demander par le sujet*/
int compileFile(const char *source, const char *output){
	FILE *fichier_source=NULL;
	FILE *fichier_destination=NULL;
	Instruction instr;
	char line[LINE_MAX_WIDTH];
	int state=1;
	fichier_source = fopen(source,"rb");
	fichier_destination = fopen(output,"wb");
	if(fichier_source!=NULL && fichier_destination!= NULL){
		while(!feof(fichier_source) && state){
			initInst(&instr);
			readInstruction(fichier_source,line);
			if(*line!='\0'){
				state=compileline(line, &instr);
				if(state){
					writeHexInstructionToFile(fichier_destination,instr);
				}else{
					printf("echec de compilation\n");
				}
			}
		}
	}else{
		printf("impossible d'ouvrir ou de creer les fichiers\n");
	}

	if(fichier_source!=NULL){
		fclose(fichier_source);
	}

	if(fichier_destination!=NULL){
		fclose(fichier_destination);
	}

	return state;
}

/*desc dans .h*/
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

	inst->id = UNKNOWN_ID;
}