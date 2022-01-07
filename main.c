#include <stdio.h>
#include "module_compilateur.h"
#include "module_emulateur.h"
#include "ManipulationsFichier.h"

void interpreteur(Environment *sim){
	int run=1;
	char line[LINE_MAX_WIDTH];
	Instruction instr;

	while(run){ /*tant que line != "end"*/
		printf("instruction>");
		readInstruction(stdin,line);
		if(strcmp(line,"end")==0){
			run=0;
		}else if(strcmp(line,"help")==0){
			printf("view ram - affiche la ram\nview registres - affiche les registres\n");
		}else if(strcmp(line,"view ram")==0){
			afficher_ram(sim);
		}else if(strcmp(line,"view registres")==0){
			afficher_registres(sim);
		}else{
			initInst(&instr);
			if(compileline(line, &instr)){
				printInst(sim,instr,line);
				simulate(&instr, sim);
			}else{
				printf("echec de compilation\n");
			}
		}

	}
}

int main(int argc, char const *argv[])
{
	int seq=0;
	int i;
	int source_pos;
	Environment simulation;

	initSimulation(&simulation);

	if(argc==1){
		printf("*************************************\n");
		printf("*         mode interpreteur         *\n");
		printf("*************************************\n");
		interpreteur(&simulation);
	}else if(argc <= 3){
		for(i=1;i<argc;i++){
			if(strcmp(argv[i],"-pas")==0){
				seq=1;
			}else{
				source_pos=i;
			}
		}

		compile(argv[source_pos],"output.txt");

		if(seq){
			printf("*************************************\n");
			printf("*        mode non-interactif        *\n");
			printf("*             sequentiel            *\n");
			printf("*************************************\n");
		}else{
			printf("*************************************\n");
			printf("*        mode non-interactif        *\n");
			printf("*           non sequentiel          *\n");
			printf("*************************************\n");
		}
		
		simulateFile(argv[source_pos],&simulation,seq);
	}
	
	return 0;
}
