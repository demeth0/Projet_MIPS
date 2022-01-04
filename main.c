#include <stdio.h>
#include "compiler.h"
#include "emulateur.h"
#include "MipsSimulateur.h"

void printInst(char *str_inst,Instruction *inst){
	printf("Instruction : \n\t\"%s\"\n\tID: %d\n\tHex: %02x%02x %02x%02x\n\tBlocks size: %d %d %d %d %d %d\n", 
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
				inst->b[5]);
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
					initInst(&instr);
					readInstruction(fichier,line);
					if(compileline(line, &instr)){
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
					initInst(&instr);
					if(run && compileline(line, &instr)){
						writeHexInstructionToFile(fichier_sortie,instr);
					}
				}
			}
			
			fclose(fichier_sortie);
		}
	}
	
	return 0;
}
