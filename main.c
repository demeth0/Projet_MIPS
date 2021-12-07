#include <stdio.h>
#include "InstructionCompiler.h"
#include "ManipulationsFichier.h"

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

void processLine(char *line, Instruction *inst){
	format_instr(line);
	textInstructionToOpcode(line,inst);
	writeInstructionOperands(inst, line);
	printInst(line,inst);
}

int main(int argc, char const *argv[])
{
	int seq=0;
	FILE *fichier=NULL;
	FILE *fichier_sortie=NULL;
	char dmp;

	int parseur=1;
	char line[128];
	Instruction instr;

	if(argc <= 3){
		fichier_sortie = fopen("output.txt","w+");
		initInst(&instr);

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
					processLine(line, &instr);
					writeHexInstructionToFile(fichier_sortie,instr);

					if(seq){
						printf("next>");
						scanf("n%1c\n",&dmp);
					}
				}
				printf("\n");
				fclose(fichier);
			}else{/*mode sequentiel à la main activé*/
				while(strcmp("end",line)){ /*tant que line != "end"*/
					printf("instruction>");
					fgets(line,128,stdin);
					processLine(line, &instr);
					writeHexInstructionToFile(fichier_sortie,instr);
				}
			}
			
			fclose(fichier_sortie);
		}
	}
	
	return 0;
}