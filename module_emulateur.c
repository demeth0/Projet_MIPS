#include "module_emulateur.h"


/*permet de recopier dans sortie le contenue de l'entrée*/
void copy_Byte4(Byte entree[4],Byte sortie[4]){
	sortie[0] = entree[0];
	sortie[1] = entree[1];
	sortie[2] = entree[2];
	sortie[3] = entree[3];
}


/*
écrit dans memoire ram a adresse
*/
void writeRamADDR(Environment *simulation,Byte adresse[4],Byte value){
	int new_addr;
	/*si on est sur une architecture 32 bits*/
	if(sizeof(int) == 4){
		new_addr= adresse[3];
		new_addr+= (adresse[2])<<8;
		new_addr+= (adresse[1])<<16;
		new_addr+= (adresse[0])<<24;
	}
	else if(sizeof(int) == 2){
		new_addr= (adresse[1]);
		new_addr+= (adresse[0])<<8;
	}
	simulation->RAM[new_addr]=value;
}

/*lis dans memoire a adresse*/
Byte readRamADDR(Environment *simulation,Byte adresse[4]){
	/*on calcule l'index dans le tableau*/
	/*si on est sur une architecture 32 bits*/

	int new_addr;
	if(sizeof(int) == 4){
		new_addr= adresse[3];
		new_addr+= (adresse[2])<<8;
		new_addr+= (adresse[1])<<16;
		new_addr+= (adresse[0])<<24;
	}
	else if(sizeof(int) == 2){
		new_addr= (adresse[1]);
		new_addr+= (adresse[0])<<8;
	}
	return simulation->RAM[new_addr];
}

/*affiche toute la ram*/
void afficher_ram(Environment *simulation){

	int index =0;

	for(index =0;index<MEM_SIZE;index++){
		printf("%02x ",simulation->RAM[index]);
	}
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
void decodeInstruction(Instruction *instruction,Byte *rs,Byte *rd,Byte *rt,Byte *sa,Byte imm[2],Byte target[4]);
void fetchData(Instruction *instruction, Environment *simulation,Byte *rs,Byte *rd,Byte *rt,Byte *sa,Byte imm[2],Byte target[4]);
void processData(Instruction *instruction, Environment *simulation,Byte *rs,Byte *rd,Byte *rt,Byte *sa,Byte imm[2],Byte target[4]);
void writeResult(Instruction *instruction, Environment *simulation,Byte *rs,Byte *rd,Byte *rt,Byte *sa,Byte imm[2],Byte target[4]);

void simulate(Instruction *instruction, Environment *simulation){
	/*déterminer les espace temporaires nécéssaires: 
		type R: rs(5) rt(5) rd(5) sa(5)
		type I: rs(5) rt(5) imm(16)
		type J: target(26)*/
	Byte rs=0,rd=0,rt=0,sa=0;
	Byte imm[2];
	Byte target[4];
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
void decodeInstruction(Instruction *instruction,Byte *rs,Byte *rd,Byte *rt,Byte *sa,Byte imm[2],Byte target[4]){
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

void fetchData(Instruction *instruction, Environment *simulation,Byte *rs,Byte *rd,Byte *rt,Byte *sa,Byte imm[2],Byte target[4]){
	/*dans cette architecture il n'y a que l'instruction ld qui utlise cette étape*/
	Byte adresse[4];
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

void processData(Instruction *instruction, Environment *simulation,Byte *rs,Byte *rd,Byte *rt,Byte *sa,Byte imm[2],Byte target[4]){

}

void writeResult(Instruction *instruction, Environment *simulation,Byte *rs,Byte *rd,Byte *rt,Byte *sa,Byte imm[2],Byte target[4]){
	/*dans cette architecture il n'y a que l'instruction sw qui utlise cette étape*/
	Byte adresse[4];
	
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