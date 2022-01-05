#include <stdio.h>
#include "module_compilateur.h"
#include "module_emulateur.h"
#include "ManipulationsFichier.h"

void interpreteur(Environment *sim){
	int run=1;
	char line[256];
	Instruction instr;

	while(run){ /*tant que line != "end"*/
		printf("instruction>");
		readInstruction(stdin,line);
		run = strcmp(line,"end");
		initInst(&instr);
		if(run && compileline(line, &instr)){
			printInst(sim,instr,line);
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
