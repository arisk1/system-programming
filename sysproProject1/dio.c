
#include "dio.h"

void CreateHT(int HTentries,hash* hashTable)
{
	int i;
	for(i=0;i<HTentries;i++)
	{
		hashTable[i].head=NULL;
	}	
}


		
