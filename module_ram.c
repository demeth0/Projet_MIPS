#include "module_ram.h"

/*
ecrit dans memoire ram avec adresse
*/
void writeRamADDR(Environment *simulation,DWord adresse,Byte value){
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

/*lit dans memoire avec adresse*/
Byte readRamADDR(Environment *simulation,DWord adresse){
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
	int incr = 0;

	for(index =0;index<MEM_SIZE;index++){
		for(incr =0;(index+incr)<MEM_SIZE && incr < 4;incr++){
			printf("%02x ",simulation->RAM[index+incr]);
			
		}
		for(incr =0;(index+incr)<MEM_SIZE && incr < 4;incr++){
			printf("%c ",simulation->RAM[index+incr]);
		}
		printf("\n");
		index = index+incr;
		
	}
	printf("\n");
}