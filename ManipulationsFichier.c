#include "ManipulationsFichier.h"



void readInstruction(FILE *f, char *result){
	char c = 'a';
	int loop=1;
	int i =0;
	/*
	lis les caractères uns a uns et s'arrête si il trouve un retour a la ligne ou un debut de commentaire
	*/
	while(loop){
		c = fgetc(f);
		loop=(!feof(f)) && (c != '\n') && (c != '#');

		if (loop){
			/* met la valeur dans resultat */

			result[i]= c;
			/* avance dans la chaine de caractères */
			i++;
		}
	}

	result[i] = '\0';

	/*
	dans le cas ou il trouve un commentaire on continue de lire la ligne jusqu'au bout pour avancer le curseur jusqu'a la fin de la ligne
	*/
	while((!feof(f)) && (c != '\n')){
		c=fgetc(f);
	}
}

void writeHexInstructionToFile(FILE *f, Instruction inst){
	fprintf(f, "%02x%02x%02x%02x\n", inst.code[0],inst.code[1],inst.code[2],inst.code[3]);
}

void ecrireProgram(FILE *f, Program prog){
	int i=0;
	while(prog[i].id != HALT_ID){
		writeHexInstructionToFile(f, prog[i]);
		i++;
	}
}

void ecrireRegistres(FILE *f, Environment *sim){
	int i,j;
	for(i=0;i<8;i++){
		j=i*4;
		fprintf(f,"$%d:%ld [%02x%02x %02x%02x]    ", j,DWordToLong(sim->registers[j]),sim->registers[j][0],sim->registers[j][1],sim->registers[j][2],sim->registers[j][3]);
		j++;
		fprintf(f,"$%d:%ld [%02x%02x %02x%02x]    ", j,DWordToLong(sim->registers[j]),sim->registers[j][0],sim->registers[j][1],sim->registers[j][2],sim->registers[j][3]);
		j++;
		fprintf(f,"$%d:%ld [%02x%02x %02x%02x]    ", j,DWordToLong(sim->registers[j]),sim->registers[j][0],sim->registers[j][1],sim->registers[j][2],sim->registers[j][3]);
		j++;
		fprintf(f,"$%d:%ld [%02x%02x %02x%02x]\n", j,DWordToLong(sim->registers[j]),sim->registers[j][0],sim->registers[j][1],sim->registers[j][2],sim->registers[j][3]);
	}
	fprintf(f,"\nPC:%02x%02x %02x%02x    HI:%ld [%02x%02x %02x%02x]    LO:%ld [%02x%02x %02x%02x]\n\n", 
				sim->PC[0],sim->PC[1],sim->PC[2],sim->PC[3],
				DWordToLong(sim->HI),sim->HI[0],sim->HI[1],sim->HI[2],sim->HI[3],
				DWordToLong(sim->LO),sim->LO[0],sim->LO[1],sim->LO[2],sim->LO[3]);
}

void supprimerEspaces(char *line){
	line[0]='z';
}