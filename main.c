#include <stdio.h>
#include "module_compilateur.h"
#include "module_emulateur.h"
#include "ManipulationsFichier.h"

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



void interpreteur(){
	int run=1;
	char line[256];
	Instruction instr;

	while(run){ /*tant que line != "end"*/
		printf("instruction>");
		readInstruction(stdin,line);
		run = strcmp(line,"end");
		initInst(&instr);
		if(run && compileline(line, &instr)){
			printInst(line,&instr);
		}
	}
}

int main(int argc, char const *argv[])
{
	int seq=0;
	int i;
	int source_pos;

	if(argc==1){
		printf("*************************************\n");
		printf("*         mode interpréteur         *\n");
		printf("*************************************\n");
		interpreteur();
	}else if(argc <= 3){
		for(i=1;i<argc;i++){
			if(strcmp(argv[i],"-pas")==0){
				seq=1;
			}else{
				source_pos=i;
			}
		}
		if(seq){
			printf("*************************************\n");
			printf("*        mode non-intéractif        *\n");
			printf("*             séquentiel            *\n");
			printf("*************************************\n");
			compile_sequential(argv[source_pos],"output.txt");
		}else{
			printf("*************************************\n");
			printf("*        mode non-intéractif        *\n");
			printf("*           non séquentiel          *\n");
			printf("*************************************\n");
			compile(argv[source_pos],"output.txt");
		}
	}
	
	return 0;
}
