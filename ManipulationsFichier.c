#include "ManipulationsFichier.h"

/*
Description: 
	va metre dans resultat l'instruction (texte) sur la ligne suivante dans le fichier en paramètre en enlevant les commentaires.
Paramètre:
	f      - le fichier dans lequel se trouve les instructions
	result - la chaine de caractère qui va stocker l'instruction lue
return:
	void
*/
void readInstruction(FILE *f, char *result){
	char c = 'a';
	
	/*
	lis les caractères uns a uns et s'arrete si il trouve un retours a la ligne ou un début de commentaire
	*/
	while((!feof(f)) && (c != '\n') && (c != '#')){
		c = fgetc(f);

		/* met la valeur dans resultat */
		*result = c;
		/* avance dans la chaine de caractère */
		result++;
	}

	/*
	dans le cas ou il trouve un commentaire on continue de lire la ligne jusqu'au bout pour avancer le curseur jusqu'a la fin de la ligne
	*/
	while((!feof(f)) && (c != '\n')){
		fgetc(f);
	}
}


/*
Description:
	écrit dans le fichier l'instruction en valeur hexadécimale formattée sur 8 char
parametres:
	f - le fichier ou écrire la donnée
	inst - l'instruction hexa a écrire dans le fichier
*/
void writeHexInstructionToFile(FILE *f, Instruction inst){
	fprintf(f, "%02x%02x%02x%02x\n", inst.code[0],inst.code[1],inst.code[2],inst.code[3]);
}