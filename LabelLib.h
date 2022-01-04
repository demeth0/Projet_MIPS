#ifndef LABEL_LIB_DEFINE
#define LABEL_LIB_DEFINE

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "mips_structures.h"

struct chainedList{
	char *name;
	char addr[4];
	struct chainedList *next;
};

typedef struct chainedList ChainedElement;

void addLabel(char *label, DWord addr);
void clearLabelList();
void getAddress(char *label,DWord addr);


#endif