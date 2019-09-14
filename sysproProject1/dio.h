#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cdr
{
	char* uniqID;
	char* originatorNumber;
	char* destinatorNumber;
	char* date;
	char* inittime;
	char* duration;
	char* type;
	char* tarrif;
	char* faultCondition;
}cdr;

typedef struct lightPink 
{
	cdr *pinkEntry;
	struct lightPink *ptr2;
		int cntr; 
}lightPink;

typedef struct bucketNode
{
	char* number;
	lightPink *ptr0;
}bucketNode;

typedef struct bucketList
{
	bucketNode *bcks;
	struct bucketList *ptr1;
	int counter;
}bucketList;

typedef struct hash //list of bucketLists
{
	bucketList *head;
}hash;

void CreateHT(int HTentries,hash* hashTable);
/*---------------------------------------------*/










