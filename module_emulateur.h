#ifndef MIPS_SIMULATEUR_DEFINE
#define MIPS_SIMULATEUR_DEFINE

#include <stdio.h>
#include <stdlib.h>
#include "ByteUtils.h"
#include "operation_code_defines.h"
#include "ByteUtils.h"
#include "mips_structures.h"
#include "module_ram.h"
#include "module_compilateur.h"
#include "ManipulationsFichier.h"

/*
Description:
	initialise la simulation
erreurs:
	si simulation nul
*/
void initSimulation(Environment *simulation);
	
/*
Description:
	prend une instruction en paramètre et l'execute dans l'environement.
erreurs:
	si un des paramètres nul
*/
void simulate(Instruction *instruction, Environment *simulation);

/*
Description:
	simule un programme dans l'environement, mode pas a pas ou non 	
erreurs:
	si fichier non valides ou simulation nul
*/
void simulateProgram(Program prog,Environment *simulation,int sequential);

/*
Description:
	affiche l'instruction sous le format : 
	PC Hex inst
erreurs:
	si un des paramètres nul
*/
void printInst(Environment *sim, Instruction inst);

/*
Description:
	afficher les registres d'une simulation
erreurs:
	si sim nul
*/
void afficher_registres(Environment *sim);

#endif