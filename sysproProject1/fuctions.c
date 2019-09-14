
#include "fuctions.h"


int userCommand(char* input)
{
	if(strstr(input,"insert")!=NULL)
		return 1;
	else if(strstr(input,"delete")!=NULL)
		return 2;
	else if(strstr(input,"find")!=NULL)
		return 3;
	else if(strstr(input,"lookup")!=NULL)
		return 4;
	else if(strstr(input,"indist")!=NULL)
		return 5;
	else if(strstr(input,"topdest")!=NULL)
		return 6;
	else if(strstr(input,"top")!=NULL)
		return 7;
	else if(strstr(input,"bye")!=NULL)
		return 8;
	else if(strstr(input,"print hashtable1")!=NULL)
		return 9;
	else if(strstr(input,"print hashtable2")!=NULL)
		return 10; 
	else if(strstr(input,"dump hashtable1")!=NULL)
		return 11;
	else if(strstr(input,"dump hashtable2")!=NULL)
		return 12;
	else 
		return -1;
}

unsigned long
hash1(unsigned char *str,int hashTableEntries)
{
    unsigned long key;
    unsigned long hash1 = 5381;
    int c;

    while (c = *str++)
        hash1 = ((hash1 << 5) + hash1) + c; /* hash1 * 33 + c */

    key = hash1;
    key = key % hashTableEntries;
    return key;
}

void Insert1(char* line,hash* hashTable,int bucketSize,int hashTableEntries)
{
	printf("insert >>>>\n");
	//dimiourgia ena CDR struct 
	cdr* cdr1;
	cdr1 = malloc(sizeof(cdr));
	cdr1->uniqID=malloc(10*sizeof(char));
	cdr1->originatorNumber=malloc(14*sizeof(char));	
	cdr1->destinatorNumber=malloc(14*sizeof(char));
	cdr1->date=malloc(8*sizeof(char));
	cdr1->inittime=malloc(5*sizeof(char));
	cdr1->duration=malloc(4*sizeof(char));
	cdr1->type=malloc(sizeof(char));
	cdr1->tarrif=malloc(sizeof(char));
	cdr1->faultCondition=malloc(4*sizeof(char));
	//bazoume ta dedomena tou line sto struct CDR
	int i=0;
	char *cdrEntry;
	cdrEntry = strtok(line," "); //first word
	while((cdrEntry = strtok(NULL,";")) != NULL)
	{
		if(i==0){strcpy(cdr1->uniqID,cdrEntry); i++;}
		else if(i==1){strcpy(cdr1->originatorNumber,cdrEntry); i++;}
		else if(i==2){strcpy(cdr1->destinatorNumber,cdrEntry); i++;}
		else if(i==3){strcpy(cdr1->date,cdrEntry); i++;}
		else if(i==4){strcpy(cdr1->inittime,cdrEntry); i++;}
		else if(i==5){strcpy(cdr1->duration,cdrEntry); i++;}
		else if(i==6){strcpy(cdr1->type,cdrEntry); i++;}
		else if(i==7){strcpy(cdr1->tarrif,cdrEntry); i++;}
		else if(i==8){strcpy(cdr1->faultCondition,cdrEntry); i++;}
	}	
	if(cdr1->uniqID == NULL){printf("INSERT ERROR\n");exit(-1);}
	else if(cdr1->originatorNumber==NULL){printf("INSERT ERROR \n");exit(-1);}
	else if(cdr1->destinatorNumber==NULL){printf("INSERT ERROR \n");exit(-1);}
	else if(cdr1->date==NULL){printf("INSERT ERROR \n");exit(-1);}
	else if(cdr1->inittime==NULL){printf("INSERT ERROR \n");exit(-1);}
	else if(cdr1->duration==NULL){printf("INSERT ERROR \n");exit(-1);}
	else if(cdr1->type==NULL){printf("INSERT ERROR \n");exit(-1);}
	else if(cdr1->tarrif==NULL){printf("INSERT ERROR \n");exit(-1);}
	else if(cdr1->faultCondition==NULL){printf("INSERT ERROR \n");exit(-1);}
	int maxSize;
	maxSize = bucketSize/(sizeof(char)*14);
	//efarmozoume hashfunction 
	
	int key;
	key=hash1(cdr1->originatorNumber,hashTableEntries);
	if(hashTable[key].head==NULL)
	{
		//dimiourgia bucket 
		bucketList* bucket;
		bucket=malloc(sizeof(bucketList));
		bucket->bcks=(bucketNode *)malloc(maxSize*sizeof(bucketNode));
		int j=0;
		for(j=0;j<maxSize;j++)	
		{
			bucket->bcks[j].number= malloc(14 * sizeof(char));
			bucket->bcks[j].ptr0 = NULL;
		}
		bucket->counter=0;
		bucket->ptr1 = NULL;
		// kai to sindeoume me to hashTable 
		hashTable[key].head=bucket;
		//kia kanoume insert 
		strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].number,cdr1->originatorNumber);		
		//printf(" ----- %s \n",hashTable[key].head->bcks[hashTable[key].head->counter].number);
		// dimiourgoume kai bazoume kai ta ipoloipa cdr entreis sto pink 
		lightPink *lp;
		lp =(lightPink*) malloc(sizeof(lightPink));
		lp->pinkEntry =(cdr *)malloc(maxSize*sizeof(cdr));
		int z;
		for(z=0;z<maxSize;z++)
		{
			lp->pinkEntry[z].uniqID=malloc(10*sizeof(char));
			lp->pinkEntry[z].originatorNumber=malloc(14*sizeof(char));	
			lp->pinkEntry[z].destinatorNumber=malloc(14*sizeof(char));
			lp->pinkEntry[z].date=malloc(8*sizeof(char));
			lp->pinkEntry[z].inittime=malloc(5*sizeof(char));
			lp->pinkEntry[z].duration=malloc(4*sizeof(char));
			lp->pinkEntry[z].type=malloc(sizeof(char));
			lp->pinkEntry[z].tarrif=malloc(sizeof(char));
			lp->pinkEntry[z].faultCondition=malloc(4*sizeof(char));
		}
		lp->cntr=0;
		lp->ptr2=NULL;
		//to sindeoume sto bucket
		hashTable[key].head->bcks[hashTable[key].head->counter].ptr0=lp;
		//bazoume ta cdr sto lightpink
		strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].uniqID,cdr1->uniqID);
		strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].originatorNumber,cdr1->originatorNumber);
		strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].destinatorNumber,cdr1->destinatorNumber);
		strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].date,cdr1->date);
		strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].inittime,cdr1->inittime);
		strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].duration,cdr1->duration);
		strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].type,cdr1->type);
		strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].tarrif,cdr1->tarrif);
		strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].faultCondition,cdr1->faultCondition);
		//printf(" |u|n|i|q|I|D|:)| %s \n",hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].uniqID);
		hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr++;
		hashTable[key].head->counter++;

	} 
	else	
 	{
		//an iparxei idi bucket sto hashTable[key].head
		//elenxoume an iparxei o caller se kapoio bucket		
		bucketList* assistant;
		assistant = malloc(sizeof(bucketList));
		assistant=hashTable[key].head;
		int findPos=0;
		int findFlag = 0;
		while(assistant != NULL)
		{	
			for(int o=0;o<assistant->counter;o++)
			{
				if(strcmp(cdr1->originatorNumber,assistant->bcks[o].number)==0)
				{
					findFlag=1; //brethike
					findPos = o;
					break;
				}	
			}if(findFlag==1){break;}
			assistant = hashTable[key].head->ptr1;
		} 
		//elenxoume an brethike o caller se kapoio bucket
		if(findFlag == 1 ) //an brethike,bazoume ta cdr sto lightPink
		{
			//prota elenxoume an eiani gemato to lightpink
			if(assistant->bcks[findPos].ptr0->cntr < maxSize)
			{ 
				// an den einai vazoume ta cdr sto lightpink
				strcpy(assistant->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].uniqID,cdr1->uniqID);
				strcpy(assistant->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].originatorNumber,cdr1->originatorNumber);
				strcpy(assistant->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].destinatorNumber,cdr1->destinatorNumber);
				strcpy(assistant->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].date,cdr1->date);
				strcpy(assistant->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].inittime,cdr1->inittime);
				strcpy(assistant->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].duration,cdr1->duration);
				strcpy(assistant->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].type,cdr1->type);
				strcpy(assistant->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].tarrif,cdr1->tarrif);
				strcpy(assistant->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].faultCondition,cdr1->faultCondition);
				assistant->bcks[findPos].ptr0->cntr++;
			}
			else
			{
				//an einai gematos ftiaxnoume ena neo lightpink
				lightPink *lp;
				lp = (lightPink*)malloc(sizeof(lightPink));
				lp->pinkEntry =(cdr *)malloc(maxSize*sizeof(cdr));
				int z;
				for(z=0;z<maxSize;z++)
				{
					lp->pinkEntry[z].uniqID=malloc(10*sizeof(char));
					lp->pinkEntry[z].originatorNumber=malloc(14*sizeof(char));	
					lp->pinkEntry[z].destinatorNumber=malloc(14*sizeof(char));
					lp->pinkEntry[z].date=malloc(8*sizeof(char));
					lp->pinkEntry[z].inittime=malloc(5*sizeof(char));
					lp->pinkEntry[z].duration=malloc(4*sizeof(char));
					lp->pinkEntry[z].type=malloc(sizeof(char));
					lp->pinkEntry[z].tarrif=malloc(sizeof(char));
					lp->pinkEntry[z].faultCondition=malloc(4*sizeof(char));
				}
				lp->cntr=0;
				lp->ptr2=NULL;
				//to sindeoume sto palio
				hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->ptr2=lp;
				//bazoume to cdr sto neo lightpink
				strcpy(hashTable[key].head->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].uniqID,cdr1->uniqID);
				strcpy(hashTable[key].head->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].originatorNumber,cdr1->originatorNumber);
				strcpy(hashTable[key].head->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].destinatorNumber,cdr1->destinatorNumber);
				strcpy(hashTable[key].head->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].date,cdr1->date);
				strcpy(hashTable[key].head->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].inittime,cdr1->inittime);
				strcpy(hashTable[key].head->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].duration,cdr1->duration);
				strcpy(hashTable[key].head->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].type,cdr1->type);
				strcpy(hashTable[key].head->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].tarrif,cdr1->tarrif);
				strcpy(hashTable[key].head->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].faultCondition,cdr1->faultCondition);
				hashTable[key].head->bcks[findPos].ptr0->cntr++;			
			}  
		}
		else //an den vrethike o caller
		{	  
			//elenxoume an einai gemato
			if(hashTable[key].head->counter < maxSize)
			{
				//an den einai vazoume to cdr1->originatorNumber sto bucket
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].number,cdr1->originatorNumber);	
				// dimiourgoume kai bazoume kai ta ipoloipa cdr entreis sto pink
				lightPink *lp;
				lp =(lightPink*) malloc(sizeof(lightPink));
				lp->pinkEntry =(cdr *)malloc(maxSize*sizeof(cdr));
				int z;
				for(z=0;z<maxSize;z++)
				{
					lp->pinkEntry[z].uniqID=malloc(10*sizeof(char));
					lp->pinkEntry[z].originatorNumber=malloc(14*sizeof(char));	
					lp->pinkEntry[z].destinatorNumber=malloc(14*sizeof(char));
					lp->pinkEntry[z].date=malloc(8*sizeof(char));
					lp->pinkEntry[z].inittime=malloc(5*sizeof(char));
					lp->pinkEntry[z].duration=malloc(4*sizeof(char));
					lp->pinkEntry[z].type=malloc(sizeof(char));
					lp->pinkEntry[z].tarrif=malloc(sizeof(char));
					lp->pinkEntry[z].faultCondition=malloc(4*sizeof(char));
				}
				lp->cntr=0;
				lp->ptr2=NULL;
				//to sindeoume sto bucket
				hashTable[key].head->bcks[hashTable[key].head->counter].ptr0=lp;
				//bazoume ta cdr sto lightpink
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].uniqID,cdr1->uniqID);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].originatorNumber,cdr1->originatorNumber);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].destinatorNumber,cdr1->destinatorNumber);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].date,cdr1->date);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].inittime,cdr1->inittime);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].duration,cdr1->duration);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].type,cdr1->type);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].tarrif,cdr1->tarrif);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].faultCondition,cdr1->faultCondition);
				hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr++;
				hashTable[key].head->counter++;
		
			}
			else //an exei gemisei o bucket ftiakse kainourgio 
			{
				bucketList* bucket;
				bucket=malloc(sizeof(bucketList));
				bucket->bcks=(bucketNode *)malloc(maxSize*sizeof(bucketNode));
				int j=0;
				for(j=0;j<maxSize;j++)	
				{
					bucket->bcks[j].number= malloc(14 * sizeof(char));
					bucket->bcks[j].ptr0 = NULL;
				}
				bucket->counter=0;
				bucket->ptr1 = NULL;
				//enonoume auto to bucket me to palio
				bucket->ptr1=hashTable[key].head;
				//kai knoume overwrite to head na deixnei sto kainourgio bucket
				hashTable[key].head = bucket;
				//kai kanoue insert to cdr1->originatorNumber sto kainourgio bucket
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].number,cdr1->originatorNumber);	
				// dimiourgoume kai bazoume kai ta ipoloipa cdr entreis sto pink 
				lightPink *lp;
				lp =(lightPink*) malloc(sizeof(lightPink));
				lp->pinkEntry =(cdr *)malloc(maxSize*sizeof(cdr));
				int z;
				for(z=0;z<maxSize;z++)
				{
					lp->pinkEntry[z].uniqID=malloc(10*sizeof(char));
					lp->pinkEntry[z].originatorNumber=malloc(14*sizeof(char));	
					lp->pinkEntry[z].destinatorNumber=malloc(14*sizeof(char));
					lp->pinkEntry[z].date=malloc(8*sizeof(char));
					lp->pinkEntry[z].inittime=malloc(5*sizeof(char));
					lp->pinkEntry[z].duration=malloc(4*sizeof(char));
					lp->pinkEntry[z].type=malloc(sizeof(char));
					lp->pinkEntry[z].tarrif=malloc(sizeof(char));
					lp->pinkEntry[z].faultCondition=malloc(4*sizeof(char));
				}
				lp->cntr=0;
				lp->ptr2=NULL;
				//to sindeoume sto bucket
				hashTable[key].head->bcks[hashTable[key].head->counter].ptr0=lp;
				//bazoume ta cdr sto lightpink
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].uniqID,cdr1->uniqID);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].originatorNumber,cdr1->originatorNumber);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].destinatorNumber,cdr1->destinatorNumber);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].date,cdr1->date);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].inittime,cdr1->inittime);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].duration,cdr1->duration);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].type,cdr1->type);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].tarrif,cdr1->tarrif);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].faultCondition,cdr1->faultCondition);
				hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr++;	
				hashTable[key].head->counter++;
			}		
		}
	}
	free(cdr1->uniqID);
	free(cdr1->originatorNumber);
	free(cdr1->destinatorNumber);
	free(cdr1->date);
	free(cdr1->inittime);
	free(cdr1->tarrif);
	free(cdr1->type);
	free(cdr1->faultCondition);
	free(cdr1->duration);
	free(cdr1);
}

