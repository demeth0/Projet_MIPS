#include <stdio.h>
#include "InstructionCompiler.h"
#include "ManipulationsFichier.h"

void printInst(char *str_inst,Instruction inst){
	printf("Instruction : \n\t\"%s\"\n\tID: %d\n\tHex: %02x%02x %02x%02x\n\tBlocks size: %d %d %d %d %d %d %d %d\n", 
				str_inst,
				inst.id,
				inst.code[0],
				inst.code[1],
				inst.code[2],
				inst.code[3],
				inst.b[0],
				inst.b[1],
				inst.b[2],
				inst.b[3],
				inst.b[4],
				inst.b[5],
				inst.b[6],
				inst.b[7]);
}

int main(int argc, char const *argv[])
{


	int seq=0;
	FILE *fichier=NULL;
	char dmp;

	int parseur = 1;
	char line[128];
	Instruction instr;
	FILE *fichier_sortie = fopen("output.txt","w+");
	if(fichier_sortie!= NULL){
		while(parseur< argc){
			if(!strcmp("-pas",argv[parseur]))
				seq=1;
			else{
				fichier = fopen(argv[parseur],"r");
				if (fichier ==NULL){
					printf("erreur: fichier non valide");
					exit(EXIT_FAILURE);
				}
			}
			parseur++;
		}

		printf("%d\n",seq);
		if (fichier == NULL)
			printf("null\n");
		else 
			printf("non null\n");



		if(fichier !=NULL){ /*mode fichier activé*/
			while(!feof(fichier)){
				readInstruction(fichier,line);
				format_instr(line);
				initInst(&instr);
				textInstructionToOpcode(line,&instr);

				writeInstructionOperands(&instr, line);
				writeHexInstructionToFile(fichier_sortie,instr);
				printInst(line,instr);
				if(seq){
					printf("next>");
					scanf("n%1c\n",&dmp);
				}
			}
			printf("\n");
			fclose(fichier);
		}
		else{/*mode sequentiel à la main activé*/
			while(strcmp("end",line)){ /*tant que line != "end"*/
				printf("instruction>");
				fgets(line,128,stdin);
				format_instr(line);
				initInst(&instr);
				textInstructionToOpcode(line,&instr);

				writeInstructionOperands(&instr, line);
				writeHexInstructionToFile(fichier_sortie,instr);
				printInst(line,instr);
			}
		}

		
		fclose(fichier_sortie);
	}
	return 0;
}