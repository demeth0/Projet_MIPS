#include "module_emulateur.h"

/*initialise simulation*/
void initSimulation(Environment *simulation){
	int index =0;
	Byte *CP = simulation->RAM;

	/*comme tout est a la suite dans une structure, cela permet de tout
	  initialiser a 0 (RAM avec MEM_SIZE puis registres avec 32*4 puis le PC,HI et LOW avec +12...)*/
	for(index =0;index<(32*4+12+MEM_SIZE);index++){
		*(CP+index) = 0;
	}
}

void fetchInstruction(Program prog,Environment *simulation,int *i);
void decodeInstruction(Instruction *instruction,Byte *rs,Byte *rd,Byte *rt,Byte *sa,DWord imm,DWord target);
void fetchData(Instruction *instruction, Environment *simulation,Byte rs,Byte rd,Byte rt,Byte sa,DWord imm,DWord target);
void processData(Instruction *instruction, Environment *simulation,Byte rs,Byte rd,Byte rt,Byte sa,DWord imm,DWord target);
void writeResult(Instruction *instruction, Environment *simulation,Byte rs,Byte rd,Byte rt,Byte sa,DWord imm,DWord target);

void printInst(Environment *sim, Instruction inst){
	printf("%02X%02X%02X%02X %02X%02X%02X%02X %s\n", sim->PC[0],sim->PC[1],sim->PC[2],sim->PC[3],
								  inst.code[0],inst.code[1],inst.code[2],inst.code[3],inst.text_instr);
}

void simulate(Instruction *instruction, Environment *simulation){
	/*determiner les espace temporaires necessaires: 
		type R: rs(5) rt(5) rd(5) sa(5)
		type I: rs(5) rt(5) imm(16)
		type J: target(26)*/
	Byte rs=0,rd=0,rt=0,sa=0;
	DWord imm;
	DWord target;
	imm[0]=0;imm[1]=0,imm[2]=0,imm[3]=0;
	target[0]=0;target[1]=0;target[2]=0;target[3]=0;

	/*pour reference, ne fait rien les instructions sont passees en paramètre
	  incremente le PC de 4*/
	incr4(simulation->PC);

	/*calculer les indexes de registres et/ou adresses memoire*/
	decodeInstruction(instruction,&rs,&rd,&rt,&sa,imm,target);
	
	/*recupere les donnees,  si ld charge dans les registres*/
	fetchData(instruction,simulation,rs,rd,rt,sa,imm,target);

	/*execute l'operation (addition soustraction jump ...)*/
	processData(instruction,simulation,rs,rd,rt,sa,imm,target);
	/*ecrit resultat, ecrit en memoire ou sw*/
	writeResult(instruction,simulation,rs,rd,rt,sa,imm,target);
}

void simulateProgram(Program prog,Environment *simulation,int sequential){
	int i=0;
	Byte rs=0,rd=0,rt=0,sa=0;
	DWord imm;
	DWord target;

	/*mise du PC a zero*/
	simulation->PC[0]=0;	
	simulation->PC[1]=0;
	simulation->PC[2]=0;
	simulation->PC[3]=0;

	while(prog[i].id != HALT_ID){
		rs=0;rd=0;rt=0;sa=0;
		imm[0]=0;imm[1]=0,imm[2]=0,imm[3]=0;
		target[0]=0;target[1]=0;target[2]=0;target[3]=0;
		if(sequential){
			printf("next>");
			/*wait enter*/
			/*fgetc(stdin);*/
			
			getchar();
		}
		/*calcule l'index dans le programme*/
		fetchInstruction(prog,simulation,&i);
		
		if(prog[i].id != HALT_ID){
			/*calculer les indexes de registres et/ou adresses memoire*/
			decodeInstruction(prog+i,&rs,&rd,&rt,&sa,imm,target);
		
			/*recupere les donnees,  si ld charge dans les registres*/
			fetchData(prog+i,simulation,rs,rd,rt,sa,imm,target);

			/*execute l'operation (addition soustraction jump ...)*/
			processData(prog+i,simulation,rs,rd,rt,sa,imm,target);
			/*ecrit resultat, ecrit en memoire ou sw*/
			writeResult(prog+i,simulation,rs,rd,rt,sa,imm,target);
			if(sequential){
				printInst(simulation,prog[i]);
			} 
		}
	}

}