void Insert2(char* line,hash* hashTable,int bucketSize,int hashTableEntries)
{
	//dimiourgia ena CDR struct 
	cdr* cdr1;
	cdr1 = malloc(sizeof(cdr));
	cdr1->uniqID=malloc(10*sizeof(char));
	cdr1->originatorNumber=malloc(14*sizeof(char));	
	cdr1->destinatorNumber=malloc(14*sizeof(char));
	cdr1->date=malloc(8*sizeof(char));
	cdr1->inittime=malloc(5*sizeof(char));
	cdr1->duration=malloc(4*sizeof(char));
	cdr1->type=malloc(sizeof(char));
	cdr1->tarrif=malloc(sizeof(char));
	cdr1->faultCondition=malloc(4*sizeof(char));
	//bazoume ta dedomena tou line sto struct CDR
	int i=0;
	char *cdrEntry;
	cdrEntry = strtok(line," "); //first word
	while((cdrEntry = strtok(NULL,";")) != NULL)
	{
		if(i==0){strcpy(cdr1->uniqID,cdrEntry); i++;}
		else if(i==1){strcpy(cdr1->originatorNumber,cdrEntry); i++;}
		else if(i==2){strcpy(cdr1->destinatorNumber,cdrEntry); i++;}
		else if(i==3){strcpy(cdr1->date,cdrEntry); i++;}
		else if(i==4){strcpy(cdr1->inittime,cdrEntry); i++;}
		else if(i==5){strcpy(cdr1->duration,cdrEntry); i++;}
		else if(i==6){strcpy(cdr1->type,cdrEntry); i++;}
		else if(i==7){strcpy(cdr1->tarrif,cdrEntry); i++;}
		else if(i==8){strcpy(cdr1->faultCondition,cdrEntry); i++;}
	}	
	if(cdr1->uniqID == NULL){printf("INSERT ERROR\n");exit(-1);}
	else if(cdr1->originatorNumber==NULL){printf("INSERT ERROR \n");exit(-1);}
	else if(cdr1->destinatorNumber==NULL){printf("INSERT ERROR \n");exit(-1);}
	else if(cdr1->date==NULL){printf("INSERT ERROR \n");exit(-1);}
	else if(cdr1->inittime==NULL){printf("INSERT ERROR \n");exit(-1);}
	else if(cdr1->duration==NULL){printf("INSERT ERROR \n");exit(-1);}
	else if(cdr1->type==NULL){printf("INSERT ERROR \n");exit(-1);}
	else if(cdr1->tarrif==NULL){printf("INSERT ERROR \n");exit(-1);}
	else if(cdr1->faultCondition==NULL){printf("INSERT ERROR \n");exit(-1);} 
	int maxSize;
	maxSize = bucketSize/(sizeof(char)*14);
	//efarmozoume hashfunction 
	
	int key;
	key=hash1(cdr1->destinatorNumber,hashTableEntries);
	if(hashTable[key].head==NULL)
	{
		//dimiourgia bucket 
		bucketList* bucket;
		bucket=malloc(sizeof(bucketList));
		bucket->bcks=(bucketNode *)malloc(maxSize*sizeof(bucketNode));
		int j=0;
		for(j=0;j<maxSize;j++)	
		{
			bucket->bcks[j].number= malloc(14 * sizeof(char));
			bucket->bcks[j].ptr0 = NULL;
		}
		bucket->counter=0;
		bucket->ptr1 = NULL;
		// kai to sindeoume me to hashTable 
		hashTable[key].head=bucket;
		//kia kanoume insert 
		strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].number,cdr1->destinatorNumber);		
		//printf(" ----- %s \n",hashTable[key].head->bcks[hashTable[key].head->counter].number);
		// dimiourgoume kai bazoume kai ta ipoloipa cdr entreis sto pink 
		lightPink *lp;
		lp = malloc(sizeof(lightPink));
		lp->pinkEntry =(cdr *)malloc(maxSize*sizeof(cdr));
		int z;
		for(z=0;z<maxSize;z++)
		{
			lp->pinkEntry[z].uniqID=malloc(10*sizeof(char));
			lp->pinkEntry[z].originatorNumber=malloc(14*sizeof(char));	
			lp->pinkEntry[z].destinatorNumber=malloc(14*sizeof(char));
			lp->pinkEntry[z].date=malloc(8*sizeof(char));
			lp->pinkEntry[z].inittime=malloc(5*sizeof(char));
			lp->pinkEntry[z].duration=malloc(4*sizeof(char));
			lp->pinkEntry[z].type=malloc(sizeof(char));
			lp->pinkEntry[z].tarrif=malloc(sizeof(char));
			lp->pinkEntry[z].faultCondition=malloc(4*sizeof(char));
		}
		lp->cntr=0;
		lp->ptr2=NULL;
		//to sindeoume sto bucket
		hashTable[key].head->bcks[hashTable[key].head->counter].ptr0=lp;
		//bazoume ta cdr sto lightpink
		strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].uniqID,cdr1->uniqID);
		strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].originatorNumber,cdr1->originatorNumber);
		strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].destinatorNumber,cdr1->destinatorNumber);
		strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].date,cdr1->date);
		strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].inittime,cdr1->inittime);
		strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].duration,cdr1->duration);
		strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].type,cdr1->type);
		strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].tarrif,cdr1->tarrif);
		strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].faultCondition,cdr1->faultCondition);
		//printf(" |u|n|i|q|I|D|:)| %s \n",hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].uniqID);
		hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr++;
		hashTable[key].head->counter++;

	} 
	else	
 	{
		//an iparxei idi bucket sto hashTable[key].head
		//elenxoume an iparxei o caller se kapoio bucket		
		bucketList* assistant;
		assistant = malloc(sizeof(bucketList));
		assistant=hashTable[key].head;
		int findPos=0;
		int findFlag = 0;
		while(assistant != NULL)
		{	
			for(int o=0;o<assistant->counter;o++)
			{
				if(strcmp(cdr1->destinatorNumber,assistant->bcks[o].number)==0)
				{
					findFlag=1; //brethike
					findPos = o;
					break;
				}	
			}if(findFlag==1){break;}
			assistant = hashTable[key].head->ptr1;
		} 
		//elenxoume an brethike o caller se kapoio bucket
		if(findFlag == 1 ) //an brethike,bazoume ta cdr sto lightPink
		{
			//prota elenxoume an eiani gemato to lightpink
			if(assistant->bcks[findPos].ptr0->cntr < maxSize)
			{ 
				// an den einai vazoume ta cdr sto lightpink
				strcpy(assistant->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].uniqID,cdr1->uniqID);
				strcpy(assistant->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].originatorNumber,cdr1->originatorNumber);
				strcpy(assistant->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].destinatorNumber,cdr1->destinatorNumber);
				strcpy(assistant->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].date,cdr1->date);
				strcpy(assistant->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].inittime,cdr1->inittime);
				strcpy(assistant->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].duration,cdr1->duration);
				strcpy(assistant->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].type,cdr1->type);
				strcpy(assistant->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].tarrif,cdr1->tarrif);
				strcpy(assistant->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].faultCondition,cdr1->faultCondition);
				assistant->bcks[findPos].ptr0->cntr++;
			}
			else
			{
				//an einai gematos ftiaxnoume ena neo lightpink
				lightPink *lp;
				lp = malloc(sizeof(lightPink));
				lp->pinkEntry =(cdr *)malloc(maxSize*sizeof(cdr));
				int z;
				for(z=0;z<maxSize;z++)
				{
					lp->pinkEntry[z].uniqID=malloc(10*sizeof(char));
					lp->pinkEntry[z].originatorNumber=malloc(14*sizeof(char));	
					lp->pinkEntry[z].destinatorNumber=malloc(14*sizeof(char));
					lp->pinkEntry[z].date=malloc(8*sizeof(char));
					lp->pinkEntry[z].inittime=malloc(5*sizeof(char));
					lp->pinkEntry[z].duration=malloc(4*sizeof(char));
					lp->pinkEntry[z].type=malloc(sizeof(char));
					lp->pinkEntry[z].tarrif=malloc(sizeof(char));
					lp->pinkEntry[z].faultCondition=malloc(4*sizeof(char));
				}
				lp->cntr=0;
				lp->ptr2=NULL;
				//to sindeoume sto palio
				hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->ptr2=lp;
				//bazoume to cdr sto neo lightpink
				strcpy(hashTable[key].head->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].uniqID,cdr1->uniqID);
				strcpy(hashTable[key].head->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].originatorNumber,cdr1->originatorNumber);
				strcpy(hashTable[key].head->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].destinatorNumber,cdr1->destinatorNumber);
				strcpy(hashTable[key].head->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].date,cdr1->date);
				strcpy(hashTable[key].head->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].inittime,cdr1->inittime);
				strcpy(hashTable[key].head->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].duration,cdr1->duration);
				strcpy(hashTable[key].head->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].type,cdr1->type);
				strcpy(hashTable[key].head->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].tarrif,cdr1->tarrif);
				strcpy(hashTable[key].head->bcks[findPos].ptr0->pinkEntry[hashTable[key].head->bcks[findPos].ptr0->cntr].faultCondition,cdr1->faultCondition);
				hashTable[key].head->bcks[findPos].ptr0->cntr++;			
			}  
		}
		else //an den vrethike o caller
		{	  
			//elenxoume an einai gemato
			if(hashTable[key].head->counter < maxSize)
			{
				//an den einai vazoume to cdr1->destinatorNUmber sto bucket
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].number,cdr1->destinatorNumber);	
				// dimiourgoume kai bazoume kai ta ipoloipa cdr entreis sto pink
				lightPink *lp;
				lp = malloc(sizeof(lightPink));
				lp->pinkEntry =(cdr *)malloc(maxSize*sizeof(cdr));
				int z;
				for(z=0;z<maxSize;z++)
				{
					lp->pinkEntry[z].uniqID=malloc(10*sizeof(char));
					lp->pinkEntry[z].originatorNumber=malloc(14*sizeof(char));	
					lp->pinkEntry[z].destinatorNumber=malloc(14*sizeof(char));
					lp->pinkEntry[z].date=malloc(8*sizeof(char));
					lp->pinkEntry[z].inittime=malloc(5*sizeof(char));
					lp->pinkEntry[z].duration=malloc(4*sizeof(char));
					lp->pinkEntry[z].type=malloc(sizeof(char));
					lp->pinkEntry[z].tarrif=malloc(sizeof(char));
					lp->pinkEntry[z].faultCondition=malloc(4*sizeof(char));
				}
				lp->cntr=0;
				lp->ptr2=NULL;
				//to sindeoume sto bucket
				hashTable[key].head->bcks[hashTable[key].head->counter].ptr0=lp;
				//bazoume ta cdr sto lightpink
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].uniqID,cdr1->uniqID);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].originatorNumber,cdr1->originatorNumber);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].destinatorNumber,cdr1->destinatorNumber);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].date,cdr1->date);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].inittime,cdr1->inittime);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].duration,cdr1->duration);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].type,cdr1->type);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].tarrif,cdr1->tarrif);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].faultCondition,cdr1->faultCondition);
				hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr++;
				hashTable[key].head->counter++;
		
			}
			else //an exei gemisei o bucket ftiakse kainourgio 
			{
				bucketList* bucket;
				bucket=malloc(sizeof(bucketList));
				bucket->bcks=(bucketNode *)malloc(maxSize*sizeof(bucketNode));
				int j=0;
				for(j=0;j<maxSize;j++)	
				{
					bucket->bcks[j].number= malloc(14 * sizeof(char));
					bucket->bcks[j].ptr0 = NULL;
				}
				bucket->counter=0;
				bucket->ptr1 = NULL;
				//enonoume auto to bucket me to palio
				bucket->ptr1=hashTable[key].head;
				//kai knoume overwrite to head na deixnei sto kainourgio bucket
				hashTable[key].head = bucket;
				//kai kanoue insert to cdr1->destinatorNumber sto kainourgio bucket
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].number,cdr1->destinatorNumber);	
				// dimiourgoume kai bazoume kai ta ipoloipa cdr entreis sto pink 
				lightPink *lp;
				lp = malloc(sizeof(lightPink));
				lp->pinkEntry =(cdr *)malloc(maxSize*sizeof(cdr));
				int z;
				for(z=0;z<maxSize;z++)
				{
					lp->pinkEntry[z].uniqID=malloc(10*sizeof(char));
					lp->pinkEntry[z].originatorNumber=malloc(14*sizeof(char));	
					lp->pinkEntry[z].destinatorNumber=malloc(14*sizeof(char));
					lp->pinkEntry[z].date=malloc(8*sizeof(char));
					lp->pinkEntry[z].inittime=malloc(5*sizeof(char));
					lp->pinkEntry[z].duration=malloc(4*sizeof(char));
					lp->pinkEntry[z].type=malloc(sizeof(char));
					lp->pinkEntry[z].tarrif=malloc(sizeof(char));
					lp->pinkEntry[z].faultCondition=malloc(4*sizeof(char));
				}
				lp->cntr=0;
				lp->ptr2=NULL;
				//to sindeoume sto bucket
				hashTable[key].head->bcks[hashTable[key].head->counter].ptr0=lp;
				//bazoume ta cdr sto lightpink
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].uniqID,cdr1->uniqID);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].originatorNumber,cdr1->originatorNumber);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].destinatorNumber,cdr1->destinatorNumber);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].date,cdr1->date);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].inittime,cdr1->inittime);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].duration,cdr1->duration);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].type,cdr1->type);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].tarrif,cdr1->tarrif);
				strcpy(hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->pinkEntry[hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr].faultCondition,cdr1->faultCondition);
				hashTable[key].head->bcks[hashTable[key].head->counter].ptr0->cntr++;	
				hashTable[key].head->counter++;
			}		
		}
	}
	free(cdr1->uniqID);
	free(cdr1->originatorNumber);
	free(cdr1->destinatorNumber);
	free(cdr1->date);
	free(cdr1->inittime);
	free(cdr1->tarrif);
	free(cdr1->type);
	free(cdr1->faultCondition);
	free(cdr1->duration);
	free(cdr1);
}

