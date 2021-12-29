#include <stdio.h>
#include "InstructionCompiler.h"
#include "ManipulationsFichier.h"
#include "MipsSimulateur.h"

void printInst(char *str_inst,Instruction *inst){
	printf("Instruction : \n\t\"%s\"\n\tID: %d\n\tHex: %02x%02x %02x%02x\n\tBlocks size: %d %d %d %d %d %d %d %d\n", 
				str_inst,
				inst->id,
				inst->code[0],
				inst->code[1],
				inst->code[2],
				inst->code[3],
				inst->b[0],
				inst->b[1],
				inst->b[2],
				inst->b[3],
				inst->b[4],
				inst->b[5],
				inst->b[6],
				inst->b[7]);
}

/*
return:
	success : la commande a été traduite et executée sans erreurs
*/
int processLine(char *line, Instruction *inst){
	int state=1;
	initInst(inst); /* remet l'instruction a 0 a chaque appel */
	format_instr(line); /*supprimes espaces en trop*/
	textInstructionToOpcode(line,inst); /*détermine le type d'instruction*/
	if(inst->id == UNKNOWN_ID){
		printf("instruction non connue\n");
		state=0;
	}else{
		writeInstructionOperands(inst, line);
		printInst(line,inst);
	}
	
	return state;
}

void waitForNext(){
	fgetc(stdin);
}

int main(int argc, char const *argv[])
{
	int seq=0;
	int run=1;

	FILE *fichier=NULL;
	FILE *fichier_sortie=NULL;

	int parseur=1;
	char line[128];
	Instruction instr;


	if(argc <= 3){
		fichier_sortie = fopen("output.txt","w+");

		if(fichier_sortie!= NULL){
			while(parseur<argc){
				if(!strcmp(argv[parseur],"-pas")){
					seq=1;
				}else{
					fichier = fopen(argv[parseur],"r");
					if (fichier ==NULL){
						printf("erreur: fichier non valide");
						exit(EXIT_FAILURE);
					}
				}
				parseur++;
			}

			if(fichier !=NULL){ /*mode fichier activé*/
				while(!feof(fichier)){
					readInstruction(fichier,line);
					if(processLine(line, &instr)){
						writeHexInstructionToFile(fichier_sortie,instr);
					}

					if(seq){
						printf("next>");
						waitForNext();
					}
				}
				printf("\n");
				fclose(fichier);
			}else{/*mode sequentiel à la main activé*/
				while(run){ /*tant que line != "end"*/
					printf("instruction>");
					readInstruction(stdin,line);
					run = strcmp(line,"end");
					if(run && processLine(line, &instr)){
						writeHexInstructionToFile(fichier_sortie,instr);
					}
				}
			}
			
			fclose(fichier_sortie);
		}
	}
	
	return 0;
}
