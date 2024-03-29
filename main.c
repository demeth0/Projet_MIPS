#include <stdio.h>
#include "module_compilateur.h"
#include "module_emulateur.h"
#include "module_io.h"

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
				printInst(sim,instr);
				simulate(&instr, sim);
			}
		}

	}
}

int main(int argc, char const *argv[])
{
	Environment simulation;
	Program prog;
	FILE *output = NULL;
	FILE *simu = NULL;
	
	initSimulation(&simulation);

	if(argc==1){
		printf("*************************************\n");
		printf("*         mode interpreteur         *\n");
		printf("*************************************\n");
		interpreteur(&simulation);
	}else if(argc == 3){
		if(strcmp(argv[2],"-pas")==0){
			printf("*************************************\n");
			printf("*        mode non-interactif        *\n");
			printf("*             sequentiel            *\n");
			printf("*************************************\n");
			if(compile(argv[1],prog)){
				/*simule le programme pas a pas*/
				simulateProgram(prog,&simulation,1);
			}
		}else{
			printf("paramètres manquants\n");
		}
		
		/*simulateFile(argv[source_pos],&simulation,seq);*/
	}else if(argc == 4){
		/*ouvrir deux fichiers de sortie ecrire les valeurs demandes*/
		if(compile(argv[1],prog)){
			output = fopen(argv[2],"w+");
			simu = fopen(argv[3],"w+");

			if (output != NULL && simu != NULL){
				simulateProgram(prog,&simulation,0);
				ecrireProgram(output, prog);
				ecrireRegistres(simu, &simulation);
			}
			if(output!=NULL)
				fclose(output);
			if (simu != NULL)
				fclose(simu);

		}
	}else{
		printf("commande inconue\n");
	}
	
	return 0;
}