void Print(hash *hashTable,int hashTableEntries,int flag)
{
	printf("printf >>>> \n");
	for(int i=0;i<hashTableEntries;i++)
	{
		if(flag==1){if(i==0){printf("-------------------------HT1-------------------------\n");}}
		else if(flag==2){if(i==0){printf("-------------------------HT2-------------------------\n");}}
		printf("HT[%d]",i);
		if(hashTable[i].head != NULL)
		{
			if(flag==1)
			{
				for(int y=0;y<hashTable[i].head->counter;y++)
				{
					printf("->BN[%d]",y);
					printf("|%s|",hashTable[i].head->bcks[y].number);	
					for(int z=0;z<hashTable[i].head->bcks[y].ptr0->cntr;z++)
					{
						printf("\n");
						printf("->LP[%d]%s",z,hashTable[i].head->bcks[y].ptr0->pinkEntry[z].uniqID);
						printf("|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].destinatorNumber);
						printf("|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].date);
						printf("|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].inittime);
						printf("|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].date);
						printf("|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].duration);
						printf("|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].type);
						printf("|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].tarrif);
						printf("|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].faultCondition);
					}
				}
				printf("\n");		
			}
			else if(flag==2)
			{
							for(int y=0;y<hashTable[i].head->counter;y++)
				{
					printf("->BN[%d]",y);
					printf("|%s|",hashTable[i].head->bcks[y].number);	
					for(int z=0;z<hashTable[i].head->bcks[y].ptr0->cntr;z++)
					{
						printf("\n");
						printf("->LP[%d]%s",z,hashTable[i].head->bcks[y].ptr0->pinkEntry[z].uniqID);
						printf("|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].destinatorNumber);
						printf("|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].date);
						printf("|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].inittime);
						printf("|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].date);
						printf("|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].duration);
						printf("|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].type);
						printf("|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].tarrif);
						printf("|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].faultCondition);
					}
				}
				printf("\n");
			}
		}
		else 
		{ 
			printf("no key made \n");
		}
	}
}

void Delete(hash *hashTable,char *line,int hashTableEntries)
{
	printf("delete >>>>\n");
	//dimiourgia ena CDR struct 
	cdr* cdr2;
	cdr2 = malloc(sizeof(cdr));
	cdr2->uniqID=malloc(10*sizeof(char));
	cdr2->originatorNumber=malloc(14*sizeof(char));	
	cdr2->destinatorNumber=malloc(14*sizeof(char));
	cdr2->date=malloc(8*sizeof(char));
	cdr2->inittime=malloc(5*sizeof(char));
	cdr2->duration=malloc(4*sizeof(char));
	cdr2->type=malloc(sizeof(char));
	cdr2->tarrif=malloc(sizeof(char));
	cdr2->faultCondition=malloc(4*sizeof(char));
	//bazoume ta dedomena tou line sto struct CDR
	char *cdrEntry;
	cdrEntry = strtok(line," "); //first word
	cdrEntry = strtok(NULL," "); //second word
	strcpy(cdr2->uniqID,cdrEntry);
	cdrEntry = strtok(NULL,"\n"); //third word
	strcpy(cdr2->originatorNumber,cdrEntry);
	int findFlag=0;
	int findPos=0;
	int key1=hash1(cdr2->originatorNumber,hashTableEntries);
	//for(int key=0;key<hashTableEntries;key++)
	//{
		bucketList* assistant;
		assistant=hashTable[key1].head; //o assitant pernei to proto bucket pou deixnei to hashtable
		int blcounter=0;
		if(hashTable[key1].head != NULL)
		{
			//elenxoume an iparxei o caller se kapoio bucket		
			//int findPos=0;
			while(assistant != NULL)
			{	
				for(int o=0;o<assistant->counter;o++)
				{
					if(strcmp(cdr2->originatorNumber,assistant->bcks[o].number)==0)
					{
						findFlag=1; //brethike
						findPos = o;
						//key1=key;
						break;
					}	
				}if(findFlag==1){break;}
				assistant = hashTable[key1].head->ptr1;
				blcounter++;
			}
		}	
	//}
	if(findFlag == 1)
	{
		//tora tha psaksoume to light pink gia to uniqID
		lightPink* assistant1;	
		lightPink* assistant2;	
		assistant1 = assistant->bcks[findPos].ptr0;
		assistant2 = assistant->bcks[findPos].ptr0;
		int lpcounter=0;
		while(assistant2 != NULL){lpcounter++;assistant2 = assistant2->ptr2;}
		int findPinkPos=0;
		int findPinkFlag=0;
		int lpcounterpos=0;
		while(assistant1 != NULL)
		{		
			for(int z=0;z<assistant1->cntr;z++)
			{
				if(strcmp(cdr2->uniqID,assistant1->pinkEntry[z].uniqID)==0)
				{		
					findPinkFlag=1; // brethike to uniqID
					findPinkPos = z;
					break;
				}
			}if(findPinkFlag==1){break;}
			assistant1 = assistant1->ptr2;
			lpcounterpos++;
		}	
		if(findPinkFlag ==1)
		{	
			printf("	callers info with num {%s} and uniqID {%s} about to be  deleted\n",hashTable[key1].head->bcks[findPos].number,assistant1->pinkEntry[findPinkPos].uniqID);	
			//tora prepei na kano free ola ta stoixeia tou cdr entry sto pinkEntry
			free(assistant1->pinkEntry[findPinkPos].uniqID);
			free(assistant1->pinkEntry[findPinkPos].originatorNumber);
			free(assistant1->pinkEntry[findPinkPos].destinatorNumber);
			free(assistant1->pinkEntry[findPinkPos].date);
			free(assistant1->pinkEntry[findPinkPos].inittime);
			free(assistant1->pinkEntry[findPinkPos].duration);
			free(assistant1->pinkEntry[findPinkPos].type);
			free(assistant1->pinkEntry[findPinkPos].tarrif);
			free(assistant1->pinkEntry[findPinkPos].faultCondition);	
			if((((assistant1->cntr) - 1) == 0)&& lpcounter == 1) //an o pink exei mono ena stoixeio kai einai enas 
			{
				free(hashTable[key1].head->bcks[findPos].ptr0->pinkEntry);	
				if((assistant->counter - 1) == 0)
				{
					assistant->bcks[findPos].ptr0 = NULL;
					free(assistant->bcks[findPos].number);
					free(assistant->bcks);
					if((hashTable[key1].head == assistant )&&(assistant->ptr1 == NULL)) //an einai to proto apo to bucketlist kai den deixnei se allo tote 
					{
						hashTable[key1].head = NULL;
					}
					else if((hashTable[key1].head == assistant )&&(assistant->ptr1 != NULL)) //an einai to proto alla exei kai alla sto bucketlist
					{//prepei na enothei to epomeno me to head 
						assistant = assistant->ptr1;
						hashTable[key1].head = assistant;
					}
					else if((hashTable[key1].head != assistant )&&(assistant->ptr1 == NULL))//an einai to teleutaio tou bucketlist
					{//kanoume to ptr1 tou proigoumenou NULL
						bucketList *assistantBN;
						assistantBN = hashTable[key1].head;
						for(int i=0;i<(blcounter-1);i++)
						{
							assistantBN = assistantBN->ptr1;
						}//autos deixnei sto proigoumeno bucketList
						assistantBN->ptr1 == NULL;
					}
					else //an den einai oute sth teleutaia thesi oute auto pou deixnei to head tote
					{//prepei to proigoumeno apo auto na deiksei sto epomeno
						bucketList *assistantBN;
						assistantBN = hashTable[key1].head;
						for(int i=0;i<(blcounter-1);i++)
						{
							assistantBN = assistantBN->ptr1;
						}//autos deixnei sto proigoumeno bucketList
						assistantBN->ptr1 = assistant->ptr1;
					}
				}
				else if((assistant->counter -1 ) > 0)
				{
					if(findPos == assistant->counter)// an brisketai teleutaia thesi apla diagrapse t kai katebase to counter
					{
						assistant->bcks[findPos].ptr0 = NULL;
						free(assistant->bcks[findPos].number);
						assistant->counter=assistant->counter - 1;
					}
					else// an brisketai se allh thesi kanta ola olisthisi pros ta pano mia thesi
					{
						for(int i=findPos;i<((assistant->counter)-1);i++)
						{
							assistant->bcks[i].number = assistant->bcks[i+1].number;	
							assistant->bcks[i].ptr0 = assistant->bcks[i+1].ptr0;
						}	
						assistant->bcks[assistant->counter].ptr0 = NULL;
						free(assistant->bcks[assistant->counter].number);
						assistant->counter=assistant->counter - 1;
					}
				}
			} 
			else if((((assistant1->cntr) - 1) == 0)&& lpcounter > 1)
			{
				if((assistant1->ptr2==NULL)) //simainei pos einai to teleutaio lightpink
				{//kai prepei apla na to diagrapsoume kai na kanoume to ptr tou proigoumenou lightpink NULL
					free(assistant->bcks[findPos].ptr0->pinkEntry);
					lightPink* assistant3;
					assistant3 = assistant->bcks[findPos].ptr0; //pernei to ptr sto proto ligtpink	
					for(int i=0;i <lpcounterpos-1;i++) //o assistant3 ftanei mexri to proigoumeno  lightpink
					{
						assistant3 = assistant3->ptr2;
					}
					//kai tora kano null auto to ptr
					assistant1 = assistant3;
					assistant1->ptr2 = NULL;
				}
				else if((assistant1->ptr2 !=NULL)&&(assistant1 == assistant->bcks[findPos].ptr0))
				{// tote apla o assistant->bcks[findPos].ptr0 deixnei sto epomeno lighpink
					assistant->bcks[findPos].ptr0 = assistant1->ptr2;
				}				
				else
				{ // simainei pos den einai teleutaio kai prepei na diagrafei kai o proigoumenos ptr2 na deiksei sto parapepomeno
					free(assistant->bcks[findPos].ptr0->pinkEntry);
					lightPink* assistant3;
					assistant3 = assistant->bcks[findPos].ptr0; //pernei to ptr sto proto ligtpink	
					for(int i=0;i <lpcounterpos-1;i++) //o assistant3 ftanei mexri to proigoumeno  lightpink
					{
						assistant3 = assistant3->ptr2;
					} //exei ftasei sto proigoumeno lightpink
					assistant3 = assistant1->ptr2; // pame sto paraepomeno afou o assistant1 deixnei to lightpink pou theloume na diagrapsoume
					assistant1 = assistant3; //tora kanoume to proigoumeno tou assistant1->ptr2 na deixnei sto epomeno tou assistan1->ptr2
				}	
				
			}
			else if((((assistant1->cntr) - 1) > 0)&& ((lpcounter == 1)||(lpcounter > 1)))
			{
				//tote kano olisthisi pros ta pano ta stoixeia tou lightpink
				for(int i=findPinkPos;i<((assistant1->cntr)-1);i++)
				{
					assistant1->pinkEntry[i] = assistant1->pinkEntry[i+1];
				}
				//assistant1->pinkEntry[assistant1->cntr] = NULL;
				assistant1->cntr = assistant1->cntr - 1;
				
			}
			printf("	successfully deleted\n");
		}
		else
		{
			printf("	This UniqID is not on the list\n ");
		}
	}			
	else
	{
		printf("	caller number is not in the list \n");
	}	
	free(cdr2->uniqID);
	free(cdr2->originatorNumber);
	free(cdr2->destinatorNumber);
	free(cdr2->date);
	free(cdr2->inittime);
	free(cdr2->tarrif);
	free(cdr2->type);
	free(cdr2->faultCondition);
	free(cdr2->duration);
	free(cdr2);
}



int convertTime(char *time1)
{
	int h, m;
	sscanf(time1, "%d:%d", &h, &m);
	h = h * 100;
	int time2 = h + m;
	return time2;
}

int convertYear(char *year)
{
	//px 11 12 1999
	int y1= atoi(year) % 10000; // pernei to 1999
	int y2= atoi(year) / 10000; // pernei to 1112
	int a = y2 % 100; // pernei to 12
	int b = y2 / 100; // pernei to 11
	a = a * 100;	// kanei to 12 1200
	int x = a + b;  // ta prosthei kai ta kanei 1211
	y1 = y1 * 10000; // kanei to 1999 19990000
	int ap = y1 + x; // kai ta prosthetei
	return ap;	// apotelesma = 1999 12 11
}

void FindCaller(hash *hashTable,char *line1,char* line2,int hashTableEntries)
{
	printf("find >>>>\n");
	int i=0;
	char *cdrEntry;
	cdrEntry = strtok(line1," "); //first word :find
	//cdrEntry = strtok(NULL," "); //second word :caller
	while((cdrEntry = strtok(NULL," ")) != NULL)
	{
		i++;
	}	
	printf("	Arguments given in this line %d \n",i);
	if(i==1)
	{
		//an einai mono to call number bres to kai ektopose ola ta cdr tou
		cdr* cdr2;
		cdr2 = malloc(sizeof(cdr));
		cdr2->originatorNumber=malloc(14*sizeof(char));	
		char *cdrEntry;
		cdrEntry = strtok(line2," "); //first word :find
		//cdrEntry = strtok(NULL," "); //second word	:caller
		cdrEntry = strtok(NULL,"\n"); //third word
		strcpy(cdr2->originatorNumber,cdrEntry);
		printf("	number we print info for %s \n",cdr2->originatorNumber);
		//kanoume search gia to number pou dothike
		int findFlag=0;
		int findPos=0;
		int key1=hash1(cdr2->originatorNumber,hashTableEntries);
		//for(int key=0;key<hashTableEntries;key++)
		//{
			if(hashTable[key1].head != NULL)
			{
				//elenxoume an iparxei o caller se kapoio bucket		
				bucketList* assistant;
				assistant=hashTable[key1].head;
				//int findPos=0;
				while(assistant != NULL)
				{	
					for(int o=0;o<assistant->counter;o++)
					{
						if(strcmp(cdr2->originatorNumber,assistant->bcks[o].number)==0)
						{
							findFlag=1; //brethike
							findPos = o;
							//key1=key;
							break;
						}	
					}if(findFlag==1){break;}
					assistant = hashTable[key1].head->ptr1;
				}
			}	
		//}
		if(findFlag==1)
		{
			lightPink *ass;
			ass = hashTable[key1].head->bcks[findPos].ptr0;
			while(ass != NULL)
			{
				//afou vrikame th thesi tou caller tha ektiposoume ta cdrs
				for(int x=0;x<hashTable[key1].head->bcks[findPos].ptr0->cntr;x++)
				{
					printf("	|%s",ass->pinkEntry[x].uniqID);
					printf("	|%s",ass->pinkEntry[x].destinatorNumber);
					printf("	|%s",ass->pinkEntry[x].date);
					printf("	|%s",ass->pinkEntry[x].inittime);
					printf("	|%s",ass->pinkEntry[x].duration);
					printf("	|%s",ass->pinkEntry[x].type);
					printf("	|%s",ass->pinkEntry[x].tarrif);
					printf("	|%s",ass->pinkEntry[x].faultCondition);
				}
				ass = ass->ptr2;
			}
		}
		else
		{
			printf("	CALLER WITH NUM %s NOT FOUND\n",cdr2->originatorNumber);
		}	
		free(cdr2->originatorNumber);
		free(cdr2);	
	}
	else if(i==3)
	{
		//afou to i einai 3 exei ena caller number kai dio times
		cdr* cdr2;
		cdr2 = malloc(sizeof(cdr));
		cdr2->originatorNumber=malloc(14*sizeof(char));	
		cdr2->inittime=malloc(5*sizeof(char));
		cdr* cdr3;
		char *cdrEntry;
		cdr3 = malloc(sizeof(cdr));
		cdr3->inittime=malloc(5*sizeof(char));
		cdrEntry = strtok(line2," "); //first word :find
		//cdrEntry = strtok(NULL," "); //second word	:caller
		cdrEntry = strtok(NULL," "); //third word
		strcpy(cdr2->originatorNumber,cdrEntry);
		cdrEntry = strtok(NULL," ");//fourth word
		strcpy(cdr2->inittime,cdrEntry);
		cdrEntry = strtok(NULL,"\n");//fifth word
		strcpy(cdr3->inittime,cdrEntry);
		printf("	number we print info for %s in time range <%s,%s>\n",cdr2->originatorNumber,cdr2->inittime,cdr3->inittime);
		//kanoume search gia to number pou dothike
		int findFlag=0;
		int findPos=0;
		int key1=hash1(cdr2->originatorNumber,hashTableEntries);
	//	for(int key=0;key<hashTableEntries;key++)
		//{
			if(hashTable[key1].head != NULL)
			{
				//elenxoume an iparxei o caller se kapoio bucket		
				bucketList* assistant;
				assistant=hashTable[key1].head;
				//int findPos=0;
				while(assistant != NULL)
				{	
					for(int o=0;o<assistant->counter;o++)
					{
						if(strcmp(cdr2->originatorNumber,assistant->bcks[o].number)==0)
						{
							findFlag=1; //brethike
							findPos = o;
							//key1=key;
							break;
						}	
					}if(findFlag==1){break;}
					assistant = hashTable[key1].head->ptr1;
				}
			}	
		//}
		int time1=convertTime(cdr2->inittime);
		int time2=convertTime(cdr3->inittime);
		int flag3=0;
		if(findFlag==1)
		{
			lightPink *ass;
			ass = hashTable[key1].head->bcks[findPos].ptr0;
			while(ass != NULL)
			{
				for(int x=0;x<hashTable[key1].head->bcks[findPos].ptr0->cntr;x++)
				{
					int time3=convertTime(hashTable[key1].head->bcks[findPos].ptr0->pinkEntry[x].inittime);
					if(time3 > time1 && time3 <time2)
					{
						flag3=1;
						printf("	|%s",ass->pinkEntry[x].uniqID);
						printf("	|%s",ass->pinkEntry[x].destinatorNumber);
						printf("	|%s",ass->pinkEntry[x].date);
						printf("	|%s",ass->pinkEntry[x].inittime);
						printf("	|%s",ass->pinkEntry[x].duration);
						printf("	|%s",ass->pinkEntry[x].type);
						printf("	|%s",ass->pinkEntry[x].tarrif);
						printf("	|%s",ass->pinkEntry[x].faultCondition);		
					}
				}
				if(flag3==0){printf("	NO RECORDS FOUND FOR THIS CALLER\n");}
				ass = ass->ptr2;
			}
		}
		else
		{
			printf("	CALLER WITH NUM %s NOT FOUND\n",cdr2->originatorNumber);
		}
		free(cdr2->originatorNumber);
		free(cdr2->inittime);
		free(cdr2);
		free(cdr3->inittime);
		free(cdr3);
	}
	else if(i==5)
	{
		//afou to i einai 5 exei ena caller number kai dio times kai dio dates
		cdr* cdr2;
		cdr2 = malloc(sizeof(cdr));
		cdr2->originatorNumber=malloc(14*sizeof(char));	
		cdr2->inittime=malloc(5*sizeof(char));
		cdr2->date=malloc(8*sizeof(char));
		cdr* cdr3;
		char *cdrEntry;
		cdr3 = malloc(sizeof(cdr));
		cdr3->inittime=malloc(5*sizeof(char));
		cdr3->date=malloc(8*sizeof(char));
		cdrEntry = strtok(line2," "); //first word :find
		//cdrEntry = strtok(NULL," "); //second word	:caller
		cdrEntry = strtok(NULL," "); //third word :number
		strcpy(cdr2->originatorNumber,cdrEntry);
		cdrEntry = strtok(NULL," ");//fourth word :first time
		strcpy(cdr2->inittime,cdrEntry);
		cdrEntry = strtok(NULL," ");//fifth word :first date
		strcpy(cdr2->date,cdrEntry);
		cdrEntry=strtok(NULL," "); //sixth word :second time
		strcpy(cdr3->inittime,cdrEntry);
		cdrEntry=strtok(NULL,"\n"); //seveth/last word :second date
		strcpy(cdr3->date,cdrEntry);
		printf("	number we print info for %s in time range <%s,%s> and date range <%s,%s>\n",cdr2->originatorNumber,cdr2->inittime,cdr3->inittime,cdr2->date,cdr3->date);
		//kanoume search gia to number pou dothike
		int findFlag=0;
		int findPos=0;
		int key1=hash1(cdr2->originatorNumber,hashTableEntries);
		//for(int key=0;key<hashTableEntries;key++)
		//{
			if(hashTable[key1].head != NULL)
			{
				//elenxoume an iparxei o caller se kapoio bucket		
				bucketList* assistant;
				assistant=hashTable[key1].head;
				//int findPos=0;
				while(assistant != NULL)
				{	
					for(int o=0;o<assistant->counter;o++)
					{
						if(strcmp(cdr2->originatorNumber,assistant->bcks[o].number)==0)
						{
							findFlag=1; //brethike
							findPos = o;
							//key1=key;
							break;
						}	
					}if(findFlag==1){break;}
					assistant = hashTable[key1].head->ptr1;
				}
			}	
		//}
		int time1=convertTime(cdr2->inittime);
		int time2=convertTime(cdr3->inittime);
		int year1=convertYear(cdr2->date);
		int year2=convertYear(cdr3->date);
		int flag5=0;
		if(findFlag==1)
		{
			lightPink *ass;
			ass = hashTable[key1].head->bcks[findPos].ptr0;
			while(ass != NULL)
			{
				for(int x=0;x<hashTable[key1].head->bcks[findPos].ptr0->cntr;x++)
				{
					int time3=convertTime(hashTable[key1].head->bcks[findPos].ptr0->pinkEntry[x].inittime);
					int year3=convertYear(hashTable[key1].head->bcks[findPos].ptr0->pinkEntry[x].date);
					if((time3 > time1 && time3 <time2)&&(year3 > year1 && year3 < year2))
					{
						flag5=1;
						printf("	|%s",ass->pinkEntry[x].uniqID);
						printf("	|%s",ass->pinkEntry[x].destinatorNumber);
						printf("	|%s",ass->pinkEntry[x].date);
						printf("	|%s",ass->pinkEntry[x].inittime);
						printf("	|%s",ass->pinkEntry[x].duration);
						printf("	|%s",ass->pinkEntry[x].type);
						printf("	|%s",ass->pinkEntry[x].tarrif);
						printf("	|%s",ass->pinkEntry[x].faultCondition);		
					}
				}	
				if(flag5==0){printf("	NO RECORDS FOUND FOR THIS CALLER\n");}
				ass = ass->ptr2;
			}
		}
		else
		{
			printf("	CALLER WITH NUM %s NOT FOUND\n",cdr2->originatorNumber);
		}
		free(cdr2->originatorNumber);
		free(cdr2->inittime);
		free(cdr2->date);
		free(cdr2);
		free(cdr3->inittime);
		free(cdr3->date);
		free(cdr3);
	}
}
void LookUp(hash *hashTable,char *line1,char* line2,int hashTableEntries)
{
	printf("lookup >>>>\n");
	int i=0;
	char *cdrEntry;
	cdrEntry = strtok(line1," "); //first word :lookup
	while((cdrEntry = strtok(NULL," ")) != NULL)
	{
		i++;
	}	
	printf("	Arguments given in this line %d \n",i);
	if(i==1)
	{
		//an einai mono to call number bres to kai ektopose ola ta cdr tou
		cdr* cdr2;
		cdr2 = malloc(sizeof(cdr));
		cdr2->destinatorNumber=malloc(14*sizeof(char));	
		char *cdrEntry;
		cdrEntry = strtok(line2," "); //first word :lookup
		cdrEntry = strtok(NULL,"\n"); //second word
		strcpy(cdr2->destinatorNumber,cdrEntry);
		printf("	number we print info for %s \n",cdr2->destinatorNumber);
		//kanoume search gia to number pou dothike
		int findFlag=0;
		int findPos=0;
		int key1=hash1(cdr2->destinatorNumber,hashTableEntries);
		//for(int key=0;key<hashTableEntries;key++)
		//{
			if(hashTable[key1].head != NULL)
			{
				//elenxoume an iparxei o caller se kapoio bucket		
				bucketList* assistant;
				assistant=hashTable[key1].head;
				//int findPos=0;
				while(assistant != NULL)
				{	
					for(int o=0;o<assistant->counter;o++)
					{
						if(strcmp(cdr2->destinatorNumber,assistant->bcks[o].number)==0)
						{
							findFlag=1; //brethike
							findPos = o;
							//key1=key;
							break;
						}	
					}if(findFlag==1){break;}
					assistant = hashTable[key1].head->ptr1;
				}
			}	
		//}
		int flag1=0;
		if(findFlag==1)
		{
			lightPink *ass;
			ass = hashTable[key1].head->bcks[findPos].ptr0;
			while(ass != NULL)
			{
				//afou vrikame th thesi tou caller tha ektiposoume ta cdrs
				for(int x=0;x<hashTable[key1].head->bcks[findPos].ptr0->cntr;x++)
				{
					flag1=1;
					printf("	|%s",ass->pinkEntry[x].uniqID);
					printf("	|%s",ass->pinkEntry[x].originatorNumber);
					printf("	|%s",ass->pinkEntry[x].date);
					printf("	|%s",ass->pinkEntry[x].inittime);
					printf("	|%s",ass->pinkEntry[x].duration);
					printf("	|%s",ass->pinkEntry[x].type);
					printf("	|%s",ass->pinkEntry[x].tarrif);
					printf("	|%s",ass->pinkEntry[x].faultCondition);
				}
				if(flag1==0){printf("	NO RECORDS FOUND FOR THIS CALEE\n");}
				ass = ass->ptr2;		
				
			}
		}
		else
		{
			printf("	CALEE WITH NUM %s NOT FOUND\n",cdr2->destinatorNumber);
		}		
		free(cdr2->destinatorNumber);
		free(cdr2);
	}
	else if(i==3)
	{
		//afou to i einai 3 exei ena caller number kai dio times
		cdr* cdr2;
		cdr2 = malloc(sizeof(cdr));
		cdr2->destinatorNumber=malloc(14*sizeof(char));	
		cdr2->inittime=malloc(5*sizeof(char));
		cdr* cdr3;
		char *cdrEntry;
		cdr3 = malloc(sizeof(cdr));
		cdr3->inittime=malloc(5*sizeof(char));
		cdrEntry = strtok(line2," "); //first word :lookup
		cdrEntry = strtok(NULL," "); //second word
		strcpy(cdr2->destinatorNumber,cdrEntry);
		cdrEntry = strtok(NULL," ");//third word
		strcpy(cdr2->inittime,cdrEntry);
		cdrEntry = strtok(NULL,"\n");//fourth word
		strcpy(cdr3->inittime,cdrEntry);
		printf("	number we print info for %s in time range <%s,%s>\n",cdr2->destinatorNumber,cdr2->inittime,cdr3->inittime);
		//kanoume search gia to number pou dothike
		int findFlag=0;
		int findPos=0;
		int key1=hash1(cdr2->destinatorNumber,hashTableEntries);
		//for(int key=0;key<hashTableEntries;key++)
		//{
			if(hashTable[key1].head != NULL)
			{
				//elenxoume an iparxei o caller se kapoio bucket		
				bucketList* assistant;
				assistant=hashTable[key1].head;
				//int findPos=0;
				while(assistant != NULL)
				{	
					for(int o=0;o<assistant->counter;o++)
					{
						if(strcmp(cdr2->destinatorNumber,assistant->bcks[o].number)==0)
						{
							findFlag=1; //brethike
							findPos = o;
							//key1=key;
							break;
						}	
					}if(findFlag==1){break;}
					assistant = hashTable[key1].head->ptr1;
				}
			}	
		//}
		int time1=convertTime(cdr2->inittime);
		int time2=convertTime(cdr3->inittime);
		int flag3=0;
		if(findFlag==1)
		{
			lightPink *ass;
			ass = hashTable[key1].head->bcks[findPos].ptr0;
			while(ass != NULL)
			{
				for(int x=0;x<hashTable[key1].head->bcks[findPos].ptr0->cntr;x++)
				{
					int time3=convertTime(hashTable[key1].head->bcks[findPos].ptr0->pinkEntry[x].inittime);
					if(time3 > time1 && time3 <time2)
					{
						flag3=1;
						printf("	|%s",ass->pinkEntry[x].uniqID);
						printf("	|%s",ass->pinkEntry[x].originatorNumber);
						printf("	|%s",ass->pinkEntry[x].date);
						printf("	|%s",ass->pinkEntry[x].inittime);
						printf("	|%s",ass->pinkEntry[x].duration);
						printf("	|%s",ass->pinkEntry[x].type);
						printf("	|%s",ass->pinkEntry[x].tarrif);
						printf("	|%s",ass->pinkEntry[x].faultCondition);		
					}
				}
				if(flag3==0){printf("	NO RECORDS FOUND FOR THIS CALEE\n");}
				ass = ass->ptr2;
			}
		}
		else
		{
			printf("	CALEE WITH NUM %s NOT FOUND\n",cdr2->destinatorNumber);
		}
		free(cdr2->destinatorNumber);
		free(cdr2->inittime);
		free(cdr2);
		free(cdr3->inittime);
		free(cdr3);
	}
	else if(i==5)
	{
		//afou to i einai 5 exei ena caller number kai dio times kai dio dates
		cdr* cdr2;
		cdr2 = malloc(sizeof(cdr));
		cdr2->destinatorNumber=malloc(14*sizeof(char));	
		cdr2->inittime=malloc(5*sizeof(char));
		cdr2->date=malloc(8*sizeof(char));
		cdr* cdr3;
		char *cdrEntry;
		cdr3 = malloc(sizeof(cdr));
		cdr3->inittime=malloc(5*sizeof(char));
		cdr3->date=malloc(8*sizeof(char));
		cdrEntry = strtok(line2," "); //first word :lookup
		cdrEntry = strtok(NULL," "); //second word :number
		strcpy(cdr2->destinatorNumber,cdrEntry);
		cdrEntry = strtok(NULL," ");//third word :first time
		strcpy(cdr2->inittime,cdrEntry);
		cdrEntry = strtok(NULL," ");//fourth word :first date
		strcpy(cdr2->date,cdrEntry);
		cdrEntry=strtok(NULL," "); //fifth word :second time
		strcpy(cdr3->inittime,cdrEntry);
		cdrEntry=strtok(NULL,"\n"); //sixth/last word :second date
		strcpy(cdr3->date,cdrEntry);
		printf("	number we print info for %s in time range <%s,%s> and date range <%s,%s>\n",cdr2->destinatorNumber,cdr2->inittime,cdr3->inittime,cdr2->date,cdr3->date);
		//kanoume search gia to number pou dothike
		int findFlag=0;
		int findPos=0;
		int key1=hash1(cdr2->destinatorNumber,hashTableEntries);
		//for(int key=0;key<hashTableEntries;key++)
		//{
			if(hashTable[key1].head != NULL)
			{
				//elenxoume an iparxei o caller se kapoio bucket		
				bucketList* assistant;
				assistant=hashTable[key1].head;
				//int findPos=0;
				while(assistant != NULL)
				{	
					for(int o=0;o<assistant->counter;o++)
					{
						if(strcmp(cdr2->destinatorNumber,assistant->bcks[o].number)==0)
						{
							findFlag=1; //brethike
							findPos = o;
						//	key1=key;
							break;
						}	
					}if(findFlag==1){break;}
					assistant = hashTable[key1].head->ptr1;
				}
			}	
		//}
		int time1=convertTime(cdr2->inittime);
		int time2=convertTime(cdr3->inittime);
		int year1=convertYear(cdr2->date);
		int year2=convertYear(cdr3->date);
		int flag5=0;
		if(findFlag==1)
		{
		lightPink *ass;
			ass = hashTable[key1].head->bcks[findPos].ptr0;
			while(ass != NULL)
			{
				for(int x=0;x<hashTable[key1].head->bcks[findPos].ptr0->cntr;x++)
				{
					int time3=convertTime(hashTable[key1].head->bcks[findPos].ptr0->pinkEntry[x].inittime);
					int year3=convertYear(hashTable[key1].head->bcks[findPos].ptr0->pinkEntry[x].date);
					if((time3 > time1 && time3 <time2)&&(year3 > year1 && year3 < year2))
					{
						flag5=1;
						printf("	|%s",ass->pinkEntry[x].uniqID);
						printf("	|%s",ass->pinkEntry[x].originatorNumber);
						printf("	|%s",ass->pinkEntry[x].date);
						printf("	|%s",ass->pinkEntry[x].inittime);
						printf("	|%s",ass->pinkEntry[x].duration);
						printf("	|%s",ass->pinkEntry[x].type);
						printf("	|%s",ass->pinkEntry[x].tarrif);
						printf("	|%s",ass->pinkEntry[x].faultCondition);		
					}
				}	
				ass=ass->ptr2;			
				if(flag5==0){printf("	NO RECORDS FOUND FOR THIS CALEE\n");}
			}
		}
		else
		{
			printf("	CALEE WITH NUM %s NOT FOUND\n",cdr2->destinatorNumber);
		}
		free(cdr2->destinatorNumber);
		free(cdr2->inittime);
		free(cdr2->date);
		free(cdr2);
		free(cdr3->inittime);
		free(cdr3->date);
		free(cdr3);
	}			
}

char* countryID(char *x)
{
	char i[20];
	strcpy(i,x);
   char *token;
   
   /* get the first token */
   token = strtok(i, "-");
	return token;
}

void TopDest(hash *hashTable,char *line,int hashTableEntries,int bucketSize)
{	
	printf("topdest >>>>\n");
	int maxSize = bucketSize / (sizeof(char)*14);
	//bres to call number
	cdr* cdr2;
	cdr2 = malloc(sizeof(cdr));
	cdr2->originatorNumber=malloc(14*sizeof(char));	
	char *cdrEntry;
	cdrEntry = strtok(line," "); //first word :topdest
	cdrEntry = strtok(NULL,"\n"); //second word	
	strcpy(cdr2->originatorNumber,cdrEntry);
	printf("	number we print info for %s \n",cdr2->originatorNumber);
	//kanoume search gia to number pou dothike
	int findFlag=0;
	int findPos=0;
	int key1=hash1(cdr2->originatorNumber,hashTableEntries);
	//for(int key=0;key<hashTableEntries;key++)
	//{
		if(hashTable[key1].head != NULL)
		{
			//elenxoume an iparxei o caller se kapoio bucket		
			bucketList* assistant;
			assistant=hashTable[key1].head;
			//int findPos=0;
			while(assistant != NULL)
			{	
				for(int o=0;o<assistant->counter;o++)
				{
					if(strcmp(cdr2->originatorNumber,assistant->bcks[o].number)==0)
					{
						findFlag=1; //brethike
						findPos = o;
						//key1=key;
						break;
					}	
				}if(findFlag==1){break;}
				assistant = hashTable[key1].head->ptr1;
			}
		}	
	//}
	if(findFlag==1)
	{
		lightPink *ass0;
		ass0 = hashTable[key1].head->bcks[findPos].ptr0;
		int lpcounter=0;
		int totalsize=0;
		while(ass0 !=NULL) //edo metrame posa einai ta lightpink 
		{
			totalsize = totalsize + ass0 -> cntr;
			lpcounter++;
			ass0 = ass0->ptr2;
		}
		typedef struct assist //prepei gia kathe lightpink na ftiakso th boithitikh domi assist
		{
			char *cid;
			int counter;
		}assist;
		assist *assist1;
		assist1 =(assist*) malloc(totalsize*sizeof(assist));
		for(int i=0;i<totalsize;i++)
		{
			assist1[i].cid=malloc(4*sizeof(char));
			assist1[i].counter=0;
		}
		lightPink *ass1;
		ass1 = hashTable[key1].head->bcks[findPos].ptr0;	
		int x=0;
		int sum=0;
		while(ass1 != NULL)
		{	
			sum = sum + ass1->cntr;
			int i=0;
			for(x=sum-(ass1->cntr);x<sum;x++)
			{
				strcpy(assist1[x].cid,countryID(ass1->pinkEntry[i].destinatorNumber));
				i++;
			}
			ass1 = ass1->ptr2; // kai pame sto epomeno lightpink
		}
		for(int i=0;i<totalsize;i++)
		{	
			char* assistline;
			assistline = malloc(4*sizeof(char));
			strcpy(assistline,assist1[i].cid);
			for(int y=0;y<totalsize;y++)	
			{
				if(strcmp(assistline,assist1[y].cid)==0)
				{
					assist1[y].counter++;
				}
			}
		}
		//prepei na vro tora to max 
		int max1 = assist1[0].counter ;//proto stoixeio tou pinakak
		for(int i=0;i<totalsize;i++)
		{
			if(assist1[i].counter > max1)
			{
				max1 = assist1[i].counter;
			}
		}
		char assistline1[totalsize][4]; //string pou ektipothikan
		int firsttime=0;
		int findbool=0;
		int w=0;
		for(int i=0;i<totalsize;i++)
		{
			if(max1 == assist1[i].counter)
			{
				if(firsttime == 0)
				{
					printf("	Caller called %d times to the countryID %s\n",max1,assist1[i].cid);
					strcpy(assistline1[w],assist1[i].cid);	
					firsttime =1;
				}
				else
				{
					for(int f=0;f<totalsize;f++)
					{
						if(strcmp(assistline1[f],assist1[i].cid)==0)
						{
							findbool=1;
							break;
						}	
						else // an den brethei kane reset to findbool
						{
							findbool=0;
						}
					}
					if(findbool == 0)
					{
						printf("	Caller called %d times to the countryID %s\n",max1,assist1[i].cid);
						w++;
						strcpy(assistline1[w],assist1[i].cid);	
					}	
				}
			}
		}
	}
	else
	{
		printf("	CALLER WITH NUM %s NOT FOUND\n",cdr2->originatorNumber);
	} 
}

void Dump(hash *hashTable,int hashTableEntries,FILE* ff,int flag)
{
	printf("DUMP >>>> \n");
	for(int i=0;i<hashTableEntries;i++)
	{
		if(flag==1){if(i==0){printf("Dumping hashtable1 right now\n");fprintf(ff,"%s \n","-------------------------HASHTABLE1-------------------------");}}
		else if(flag==2){if(i==0){printf("Dumping hashtable1 right now\n");fprintf(ff,"%s \n","-------------------------HASHTABLE2-------------------------");}}
		fprintf(ff,"HT[%d]",i);
		if(hashTable[i].head != NULL)
		{
			if(flag==1)
			{
				for(int y=0;y<hashTable[i].head->counter;y++)
				{
					fprintf(ff,"->BN[%d]",y);
					fprintf(ff,"|%s|",hashTable[i].head->bcks[y].number);	
					for(int z=0;z<hashTable[i].head->bcks[y].ptr0->cntr;z++)
					{
						fprintf(ff,"\n");
						fprintf(ff,"->LP[%d]%s",z,hashTable[i].head->bcks[y].ptr0->pinkEntry[z].uniqID);
						fprintf(ff,"|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].destinatorNumber);
						fprintf(ff,"|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].date);
						fprintf(ff,"|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].inittime);
						fprintf(ff,"|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].date);
						fprintf(ff,"|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].duration);
						fprintf(ff,"|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].type);
						fprintf(ff,"|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].tarrif);
						fprintf(ff,"|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].faultCondition);
					}
				}
				fprintf(ff,"\n");		
			}
			else if(flag==2)
			{
				for(int y=0;y<hashTable[i].head->counter;y++)
				{
					fprintf(ff,"->BN[%d]",y);
					fprintf(ff,"|%s|",hashTable[i].head->bcks[y].number);	
					for(int z=0;z<hashTable[i].head->bcks[y].ptr0->cntr;z++)
					{
						fprintf(ff,"\n");
						fprintf(ff,"->LP[%d]%s",z,hashTable[i].head->bcks[y].ptr0->pinkEntry[z].uniqID);
						fprintf(ff,"|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].destinatorNumber);
						fprintf(ff,"|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].date);
						fprintf(ff,"|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].inittime);
						fprintf(ff,"|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].date);
						fprintf(ff,"|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].duration);
						fprintf(ff,"|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].type);
						fprintf(ff,"|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].tarrif);
						fprintf(ff,"|%s",hashTable[i].head->bcks[y].ptr0->pinkEntry[z].faultCondition);
					}
				}
				fprintf(ff,"\n");
			}
		}
		else 
		{ 
			fprintf(ff,"no key made \n");
		}
	}
}

void Indist(hash *hashTable,char *line1,int hashTableEntries)
{
	printf("indist >>>>\n");
	cdr* cdr1;
	cdr1 = malloc(sizeof(cdr));
	cdr1->originatorNumber = malloc(14*sizeof(char));
	cdr* cdr2;
	cdr2 = malloc(sizeof(cdr));
	cdr2->originatorNumber = malloc(14*sizeof(char));
	char *cdrEntry;
	cdrEntry = strtok(line1," "); //first word :Indist
	cdrEntry = strtok(NULL," "); //second word	
	strcpy(cdr1->originatorNumber,cdrEntry);
	cdrEntry = strtok(NULL,"\n");//thrird word
	strcpy(cdr2->originatorNumber,cdrEntry);
	printf("	Indist for numbers<%s|%s> \n",cdr1->originatorNumber,cdr2->originatorNumber);
	int findPos1=0;
	int findPos2=0;
	int counter1222333=0;
	for(int key=0;key<hashTableEntries;key++)
	{
		if(hashTable[key].head != NULL)
		{
			//elenxoume an iparxei o caller se kapoio bucket		
			bucketList* assistant;
			assistant=hashTable[key].head;
			while(assistant != NULL)
			{	
				for(int o=0;o<assistant->counter;o++)
				{
					lightPink* assistant2;
					assistant2 = assistant->bcks[o].ptr0;
					int found1=0;
					int found2=0;
					
					while(assistant2 != NULL)
					{
						for(int z=0;z<assistant2->cntr;z++)
						{
							if(strcmp(assistant2->pinkEntry[z].destinatorNumber,cdr1->originatorNumber)==0)
							{
								findPos1=z;
								found1=1;
								if(found1==1 && found2==1){break;}
							}
							if(strcmp(assistant2->pinkEntry[z].destinatorNumber,cdr2->originatorNumber)==0)
							{
								findPos2=z;
								found2=1;
								if(found1==1 && found2==1){break;}
							}
						}if(found1==1 && found2==1){break;}	
					assistant2 = assistant2->ptr2;	
					}
					if(found1==1 && found2==1)
					{
						if((strcmp(assistant->bcks[o].number,assistant->bcks[o].ptr0->pinkEntry[findPos1].destinatorNumber)!=0)&&(strcmp(assistant->bcks[o].number,assistant->bcks[o].ptr0->pinkEntry[findPos2].destinatorNumber)!=0))	
						{
							printf("	%s \n",assistant->bcks[o].number);
							counter1222333++;
						}
					}
				}
				assistant = hashTable[key].head->ptr1;
			}
		}	
	}		
	if(counter1222333==0)
	{
		printf("	No Indist Found\n");
	}
}

void Bye(hash* hashTable1,hash *hashTable2,int hashTableEntries1,int hashTableEntries2)
{
	printf("bye >>>>\n");
	for(int i=0;i<hashTableEntries1;i++)
	{
		bucketList *assistant;
		assistant = hashTable1[i].head;
		while(assistant != NULL)
		{
			for(int y=0;y<assistant->counter;y++)
			{
				lightPink *assistant2;
				assistant2 = assistant->bcks[y].ptr0;
				while(assistant2 != NULL)
				{
					for(int z=0;z<assistant2->cntr;z++)
					{
						free(assistant2->pinkEntry[z].uniqID);
						free(assistant2->pinkEntry[z].originatorNumber);
						free(assistant2->pinkEntry[z].destinatorNumber);
						free(assistant2->pinkEntry[z].date);
						free(assistant2->pinkEntry[z].inittime);
						free(assistant2->pinkEntry[z].duration);
						free(assistant2->pinkEntry[z].type);
						free(assistant2->pinkEntry[z].tarrif);
						free(assistant2->pinkEntry[z].faultCondition);
					}	
					free(assistant2->pinkEntry);
					assistant2->cntr=0;
					assistant2 = assistant2->ptr2;
					hashTable1[i].head->bcks[y].ptr0->ptr2 = NULL;
					hashTable1[i].head->bcks[y].ptr0 = assistant2;				
				}
				assistant->bcks[y].ptr0 = NULL;
				free(assistant->bcks[y].number);
			}
			free(assistant->bcks);
			assistant->counter =0;
			assistant = assistant->ptr1;
			hashTable1[i].head->ptr1 = NULL;
			hashTable1[i].head = assistant;
		}
		hashTable1[i].head = NULL;
	}
	for(int i=0;i<hashTableEntries2;i++)
	{
		bucketList *assistant;
		assistant = hashTable2[i].head;
		while(assistant != NULL)
		{
			for(int y=0;y<assistant->counter;y++)
			{
				lightPink *assistant2;
				assistant2 = assistant->bcks[y].ptr0;
				while(assistant2 != NULL)
				{
					for(int z=0;z<assistant2->cntr;z++)
					{
						free(assistant2->pinkEntry[z].uniqID);
						free(assistant2->pinkEntry[z].originatorNumber);
						free(assistant2->pinkEntry[z].destinatorNumber);
						free(assistant2->pinkEntry[z].date);
						free(assistant2->pinkEntry[z].inittime);
						free(assistant2->pinkEntry[z].duration);
						free(assistant2->pinkEntry[z].type);
						free(assistant2->pinkEntry[z].tarrif);
						free(assistant2->pinkEntry[z].faultCondition);
					}	
					free(assistant2->pinkEntry);
					assistant2->cntr=0;
					assistant2 = assistant2->ptr2;
					hashTable2[i].head->bcks[y].ptr0->ptr2 = NULL;
					hashTable2[i].head->bcks[y].ptr0 = assistant2;				
				}
				assistant->bcks[y].ptr0 = NULL;
				free(assistant->bcks[y].number);
			}
			free(assistant->bcks);
			assistant->counter =0;
			assistant = assistant->ptr1;
			hashTable2[i].head->ptr1 = NULL;
			hashTable2[i].head = assistant;
		}
		hashTable2[i].head = NULL;
	}
	printf("	Memory is Clean\n");	
}









