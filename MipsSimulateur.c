#include "MipsSimulateur.h"


/*permet de recopier dans sortie le contenue de l'entrée*/
void copy_Byte4(Byte entree[4],Byte sortie[4]){
	sortie[0] = entree[0];
	sortie[1] = entree[1];
	sortie[2] = entree[2];
	sortie[3] = entree[3];
}


/*
struct SimEnv {
	Byte RAM[MEM_SIZE];
	Byte registers[32][4];
	Byte PC[4];
	Byte HI[4];
	Byte LO[4];
};
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

