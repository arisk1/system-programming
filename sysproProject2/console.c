
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

#define JBSIZE 5000
#define PERMS1 0666
#define PERMS2 0777
////////////////////////////////////////


int main(int argc,char* argv[])
{
	char filename[80];
    size_t size=0;
	int fdin;
	int fdout;
	char jmsin[80];
	char jmsout[80];
	FILE *file = NULL;
	char jobbuff[JBSIZE+1];
	int notfile=0;
	int firstRead=0;
	int eofFirstRead=0;
	if(argc == 7)
	{
		for(int i=1;i<=5;i+=2) //me arxeio
		{
			if(strcmp("-o",argv[i])==0)
			{
				strcpy(filename,argv[i+1]);
				file = fopen(filename,"r");
				if(file == NULL)
				{
					printf("file not found.Give me an option \n");
				}
			}
			else if(strcmp("-w",argv[i])==0)
			{
				strcpy(jmsin,argv[i+1]);
			}
			else if(strcmp("-r",argv[i])==0)
			{	
				strcpy(jmsout,argv[i+1]);
			}
			else
			{
				printf("wrong argument\n");	
			}
		}
	}
	else if(argc == 5) //xoris arxeio
	{
		for(int i=1;i<=3;i+=2)
		{
			notfile = 1;
			if(strcmp("-w",argv[i])==0)
			{
				strcpy(jmsin,argv[i+1]);
			}
			else if(strcmp("-r",argv[i])==0)
			{	
				strcpy(jmsout,argv[i+1]);
			}
			else
			{
				printf("wrong argument\n");	
			}
		}
		
	}
	else
	{
		printf("Error-wrong number of arguments\n");
		exit(-1);
	}	
	if(jmsin == NULL || jmsout == NULL)
	{
		printf("you didint give names to named-pipes\n");
		return -1;
	}
	
	if((fdin = open(jmsin, O_WRONLY | O_NONBLOCK)) < 0 )
	{
		perror("fifo jmsin open problem");
		exit(-4);
	}
	if((fdout = open(jmsout, O_RDONLY | O_NONBLOCK ))< 0)
	{
		perror("fifo jmsout	problem");
		exit(-3);
	}
	int endOfFileFlag=0;
	int nread,nwrite,len;
	char buffer[JBSIZE];
	char leng[5];
	char a[5];
	char operationLine[30];
	char *command=NULL;
	char lengthfirst[10];
	sprintf(lengthfirst,"%d",99); //grafo th proti fora pou tha anoiksei o console ena tixaio arithmo (99)
	if( ((nwrite = write(fdin,lengthfirst,strlen(lengthfirst)+1))) == -1)
	{
		perror("cant write the first time");
		exit(-1);
	}
	while(1) //autot to while einai otan trexei entoles apo arxeio
	{
		if(file!=NULL) 
		{	
			
			while( ((nread= read(fdout,a,3*sizeof(char)))== -1))
			{
				firstRead=1;
				sleep(1);			
			}
			len= atoi(a);
			if(len > 0)
			{
					sleep(3);
					while( ((nread = read(fdout,buffer,len))== -1))
					{
						sleep(1);
					}
				if(strstr(buffer,"SEND")!=NULL){printf("\njms_console>%s \n",buffer);}
				if((strstr(buffer,"SEND")!=NULL))
				{		
					if(endOfFileFlag==0)
					{
						if(fgets(operationLine, sizeof(operationLine), file) != NULL) // an den parei eof steile entolh sto coord
						{
							if((strlen(operationLine)) > JBSIZE)
							{	
								printf("operation line too long \n");
								fflush(stdout);
								continue;
							}
							if(operationLine[strlen(operationLine)-1] == '\n')
							{
								operationLine[strlen(operationLine)-1] = '\0';
							}
							len = strlen(operationLine)+1;
							command = malloc(len*sizeof(char));
							strcpy(command,operationLine);
							printf("jms_console>Command given from file |%s| ",command);
							printf("(len) %d",len);
							printf("\n");
							if(len>=10){
							sprintf(leng,"%d",len);
							}
							else{sprintf(leng," %d",len);}
							if(operationLine == NULL)
							{
								printf("something is wrong \n");
								exit(-10);
							}
							else 
							{
								if((nwrite = write(fdin,leng,strlen(leng)+1))==-1)
								{
										perror("error in writing jmsin");
										exit(-1);
								}
								if((nwrite = write(fdin,command,len))==-1)
								{
										perror("error in writing jmsin");
										exit(-1);
								}
								
							}
						free(command);
						}
						else // an pareis eof apo to arxeio spase thn epanalipsi kai par entoles apo to ternimal
						{
							sprintf(leng,"%d",-4);
							if((nwrite = write(fdin,leng,strlen(leng)+1))==-1)
							{
									perror("error in writing jmsin");
									exit(-1);
							}
							eofFirstRead = 1;
							printf("jms_console> END OF INPUT FILE \n");
							break;
						}
					}
				}
				else if((strstr(buffer,"SHUTDOWN")!=NULL)) //an dexthei shutdown kleinei ta pipes kai termatizei
				{
					printf("jms_console>%s\n",buffer);
					sleep(3);
					close(fdin); //kleino kai ta pipe
					close(fdout);
					exit(1);
					
				}	
				else //an oxi send h shutdown.Simainei oti ektiponei ta apotelesmata pou dexetai apo to coord
				{
					printf("jms_console>%s\n",buffer);
				}
			}
			else if((len==0)&&(firstRead==1))
			{
				//printf("got the zero\n");
			}  
		}
		else if(file==NULL)	
		{
			break; //paei na dexthei enotles apo to terminal
		}	
	}//end of while 
	

	char *input;
	int firstTimeIn = 0;
	while(1)
	{		
			if(firstTimeIn==1||notfile==1) //an mpei proti fora meta apo file exei diabasei idi th SEND ara den xreiazetai na ksanadiabasei(firstTimeIN)
			{								// h an den ipirxe arxeio mpainei na diabasei kanonika th SEND(notfile)
				while( ((nread= read(fdout,a,3*sizeof(char)))== -1))
				{
					sleep(1);			
				}
				len= atoi(a);
				eofFirstRead=1;
			}			
			if((len > 0) || (firstTimeIn==0))
			{
					sleep(3);
					if((firstTimeIn==1)||(notfile==1))
					{
						while( ((nread = read(fdout,buffer,len))== -1))
						{
							sleep(1);
						}
					}
				if(strstr(buffer,"SEND")!=NULL){printf("\njms_console>%s \n",buffer);}
				if((strstr(buffer,"SEND")!=NULL))
				{		
					if(endOfFileFlag==0)
					{
						firstTimeIn=1;
						printf("Give me your Option\n");
						if(getline(&input,&size,stdin)!=-1) //diabazei entoles apo to terminal
						{
							if((strlen(input)) > JBSIZE)
							{	
								printf("operation line too long \n");
								fflush(stdout);
								continue;
							}
							if(input[strlen(input)-1] == '\n')
							{
								input[strlen(input)-1] = '\0';
							}
							len = strlen(input)+1;
							command = malloc(len*sizeof(char));
							strcpy(command,input);
							
							//printf("\n");
							printf("jms_console>Command given |%s| ",command);
							printf("(len) %d",len);
							printf("\n");
							if(len>=10){
							sprintf(leng,"%d",len);
							}
							else{sprintf(leng," %d",len);}
							if(operationLine == NULL)
							{
								printf("something is wrong \n");
								exit(-10);
							}
							else 
							{
								if((nwrite = write(fdin,leng,strlen(leng)+1))==-1)
								{
										perror("error in writing jmsin");
										exit(-1);
								}
								if((nwrite = write(fdin,command,len))==-1)
								{
										perror("error in writing jmsin");
										exit(-1);
								}
								
							}
						free(command);
						}
						else
						{
							if(feof(stdin)) //an diabasei apo to terminal CTRL+D termatizei to console
							{
								printf("Terminating Console \n");
								break;
							}
						}
					}
				}
				else if((strstr(buffer,"SHUTDOWN")!=NULL)) //shutdown
				{
					printf("jms_console>%s\n",buffer);
					sleep(3);
					close(fdin); //kleino kai ta pipe
					close(fdout);
					exit(1);
					
				}	
				else //an oxi send h shutdown
				{
					printf("jms_console>%s\n",buffer);
				}
			}
			else if((len==0) && (eofFirstRead==1))
			{
			//	printf("got the zero\n");
			}  
			
	}//end of while
	free(input);
     	
}

