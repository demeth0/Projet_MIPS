#include "module_ram.h"

/*
écrit dans memoire ram a adresse
*/
void writeRamADDR(Environment *simulation,Byte adresse[4],Byte value){
	int new_addr;
	/*si on est sur une architecture 32 bits*/
	if(sizeof(int) == 4){
		new_addr= adresse[3];
		new_addr+= (adresse[2])<<8;
		new_addr+= (adresse[1])<<16;
		new_addr+= (adresse[0])<<24;
	}
	else if(sizeof(int) == 2){
		new_addr= (adresse[1]);
		new_addr+= (adresse[0])<<8;
	}
	simulation->RAM[new_addr]=value;
}

/*lis dans memoire a adresse*/
Byte readRamADDR(Environment *simulation,Byte adresse[4]){
	/*on calcule l'index dans le tableau*/
	/*si on est sur une architecture 32 bits*/

	int new_addr;
	if(sizeof(int) == 4){
		new_addr= adresse[3];
		new_addr+= (adresse[2])<<8;
		new_addr+= (adresse[1])<<16;
		new_addr+= (adresse[0])<<24;
	}
	else if(sizeof(int) == 2){
		new_addr= (adresse[1]);
		new_addr+= (adresse[0])<<8;
	}
	return simulation->RAM[new_addr];
}

/*affiche toute la ram*/
void afficher_ram(Environment *simulation){

	int index =0;

	for(index =0;index<MEM_SIZE;index++){
		printf("%02x ",simulation->RAM[index]);
	}
}