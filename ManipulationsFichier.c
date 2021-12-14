#include "ManipulationsFichier.h"



void readInstruction(FILE *f, char *result){
	char c = 'a';
	char *res_copy = result;
	int loop=1;
	/*
	lis les caractères uns à uns et s'arrête si il trouve un retour à la ligne ou un début de commentaire
	*/
	while(loop){
		c = fgetc(f);

		loop=(!feof(f)) && (c != '\n') && (c != '#');

		if (loop){
			/* met la valeur dans resultat */
			*res_copy = c;
			/* avance dans la chaine de caractères */
			res_copy++;
		}
	}
	*res_copy = '\0';

	/*
	dans le cas ou il trouve un commentaire on continue de lire la ligne jusqu'au bout pour avancer le curseur jusqu'à la fin de la ligne
	*/
	while((!feof(f)) && (c != '\n')){
		c=fgetc(f);
	}
}



void writeHexInstructionToFile(FILE *f, Instruction inst){
	fprintf(f, "%02x%02x%02x%02x\n", inst.code[0],inst.code[1],inst.code[2],inst.code[3]);
}

void supprimerEspaces(char *line){
	line[0]='z';
}