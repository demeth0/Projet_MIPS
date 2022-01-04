#include "module_emulateur.h"

/*permet de recopier dans sortie le contenue de l'entrée*/
void copy_Byte4(DWord entree,DWord sortie){
	sortie[0] = entree[0];
	sortie[1] = entree[1];
	sortie[2] = entree[2];
	sortie[3] = entree[3];
}

/*initialise simulation*/
void initSimulation(Environment *simulation){
	int index =0;
	Byte *CP = simulation->RAM;

	/*comme tout est a la suite dans une structure, cela permet de tout
	  initialiser à 0 (RAM avec MEM_SIZE puis registres avec 32*4 puis le PC,HI et LOW avec +12...)*/
	for(index =0;index<(32*4+12+MEM_SIZE);index++){
		*(CP+index) = 0;
	}
}


void fetchInstruction(Environment *simulation);
void decodeInstruction(Instruction *instruction,Byte *rs,Byte *rd,Byte *rt,Byte *sa,Byte imm[2],DWord target);
void fetchData(Instruction *instruction, Environment *simulation,Byte *rs,Byte *rd,Byte *rt,Byte *sa,Byte imm[2],DWord target);
void processData(Instruction *instruction, Environment *simulation,Byte *rs,Byte *rd,Byte *rt,Byte *sa,Byte imm[2],DWord target);
void writeResult(Instruction *instruction, Environment *simulation,Byte *rs,Byte *rd,Byte *rt,Byte *sa,Byte imm[2],DWord target);

void simulate(Instruction *instruction, Environment *simulation){
	/*déterminer les espace temporaires nécéssaires: 
		type R: rs(5) rt(5) rd(5) sa(5)
		type I: rs(5) rt(5) imm(16)
		type J: target(26)*/
	Byte rs=0,rd=0,rt=0,sa=0;
	Byte imm[2];
	DWord target;
	imm[0]=0;imm[1]=0;
	target[0]=0;target[1]=0;target[2]=0;target[3]=0;

	/*pour référence, ne fait rien les instructions sont passées en paramètre*/
	fetchInstruction(simulation);

	/*calculer les indexes de registres et/ou adresses mémoire*/
	decodeInstruction(instruction,&rs,&rd,&rt,&sa,imm,target);

	/*récupere les données,  si ld charge dans les registres*/
	fetchData(instruction,simulation,&rs,&rd,&rt,&sa,imm,target);

	/*execute l'opération (addition soustraction jump ...)*/
	processData(instruction,simulation,&rs,&rd,&rt,&sa,imm,target);
	/*écrit résultat, écrit en mémoire ou sw*/
	writeResult(instruction,simulation,&rs,&rd,&rt,&sa,imm,target);
}

void fetchInstruction(Environment *simulation){
	incr4(simulation->PC, 4);
}

/*
récupère les valeurs dans le code instruction
*/
void decodeInstruction(Instruction *instruction,Byte *rs,Byte *rd,Byte *rt,Byte *sa,Byte imm[2],DWord target){
	unsigned int type=instruction->id&OPCODE_TYPE_MASK;
	switch(type){
		case OPCODE_TYPE_R:
			(*rs) =((instruction->code[0]&0x03)<<3)+ /*000000XX -> 000XX000*/
			       ((instruction->code[1]&0xE0)>>5); /*XXX00000 -> 00000XXX*/
			(*rt) = instruction->code[1]&0x1F; /*000XXXXX*/

			(*rd) = (instruction->code[2]&0xF8)>>3; /*XXXXX000 -> 000XXXXX*/

			(*sa) =((instruction->code[2]&0x07)<<2)+ /*00000XXX -> 000XXX00*/
				   ((instruction->code[3]&0xC0)>>6); /*XX000000 -> 000000XX*/
		break;
		case OPCODE_TYPE_I:
			/*opcode | rs | rt | imm*/
			(*rs) =((instruction->code[0]&0x03)<<3)+ /*000000XX -> 000XX000*/
			       ((instruction->code[1]&0xE0)>>5); /*XXX00000 -> 00000XXX*/
			(*rt) = instruction->code[1]&0x1F; /*000XXXXX*/
			imm[0]=instruction->code[2];
			imm[1]=instruction->code[3];
		break;
		case OPCODE_TYPE_J:
			/*on copie juste les 26bits avec un masque sur les 6 premiers*/
			copy_Byte4(instruction->code,target);
			target[0]=target[0]&0x03; /*enleve 6 premiers bits*/
		break;
	}
}