void afficher_registres(Environment *sim){
	int i,j;
	for(i=0;i<8;i++){
		j=i*4;
		printf("$%d:%ld [%02x%02x %02x%02x]    ", j,DWordToLong(sim->registers[j]),sim->registers[j][0],sim->registers[j][1],sim->registers[j][2],sim->registers[j][3]);
		j++;
		printf("$%d:%ld [%02x%02x %02x%02x]    ", j,DWordToLong(sim->registers[j]),sim->registers[j][0],sim->registers[j][1],sim->registers[j][2],sim->registers[j][3]);
		j++;
		printf("$%d:%ld [%02x%02x %02x%02x]    ", j,DWordToLong(sim->registers[j]),sim->registers[j][0],sim->registers[j][1],sim->registers[j][2],sim->registers[j][3]);
		j++;
		printf("$%d:%ld [%02x%02x %02x%02x]\n", j,DWordToLong(sim->registers[j]),sim->registers[j][0],sim->registers[j][1],sim->registers[j][2],sim->registers[j][3]);
	}
	printf("\nPC:%02x%02x %02x%02x    HI:%ld [%02x%02x %02x%02x]    LO:%ld [%02x%02x %02x%02x]\n\n", 
				sim->PC[0],sim->PC[1],sim->PC[2],sim->PC[3],
				DWordToLong(sim->HI),sim->HI[0],sim->HI[1],sim->HI[2],sim->HI[3],
				DWordToLong(sim->LO),sim->LO[0],sim->LO[1],sim->LO[2],sim->LO[3]);
}


void fetchInstruction(Program prog,Environment *simulation,int *i){
	DWord temp;

	copyDWord(temp,simulation->PC);
	shiftRDWord(temp,2); /*division par 4*/

	/*traduire le PC en un index*/
	if(sizeof(int) == 4){
		*i= temp[3];
		*i+= (temp[2])<<8;
		*i+= (temp[1])<<16;
		*i+= (temp[0])<<24;
	}
	else if(sizeof(int) == 2){
		*i= (temp[1]);
		*i+= (temp[0])<<8;
	}

	/*PC +4*/
	incr4(simulation->PC);
}

/*
recupère les valeurs dans le code instruction
*/
void decodeInstruction(Instruction *instruction,Byte *rs,Byte *rd,Byte *rt,Byte *sa,DWord imm,DWord target){
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


			imm[2]=instruction->code[2];
			imm[3]=instruction->code[3];

			/*sign extension si imm etait signe comme imm code sur 32bit
			on met les 16 bits de poid forts identique au 16eme bit*/
			if((imm[2]&0x80) != 0){
				imm[0]=0xFF;
				imm[1]=0xFF;
			}else{
				imm[0]=0;
				imm[1]=0;
			}
		break;
		case OPCODE_TYPE_J:
			/*on copie juste les 26bits avec un masque sur les 6 premiers*/
			copyDWord(instruction->code,target);
			target[0]=target[0]&0x03; /*enleve 6 premiers bits*/
		break;
	}
}

void fetchData(Instruction *instruction, Environment *simulation,Byte rs,Byte rd,Byte rt,Byte sa,DWord imm,DWord target){
	/*dans cette architecture il n'y a que l'instruction ld qui utlise cette etape*/
	DWord adresse;
	DWord *GPR=simulation->registers;
	if(instruction->id == LW_ID){
		/*LW | base | rt | offset*/
		/*     rs   | rt | offset*/
		/*recupère adresse dans registre*/
		copyDWord(adresse,GPR[rs]);
		/*ajoute offset*/
		addDWord(adresse,imm);
		if((adresse[3]&0x03) != 0){
			/*si l'adresse n'est pas paire de 4*/
			printf("Adress error exception\n");
		}else{
			GPR[rt][0] = readRamADDR(simulation,adresse);
			incr(adresse); /*+1*/
			GPR[rt][1] = readRamADDR(simulation,adresse);
			incr(adresse); /*+2*/
			GPR[rt][2] = readRamADDR(simulation,adresse);
			incr(adresse); /*+3*/
			GPR[rt][3] = readRamADDR(simulation,adresse);
		}
	}
}

