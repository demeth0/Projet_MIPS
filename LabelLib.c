#include "LabelLib.h"

ChainedElement *labels_list=NULL;

void addLabel(char *label, DWord addr){
	ChainedElement *toadd;
	ChainedElement *cursor=labels_list;

	toadd = malloc(sizeof(ChainedElement));
	toadd->addr[0]=addr[0];
	toadd->addr[1]=addr[1];
	toadd->addr[2]=addr[2];
	toadd->addr[3]=addr[3];

	toadd->name = malloc(sizeof(char)*strlen(label));
	strcpy(toadd->name,label);

	toadd->next = NULL;

	if(labels_list==NULL){
		labels_list = toadd;
	}else{
		while(cursor->next != NULL){
			cursor=cursor->next;
		}
		cursor->next = toadd;
	}
}

void clearLabelList(){
	ChainedElement *old;
	while(labels_list != NULL){
		free(labels_list->name);
		old = labels_list;
		labels_list = labels_list->next;
		free(old);
	}
}

void getAddress(char *label, DWord addr){
	int notfound = 1;
	ChainedElement *cursor=labels_list;

	while(cursor != NULL && notfound != 0){
		notfound = strcmp(label, cursor->name);
		if(notfound==0){
			addr[0]=cursor->addr[0];
			addr[1]=cursor->addr[1];
			addr[2]=cursor->addr[2];
			addr[3]=cursor->addr[3];
		}
		cursor = cursor->next;
	}
}