void fetchData(Instruction *instruction, Environment *simulation,Byte *rs,Byte *rd,Byte *rt,Byte *sa,Byte imm[2],DWord target){
	/*dans cette architecture il n'y a que l'instruction ld qui utlise cette étape*/
	DWord adresse;
	int immval = (imm[0]<<8)+imm[1];
	if(instruction->id == LW_ID){
		/*LW | base | rt | offset*/
		/*     rs   | rt | offset*/
		/*rs est le registre dans le quel l'adresse est stocker, offset et l'incrément, rt est la destination*/
		copy_Byte4(simulation->registers[(*rs)],adresse);
		/*de ByteUtils, a tester pour valeurs négatives*/
		addToByte(adresse,immval);

		simulation->registers[(*rt)][0] = readRamADDR(simulation,adresse);
		incr(adresse, 4); /*+1*/
		simulation->registers[(*rt)][1] = readRamADDR(simulation,adresse);
		incr(adresse, 4); /*+2*/
		simulation->registers[(*rt)][2] = readRamADDR(simulation,adresse);
		incr(adresse, 4); /*+3*/
		simulation->registers[(*rt)][3] = readRamADDR(simulation,adresse);
	}
}

void processData(Instruction *instruction, Environment *simulation,Byte *rs,Byte *rd,Byte *rt,Byte *sa,Byte imm[2],DWord target){
	switch(instruction->id){
		case ADD_ID:
			/*GPR[rd] ← GPR[rs] + GPR[rt]*/
			/*simulation->registers[*rd]=simulation->registers[*rs]+simulation->registers[*rt];*/
			break;

		case ADDI_ID:
			break;

		case AND_ID:
			break;

		case BEQ_ID:
			break;

		case BGTZ_ID:
			break;

		case BLEZ_ID:
			break;

		case BNE_ID:
			break;

		case DIV_ID:
			break;

		case J_ID:
			printf("Instruction J a implementer\n");
			break;

		case JAL_ID:
			printf("Instruction JAL a implementer\n");
			break;

		case JR_ID:
			break;

		case LUI_ID:
			break;

		case MFHI_ID:
			break;

		case MFLO_ID:
			break;

		case MULT_ID:		
			break;

		case NOP_ID:
			break;

		case OR_ID:
			break;

		case ROTR_ID:
			break;

		case SLL_ID:
			break;

		case SLT_ID:
			break;

		case SRL_ID:
			break;

		case SUB_ID:
			break;

		case SYSCALL_ID:
			/*code sur 20 bit pas de valeur donnée*/
			break;

		case XOR_ID:
			break;

	}
}

void writeResult(Instruction *instruction, Environment *simulation,Byte *rs,Byte *rd,Byte *rt,Byte *sa,Byte imm[2],DWord target){
	/*dans cette architecture il n'y a que l'instruction sw qui utlise cette étape*/
	DWord adresse;
	
	int immval = (imm[0]<<8)+imm[1];

	if(instruction->id == LW_ID){
		/*LW | base | rt | offset*/
		/*     rs   | rt | offset*/
		/*rs est le registre dans le quel l'adresse est stocker, offset et l'incrément, rt est la destination*/
		copy_Byte4(simulation->registers[(*rs)],adresse);
		/*de ByteUtils, a tester pour valeurs négatives*/
		addToByte(adresse,immval);

		writeRamADDR(simulation,adresse,simulation->registers[(*rt)][0]);
		incr(adresse, 4); /*+1*/
		writeRamADDR(simulation,adresse,simulation->registers[(*rt)][1]);
		incr(adresse, 4); /*+2*/
		writeRamADDR(simulation,adresse,simulation->registers[(*rt)][2]);
		incr(adresse, 4); /*+3*/
		writeRamADDR(simulation,adresse,simulation->registers[(*rt)][3]);
	}
}