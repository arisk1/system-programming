#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fuctions.h"

int main(int argc,char* argv[])
{
	int HashTableEntries1,HashTableEntries2;
	int i;
	int end=0;
	int BucketSize;
	char filename[80];
	char *input;
	size_t size=0;
	FILE *file=NULL;
	if(argc < 6 || argc == 8 || argc >9)
	{
		printf("Error-wrong number of arguments! \n");
		exit(-1);
		end = 1;
	}
	else if(argc ==7)	
	{
		for(i=1;i<=5;i+=2)
		{
			if(strcmp("-h1",argv[i])==0)
			{
				HashTableEntries1=atoi(argv[i+1]);
			}
			else if(strcmp("-h2",argv[i])==0)
			{
				HashTableEntries2=atoi(argv[i+1]);
			}
			else if(strcmp("-s",argv[i])==0)
			{
				BucketSize=atoi(argv[i+1]);
			}
			else
			{
				printf("wrong argument \n");
			}
		}
	}
	else if(argc == 9)
	{
		for(i=1;i<=7;i+=2)
		{
			if(strcmp("-o",argv[i])==0)
			{
				strcpy(filename,argv[i+1]);
				file = fopen(filename, "r");
				if(file == NULL)
				{
					printf("file not found.Give me an option \n");
				}
			}
			else if(strcmp("-h1",argv[i])==0)
			{
				HashTableEntries1=atoi(argv[i+1]);
			}
			else if(strcmp("-h2",argv[i])==0)
			{
				HashTableEntries2=atoi(argv[i+1]);
			}
			else if(strcmp("-s",argv[i])==0)
			{
				BucketSize=atoi(argv[i+1]);
			}
			else
			{
				printf("wrong argument \n");
			}
		}
	}
	
	

	//ftiaxno ta 2 hashtable
	hash hashTable1[HashTableEntries1];
	hash hashTable2[HashTableEntries2];
	CreateHT(HashTableEntries1,hashTable1);	
	CreateHT(HashTableEntries2,hashTable2);
	while(end==0)
	{
		if(file!=NULL)
		{
			char line1[250];//insert1
			char line2[250];//insert2	
			//char line3[250];//delete
			//char line4[250];//findcaller1
			//char line5[250];//findcaller2
			//char line6[250];//lookup1
			//char line7[250];//lookup2
			//char line8[250];//topdest	
			//char line9[250];//indist			
			int flag=0;
			while (fgets(line1, sizeof(line1), file)) 
			{
				strcpy(line2,line1);
				//strcpy(line3,line1);
				//strcpy(line4,line1);
				//strcpy(line5,line1);
				//strcpy(line6,line1);
				//strcpy(line7,line1);
				//strcpy(line8,line1);
				//strcpy(line9,line1);
				if((strstr(line1,"insert")!=NULL))
				{
					Insert1(line1,hashTable1,BucketSize,HashTableEntries1);
					Insert2(line2,hashTable2,BucketSize,HashTableEntries2);
				}
				else if((strstr(line1,"delete")!=NULL)){Delete(hashTable1,line1,HashTableEntries1);}
				else if((strstr(line1,"find")!=NULL)){FindCaller(hashTable1,line1,line2,HashTableEntries1);}
				else if((strstr(line1,"lookup")!=NULL)){LookUp(hashTable2,line1,line2,HashTableEntries2);}
				else if((strstr(line1,"topk")!=NULL)){/*Topk(line1);*/}
				else if((strstr(line1,"indist")!=NULL)){Indist(hashTable1,line1,HashTableEntries1);}
				else if((strstr(line1,"topdest")!=NULL)){TopDest(hashTable1,line1,HashTableEntries1,BucketSize);}
				else if((strstr(line1,"print hashtable1")!=NULL))
				{
					flag=1;
					Print(hashTable1,HashTableEntries1,flag);
				}
				else if((strstr(line1,"print hashtable2")!=NULL))
				{	
					flag=2;
					Print(hashTable2,HashTableEntries2,flag);
				}
				else if((strstr(line1,"dump hashtable1")!=NULL))
				{
					flag=1;
					char* ln;
					char ll[20];
					ln = strtok(line1," "); //first word
					ln = strtok(NULL," ");
					ln = strtok(NULL,"\n");//filename
					strcpy(ll,ln);
					FILE* fp=fopen(ll,"w");
					Dump(hashTable1,HashTableEntries1,fp,flag);
					printf("	Main---File [%s] Created\n",ll);
					fclose(fp); 
				}
				else if((strstr(line1,"dump hashtable2")!=NULL))
				{
					flag=2;
					char* ln1;
					char ll1[20];
					ln1 = strtok(line1," "); //first word
					ln1 = strtok(NULL," ");
					ln1 = strtok(NULL,"\n");//filename
					strcpy(ll1,ln1);
					FILE* fp=fopen(ll1,"w");
					Dump(hashTable2,HashTableEntries2,fp,flag);
					printf("	Main---File [%s] Created\n",ll1);
					fclose(fp);
				} 
				else if((strstr(line1,"bye")!=NULL)){Bye(hashTable1,hashTable2,HashTableEntries1,HashTableEntries2);}
				
			}	
			file = NULL;
		}
		if((file == NULL))
		{
			printf("Give me your Option\n");
			if(getline(&input,&size,stdin)==-1)
			{
				if(feof(stdin))
				{
					//printf(" %s \n",input);
					printf("Terminating the Programm\n");
					end=1;
					continue;
				}
				else
				{
					printf("Error in reading input!\n");
					continue;	
				}
			}		
		}
		char *line10;
		char line11[250];
		line10 = strtok(input,"\n");
		strcpy(line11,line10);
		int x= strlen(line11);
		int flag=0;
		if(userCommand(line11)==1)
		{	
			if( x >=72 && x<=73 )
			{
				char line12[250];
				strcpy(line12,line11);
				Insert1(line11,hashTable1,BucketSize,HashTableEntries1);
				Insert2(line12,hashTable2,BucketSize,HashTableEntries2);
			}
			else
			{
				printf(">wrong info in input for insert\n");
			}
		}
		else if(userCommand(line11)==2)
		{
				if(x == 32)
				{
					Delete(hashTable1,line11,HashTableEntries1);
				}
				else
				{
					printf(">wrong info in input for delete\n");
				}
		}
		else if(userCommand(line11)==3)
		{
				if( x == 19 || x == 31 || x==49)
				{
				char line13[250];
				strcpy(line13,line11);
				FindCaller(hashTable1,line11,line13,HashTableEntries1);	
				}
				else
				{
					printf(">wrong info in input for find\n");
				}
		} 
		else if(userCommand(line11)==4)
		{
				if( x == 21 || x == 33 || x==51)
				{
				char line14[250];
				strcpy(line14,line11);
				LookUp(hashTable2,line11,line14,HashTableEntries2);	
				}
				else
				{
					printf(">wrong info in input for lookup\n");
				}
		} 
		else if(userCommand(line11)==5)
		{
				if( x == 36 )
				{
					Indist(hashTable1,line11,HashTableEntries1);
				}
				else
				{
					printf(">wrong info in input for indist \n");
				}
		}
		else if(userCommand(line11)==6)
		{
				if( x == 22 )
				{
					TopDest(hashTable1,line11,HashTableEntries1,BucketSize);
				}
				else
				{
					printf(">wrong info in input for topdest\n");
				}
		}
		else if(userCommand(line11)==7)
		{
				if( x == -1 )
				{
					/*Topk(line11);*/
				}
				else
				{
					printf(">wrong info in input for top\n");
				}
		}
		else if(userCommand(line11)==8)
		{
			Bye(hashTable1,hashTable2,HashTableEntries1,HashTableEntries2);
		}
		else if(userCommand(line11)==9)
		{
			if( x == 16)
			{
				flag=1;
				Print(hashTable1,HashTableEntries1,flag);
			}
			else
			{
				printf(">wrong info in input for printf\n");
			}
		}
		else if(userCommand(line11)==10)
		{
			if( x == 16)
			{
				flag=2;
				Print(hashTable2,HashTableEntries2,flag);
			}
			else
			{
				printf(">wrong info in input for printf\n");
			}
		}
		else if(userCommand(line11)==11)
		{
					flag=1;
					char* ln;
					char ll[20];
					ln = strtok(line11," "); //first word
					ln = strtok(NULL," ");
					ln = strtok(NULL,"\n");//filename
					strcpy(ll,ln);
					FILE* fp=fopen(ll,"w");
					Dump(hashTable1,HashTableEntries1,fp,flag);
					printf("	Main---File [%s] Created\n",ll);
					fclose(fp);
		}
		else if(userCommand(line11)==12)
		{
					flag=2;
					char* ln1;
					char ll1[20];
					ln1 = strtok(line11," "); //first word
					ln1 = strtok(NULL," ");
					ln1 = strtok(NULL,"\n");//filename
					strcpy(ll1,ln1);
					FILE* fp=fopen(ll1,"w");
					Dump(hashTable2,HashTableEntries2,fp,flag);
					printf("	Main---File [%s] Created\n",ll1);
					fclose(fp);
		}
		else
		{
			printf("NOT AN AVAILABLE COMMAND \n");
		}
	} 
	free(input);
}		
	
	
	
	
	
				
