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
	char *res_copy = result;

	/*
	lis les caractères uns a uns et s'arrete si il trouve un retours a la ligne ou un début de commentaire
	*/
	while((!feof(f)) && (c != '\n') && (c != '#')){
		c = fgetc(f);
		if (c != '\n'){
			/* met la valeur dans resultat */
			*res_copy = c;
			/* avance dans la chaine de caractère */
			res_copy++;
		}
	}
	*res_copy = '\0';

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

/*
Description:
	Supprime les espaces au début et a la fin de la ligne
parametres:
	line - chaine de caractères
	size - taille chaine de caractère
return:
	void
*/
void supprimerEspaces(char *line){
	line[0]='z';
}