void processData(Instruction *instruction, Environment *simulation,Byte rs,Byte rd,Byte rt,Byte sa,DWord imm,DWord target){
	DWord *GPR = simulation->registers;
	DWord temp;
	int overflow;

	switch(instruction->id){
		case ADD_ID:
			/*GPR[rd] ← GPR[rs] + GPR[rt]*/
			copyDWord(temp,GPR[rs]);
			overflow = addDWord(temp,GPR[rt]);
			if(overflow==0){
				copyDWord(GPR[rd],temp);
			}else{
				/*SignalException(IntegerOverflow)*/
			}
			break;

		case ADDI_ID:
			/*GPR[rt] <- GPR[rs] + immedia*/
			copyDWord(temp,GPR[rs]);
			overflow = addDWord(temp,imm); /*signe deja etendue en 32bit*/
			if(overflow==0){
				copyDWord(GPR[rt],temp);
			}else{
				/*SignalException(IntegerOverflow)*/
			}
			break;

		case AND_ID:
			/*GPR[rd] ← GPR[rs] AND GPR[rt*/
			copyDWord(GPR[rd],GPR[rs]);
			GPR[rd][0] &= GPR[rt][0];
			GPR[rd][1] &= GPR[rt][1];
			GPR[rd][2] &= GPR[rt][2];
			GPR[rd][3] &= GPR[rt][3];
			break;

		case BEQ_ID:
			/*if GPR[rs] = GPR[rt] then branc*/
			if(equalsDWord(GPR[rs],GPR[rt])){
				/*PC+4 + (offset<<2)*/
				/*attention  PC deja incremente de +4 dans fetchInstruction*/
				shiftLDWord(imm,2);
				addDWord(simulation->PC,imm);
			}
			break;

		case BGTZ_ID:
			/*if GPR[rs]>0 then branc*/
			if(greaterThanZeroDWord(GPR[rs])){
				/*PC+4 + (offset<<2)*/
				/*attention  PC deja incremente de +4 dans fetchInstruction*/
				shiftLDWord(imm,2);
				addDWord(simulation->PC,imm);
			}
			break;

		case BLEZ_ID:
			/*if GPR[rs]<=0 then branc*/
			if(!greaterThanZeroDWord(GPR[rs]) || (GPR[rs][0]|GPR[rs][1]|GPR[rs][2]|GPR[rs][3])==0){
				/*PC+4 + (offset<<2)*/
				/*attention  PC deja incremente de +4 dans fetchInstruction*/
				shiftLDWord(imm,2);
				addDWord(simulation->PC,imm);
			}
			break;

		case BNE_ID:
			/*if GPR[rs] != 0*/
			if((GPR[rs][0]|GPR[rs][1]|GPR[rs][2]|GPR[rs][3])!=0){
				/*PC+4 + (offset<<2)*/
				/*attention  PC deja incremente de +4 dans fetchInstruction*/
				shiftLDWord(imm,2);
				addDWord(simulation->PC,imm);
			}
			break;

		case DIV_ID:
			/*(HI, LO) ← GPR[rs] / GPR[rt]*/
			divideDWord(simulation->HI,simulation->LO,GPR[rs],GPR[rt]);
			break;

		case J_ID:
			printf("Instruction J a implementer\n");
			break;

		case JAL_ID:
			printf("Instruction JAL a implementer\n");
			break;

		case JR_ID:
			copyDWord(simulation->PC,GPR[rs]);
			break;

		case LUI_ID:
			/*GPR[rt] ← immediate || 016*/
			shiftLDWord(imm,16);
			copyDWord(GPR[rt],imm);
			break;

		case MFHI_ID:
			/*GPR[rd] ← HI*/
			copyDWord(GPR[rd],simulation->HI);
			break;

		case MFLO_ID:
			/*GPR[rd] ← LO*/
			copyDWord(GPR[rd],simulation->LO);
			break;

		case MULT_ID:	
			multiplyDWord(simulation->HI,simulation->LO,GPR[rs],GPR[rt]);	
			break;

		case NOP_ID:
			break;

		case OR_ID:
			/* GPR[rd] ← GPR[rs] or GPR[rt*/
			GPR[rd][0]=GPR[rs][0] | GPR[rt][0];
			GPR[rd][1]=GPR[rs][1] | GPR[rt][1];
			GPR[rd][2]=GPR[rs][2] | GPR[rt][2];
			GPR[rd][3]=GPR[rs][3] | GPR[rt][3];
			break;

		case ROTR_ID:
			copyDWord(temp,GPR[rt]);
			copyDWord(GPR[rd],GPR[rt]);
			shiftRDWord(temp,sa); /*GPR[rt]31..s*/
			shiftLDWord(GPR[rd],sa);/*GPR[rt]s-1..0*/
			addDWord(GPR[rd],temp);
			break;

		case SLL_ID:
			/*GPR[rd] ← GPR[rt] << sa*/
			copyDWord(GPR[rd],GPR[rt]);
			shiftLDWord(GPR[rd],sa);
			break;

		case SLT_ID:
			/*GPR[rd] ← (GPR[rs] < GPR[rt])*/
			/*GPR[rd] ← (GPR[rs]-GPR[rt] < 0)*/
			copyDWord(temp,GPR[rs]);
			subDWord(temp,GPR[rt]);
			twoComplementDWord(temp);
			IntegerToDWord(greaterThanZeroDWord(temp),GPR[rd]);
			break;

		case SRL_ID:
			/* GPR[rd] ← GPR[rt] >> sa*/
			copyDWord(GPR[rd],GPR[rt]);
			shiftRDWord(GPR[rd],sa);
			break;

		case SUB_ID:
			/*GPR[rd] ← GPR[rs] - GPR[rt]*/
			copyDWord(temp,GPR[rs]);
			overflow = subDWord(temp,GPR[rt]);
			if(overflow==0){
				copyDWord(GPR[rd],temp);
			}else{
				/*SignalException(IntegerOverflow)*/
			}
			break;

		case SYSCALL_ID:
			/*code sur 20 bit pas de valeur donnee*/
			break;

		case XOR_ID:
			/*GPR[rd] ← GPR[rs] XOR GPR[rt]*/
			GPR[rd][0]=GPR[rs][0] ^ GPR[rt][0];
			GPR[rd][1]=GPR[rs][1] ^ GPR[rt][1];
			GPR[rd][2]=GPR[rs][2] ^ GPR[rt][2];
			GPR[rd][3]=GPR[rs][3] ^ GPR[rt][3];
			break;

	}
}

void writeResult(Instruction *instruction, Environment *simulation,Byte rs,Byte rd,Byte rt,Byte sa,DWord imm,DWord target){
	/*dans cette architecture il n'y a que l'instruction sw qui utlise cette etape*/
	DWord adresse;
	DWord *GPR = simulation->registers;

	if(instruction->id == SW_ID){
		/*SW | base | rt | offset*/
		/*     rs   | rt | offset*/
		/*recupère adresse dans registre*/
		copyDWord(adresse,GPR[rs]);
		/*ajoute offset*/
		addDWord(adresse,imm);
		if((adresse[3]&0x03) != 0){
			/*si l'adresse n'est pas paire de 4*/
			printf("Adress error exception\n");
		}else{
			writeRamADDR(simulation,adresse,GPR[rt][0]);
			incr(adresse); /*+1*/
			writeRamADDR(simulation,adresse,GPR[rt][1]);
			incr(adresse); /*+2*/
			writeRamADDR(simulation,adresse,GPR[rt][2]);
			incr(adresse); /*+3*/
			writeRamADDR(simulation,adresse,GPR[rt][3]);
		}
	}
}