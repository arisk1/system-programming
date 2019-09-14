#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "dio.h"


int userCommand(char* input);
unsigned long
hash1(unsigned char *str,int hashTableEntries);
void Insert1(char* line,hash* hashTable,int bucketSize,int hashTableEntries);
void Insert2(char* line,hash* hashTable,int bucketSize,int hashTableEntries);
void Print(hash *hashTable,int hashTableEntries,int flag);
void Delete(hash *hashTable,char *line,int hashTableEntries);
int convertTime(char *time1);
int convertYear(char *year);
void FindCaller(hash *hashTable,char *line1,char* line2,int hashTableEntries);
void LookUp(hash *hashTable,char *line1,char* line2,int hashTableEntries);
char* countryID(char *x);
void TopDest(hash *hashTable,char *line,int hashTableEntries,int bucketSize);
void Dump(hash *hashTable,int hashTableEntries,FILE* ff,int flag);
void Bye(hash* hashTable1,hash *hashTable2,int hashTableEntries1,int hashTableEntries);
void Indist(hash *hashTable,char *line1,int hashTableEntries);
