
#include "functions.h"


int main(int argc,char* argv[])
{	
	signal(SIGPIPE,SIG_IGN); //ignore to sigpipe
	if(argc == 9)
	{
		for(int i=1;i<=7;i+=2)
		{
			if(strcmp("-l",argv[i])==0)
			{
				strcpy(path,argv[i+1]);
			}
			else if(strcmp("-w",argv[i])==0)
			{
				strcpy(jmsout,argv[i+1]);
			}
			else if(strcmp("-r",argv[i])==0)
			{	
				strcpy(jmsin,argv[i+1]);
			}
			else if(strcmp("-n",argv[i])==0)
			{
				jobspool = atoi(argv[i+1]);
			}
			else
			{
				printf("wrong argument\n");
				exit(-1);	
			}
		}
	}
	else
	{
		printf("Error-wrong number of arguments\n");
		exit(-1);
	}
	if(jmsin == NULL || jmsout == NULL || path ==NULL || jobspool < 0)
	{
		printf("you didint give right parameters \n");
	}
	if(mkfifo(jmsout , PERMS1) == -1)
	{
		if(errno != EEXIST)
		{
			perror("mkfifo jmsout");
			exit(-2);
		}
	}
	if(mkfifo(jmsin , PERMS1) == -1)
	{
		if(errno != EEXIST)
		{
			perror("mkfifo jmsin");
			exit(-3);
		}
	}
	if((fdin = open(jmsin, O_RDONLY | O_NONBLOCK )) < 0 )
	{
		perror("fifo jmsin open problem");
		exit(-4);
	}	
	
	char buffer[JBSIZE];
	char a[15];
	char b[100];
	char command[50];
	int nread,nwrite,len; //gia na diabasei apotelsema apo console
	int flag = 0;	
	int firsttime=0;
	poolIDCounter=0;
	jobIDCounter=0;
	listOfPools=NULL;
	while(1)
	{
		if( (nread=read(fdin,a,3*sizeof(char)) ) >0 )
		{
			int len1=atoi(a);
			if(firsttime==0)
			{
				if(len1==99) // diabazei to tixaio arithmo pou to esteile o console otan anoiske gia na anoiksei kai aito to jmsout gia write
				{
					//anoikse jmsout gia grapsimo
					if((fdout = open(jmsout,O_WRONLY | O_NONBLOCK)) <0)
					{
						perror("error in writing jmsout 66");
						exit(-5);
					}
					flag = 2; //otan mpainei proti fora paei kai stelnei to SEND sto console gia na pari entolh
				}
				firsttime=1;
			}
			else //if not first time
			{	
				len = atoi(a);
				if(len > 0)
				{	
					while( ((nread = read(fdin,b,len))==-1)  )
					{
						sleep(1);
					}	
					printf("\n");
					printf("jms_coord> Command from console [ %s ] len(%d) \n",b,len);
					strcpy(command,b);
					if(userCommand(command)==1)	
					{

						printf("submit >>>\n");
						submit(command);
						flag =2;		
					}
					else if(userCommand(command)==2)
					{
						printf("status >>>\n");
						status(command);
						flag =2;
					}
					else if(userCommand(command)==3)
					{
						printf("status-all >>>\n");
						statusAll(command);
						flag=2;
					}
					else if(userCommand(command)==4)
					{
						printf("show-active >>>\n");
						showActive(command);
						flag=2;
					}
					else if(userCommand(command)==5)
					{
						printf("show-pools >>>\n");
						showPools(command);
						flag=2;
					}
					else if(userCommand(command)==6)
					{
						printf("show-finished >>>\n");
						showFinished(command);
						flag=2;
					}
					else if(userCommand(command)==7)
					{
						printf("suspend >>>\n");
						flag=2;
					}
					else if(userCommand(command)==8)
					{
						printf("resume >>>\n");
						flag=2;
					}
					else if(userCommand(command)==9)
					{
						printf("shutdown >>> \n");
						shutDown(command);
						flag=10;
					}
					else
					{
						printf("Cant recognize this command\n");
						printf("Send again\n");
						flag=2;
					}	
				}
			}
			if(flag ==2  ) //opoiadipote entolh ektos apo shutdown
			{
				sleep(3);
				sprintf(a," %d",5);
				if((nwrite = write(fdout,a,3*sizeof(char))) == -1)
				{
					perror("error in writing jmsout");
					exit(-6);	
				}
				
				if((nwrite = write(fdout,"SEND",5*sizeof(char))) == -1)
				{
					perror("error in writing jmsout");
					exit(-7);	
				}
				flag=0;
			}
			else if(flag==10) //exei dothei shutdown kai stelnome sto console anti gia SEND , SHUTDOWN
			{
				sprintf(a," %d",9);
				if((nwrite = write(fdout,a,3*sizeof(char))) == -1)
				{
					perror("error in writing jmsout");
					exit(-6);	
				}
				
				if((nwrite = write(fdout,"SHUTDOWN",9*sizeof(char))) == -1)
				{
					perror("error in writing jmsout");
					exit(-7);	
				}
				exit(1);
			}
			poolInfo * assistant01;
			assistant01 = listOfPools;
			char buf20[20];
			char buf21[JBSIZE];
			int len20;
			while(assistant01!=NULL) //to pool sthn anamoni mporei na steilei mono enan arnitiko (-5) gia na termatisei
			{
				nread = read(assistant01->poolfdin,buf20,5*sizeof(char));
				if(nread>0)
				{
					len20 = atoi(buf20);
					if(len20 == -5)
					{		
						assistant01->terminationFlag=1;
						printf("	Pool #%d is terminating with terminationFlag = %d \n",assistant01->IDname,assistant01->terminationFlag);
					}
				}	
				assistant01 = assistant01->ptr;		
			}
			rqForPoolTermination(); //kai zitaei na termatisei diagrafontas ta pipes pou dimiourgithikan gia auto
		}
	}
	return 0;

}
