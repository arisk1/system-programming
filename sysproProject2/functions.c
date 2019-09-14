#include "functions.h"


int userCommand(char* input) //pairnei ena command apo to console kai analoga epistrefei enan arithmo
{
	char buf0[JBSIZE];
	strcpy(buf0,input);	
	char *command = strtok(buf0," ");
	
	if(strcmp("submit",command)==0)
		return 1;
	else if(strcmp("status",command)==0)
		return 2;
	else if(strcmp("status-all",command)==0)
		return 3;
	else if(strcmp("show-active",command)==0)
		return 4;
	else if(strcmp("show-pools",command)==0)
		return 5;
	else if(strcmp("show-finished",command)==0)
		return 6;
	else if(strcmp("suspend",command)==0)
		return 7;
	else if(strcmp("resume",command)==0)
		return 8;
	else if(strcmp("shutdown",command)==0)
		return 9;
	else
		return -1;
}

void createPool(int poolPid,int IDname , int poolfdin ,int poolfdout) //dimiourgei ti lista apo pool sthn ousia kathe pool pou 
{																		//dimiourgei mpainei sthn arxh ths listas
	poolInfo *poolBucket = malloc(sizeof(poolInfo));
	poolBucket->poolPid = poolPid;
	poolBucket->IDname = IDname;
	poolBucket->poolfdin = poolfdin;
	poolBucket->poolfdout = poolfdout;
	poolBucket->terminationFlag = 0;
	poolBucket->completeFlag = 0;
	poolBucket->ptr = listOfPools; //oste kathe kainourgio bucket
	listOfPools = poolBucket;	//na mpainei sthn arxh ths listas

}

void createJob(int jobPid,int IDname) //dimiourgei mia lista pao jobs kai kathe fora mpainie sthn arxh ths listas
{
	jobInfo *jobBucket = malloc(sizeof(jobInfo));
	jobBucket->jobPid = jobPid;
	jobBucket->IDname = IDname;
	jobBucket->status=0;
	jobBucket->ptr1 = listOfJobs;
	listOfJobs = jobBucket;
}

poolInfo* poolBucketFinderViaPid(int pidNum) //kanei anazitisi sth lista apo pool me PID
{
	poolInfo *assistant;
	assistant = listOfPools;
	while(assistant!=NULL)
	{
		if(assistant->poolPid==pidNum){return assistant;}
		assistant=assistant->ptr;
	}
	return assistant;
}

poolInfo* poolBucketFinderViaID(int pidNum) //kanei anazitisi sth lita me pool me ID
{
	poolInfo *assistant;
	assistant = listOfPools;
	while(assistant!=NULL)
	{
		if(assistant->IDname==pidNum){return assistant;}
		assistant=assistant->ptr;
	}
	return assistant;
}

jobInfo* jobBucketFinderViaPid(int pidNum) //kanei anazitisi sth lista me job me PID
{
	jobInfo *assistant;
	assistant = listOfJobs;
	while(assistant!=NULL)
	{
		if(assistant->jobPid==pidNum){return assistant;}
		assistant = assistant->ptr1;
	}
	return assistant;
}

jobInfo* jobBucketFinderViaID(int pidNum) //kanei anazitisi sth lista me job me ID
{
	jobInfo *assistant;
	assistant = listOfJobs;
	while(assistant!=NULL)
	{
		if(assistant->IDname==pidNum){return assistant;}
		assistant = assistant->ptr1;
	}
	return assistant;
}

void f(int sigflag) //kani handle to sigterm flag
{
	flagSigterm=1;

}

int findFinishedJobsInPool() //briksei ta finished job sto pool epistrefei 1 an exoun telosei oles kai einai = jobspool
{
	int doneJobs=0;
	jobInfo* assist2;
	assist2 = listOfJobs;	
	while(assist2!=NULL)	
	{
		if(assist2->status==0){ /*den exei teliosei|active*/return 0; }
		else if(assist2->status==2){/*den exei teliosei|suspended*/return 0;}
		else {doneJobs++;}
		assist2 = assist2->ptr1;
	}
	if(doneJobs==jobspool){return 1;}

}

void rqForPoolTermination() //stelnei to -5 gia termatismo tou pool kai diagrafei ta pipe tou
{
	poolInfo *assistant02;
	assistant02 = listOfPools;
	while(assistant02!=NULL)
	{
		if((assistant02->terminationFlag==1) && (assistant02->completeFlag == 0))
		{
			printf("jms_coord>Pool #%d requested termination\n",assistant02->IDname);
			//prepei na grapso to -5 sto poolfdout
			char aaa[20];
			int nwrite01;	
			sprintf(aaa,"%d",-5);
			if((nwrite01 = write(assistant02->poolfdout,aaa,strlen(aaa)+1)) == -1)
			{
				perror("error in writing for termination of pool ");
				exit(-17);	
			}
			char mirrorPipein[JBSIZE];
			char mirrorPipeout[JBSIZE];
			sprintf(mirrorPipein,"pipeforPoolin_%d",assistant02->IDname);
			sprintf(mirrorPipeout,"pipeforPoolout_%d",assistant02->IDname);
			unlink(mirrorPipein);
			unlink(mirrorPipeout);
			close(assistant02->poolfdin);
			close(assistant02->poolfdout);
			assistant02->completeFlag = 1;
		}
		assistant02 = assistant02->ptr;
	}
}

char* getTime(char* time00) //pairnei to current time kai to bazei se string
{
	struct tm *tm;
	time_t t;
	char str_time[100];
	//char str_date[100];

	t = time(NULL);
	tm = localtime(&t);

	strftime(str_time, sizeof(str_time), "%H%M%S", tm);
//	strftime(str_date, sizeof(str_date), "%d%m%Y", tm);
//	printf("time %s\n",str_time);
//	printf("date %s\n",str_date);
	strcpy(time00,str_time);
	return time00;
}

char* getDate(char* date00) //pairnei to current date kai to bazei se string
{
	struct tm *tm;
	time_t t;
	//char str_time[100];
	char str_date[100];

	t = time(NULL);
	tm = localtime(&t);

//	strftime(str_time, sizeof(str_time), "%H%M%S", tm);
	strftime(str_date, sizeof(str_date), "%d%m%Y", tm);
//	printf("time %s\n",str_time);
//	printf("date %s\n",str_date);
	strcpy(date00,str_date);
	return date00;
}

void submit(char* input) 
{
	printf("	input line %s\n",input);
	if(jobIDCounter % jobspool == 0 ) //mpainei sto 0 kai meta ana jobspool
	{	

		printf("	im creating the #%d Pool with the #%d Job\n",poolIDCounter,jobIDCounter);
		//dino sto new pool to new tou "onoma"
		poolID = poolIDCounter;
		char pipein[JBSIZE];
		char pipeout[JBSIZE];
		sprintf(pipein,"pipeforPoolin_%d",poolID);
		sprintf(pipeout,"pipeforPoolout_%d",poolID);
		if(mkfifo(pipein , PERMS1) == -1)
		{
			if(errno != EEXIST)
			{
				perror("mkfifo pooolin");
				exit(-2);
			}
		}
		if(mkfifo(pipeout , PERMS1) == -1)
		{
			if(errno != EEXIST)
			{
			perror("mkfifo poolout");
			exit(-4);
			}
		}

		int fdpipein;
		if( (fdpipein = open(pipein,O_RDONLY | O_NONBLOCK)) <0 ) //read coord		
		{
			perror("problem 01");
			exit(-1);
		}
		if((fdpipeoutpool = open(pipeout,O_RDONLY | O_NONBLOCK ))<0) //read to pool
		{
			perror("problem 04");
			exit(-1);
		}
		if((fdpipeinpool = open(pipein,O_WRONLY | O_NONBLOCK ))<0) //write to pool
		{
			perror("problem 03");
			exit(-1);
		}			

		int fdpipeout;	
		if((fdpipeout = open(pipeout,O_WRONLY|O_NONBLOCK)) <0 ) //write coord
		{
			perror("problem 02");
			exit(-1);
		}
						
		int poolPid = fork(); 
		if(poolPid!=0){printf("	pool pid %d \n",poolPid);}


	
		if(poolPid<0)
		{
			//stelno error
			printf("ERROR in fork from pool\n");
			exit(-1);
			
		}
		if(poolPid == 0)
		{
			sleep(1);
			printf("--------------------\n");
			printf("	child of pool \n");
			//child
			//kano close osa den xreiazontai
			//epidi t klironomei
			flagSigterm=0;
			signal(SIGTERM,f);
			close(fdpipein);
			close(fdpipeout);
			close(fdin);
			close(fdout);
			int len,rdresult;
			listOfJobs=NULL;
			int termFlag=0;
			char buf1[50];
			char buf2[JBSIZE];
			while(1)
			{
				//prospatho na do an exei kanei kati o coord
				//printf("eftasa:)");
			 	rdresult = read(fdpipeoutpool,buf1,3*sizeof(char));
				if(rdresult>0)
				{
					len = atoi(buf1);
					if(len>0)
					{
						printf("	cord sent a command with len: %d \n",len);
					}
					if(len == -5)//an exei steilei to coord termatismo
					{
						//diagrafo to listofjobs
						jobInfo *assistant1=listOfJobs;
						jobInfo *assistant2;
						while(assistant1 != NULL)
						{
							assistant2 = assistant1->ptr1;
							free(assistant1);
							assistant1 = assistant2;
						}
						close(fdpipeinpool);
						close(fdpipeoutpool);
						exit(1); 
					}
					while(((rdresult = read(fdpipeoutpool,buf2,len))== -1))
					{
						sleep(1);
					}
				 
					printf("	command:%s \n",buf2); 
					char help101[JBSIZE];
					strcpy(help101,buf2);
					char *command00 = strtok(help101," ");
					// h entolh brisketai sto buf2
 					//////////////////////////////////////
						/*     submit           */
                    ///////////////////////////////
					if(strcmp("submit",command00)==0)
					{
						char buf6[JBSIZE];
						//agnoo ta prota kena an iparxnoun
						int index=0; 
						if(buf2[0] == ' ')
						{		
							index=0;
							while(buf2[index]== ' '){index++;}
						}	
						//tora thelo na agnoiso kai to submit
						while(buf2[index]!=' '){index++;}
						char* line;
						line = &buf2[index];
						strcpy(buf6,line);
						//metrao tis lekseis tou line
						char helpbuf[100];
						strcpy(helpbuf,line);
						int words=0;
						char* z = strtok(helpbuf," ");
						int x=1;
						while(x==1)
						{
							if(z!=NULL)
							{
								words++;
								z=strtok(NULL," ");
							}
							else
							{
								x=0;
							}
						}
						
						printf("	command got %d words\n",words); 
						char** execArray = malloc((words+1)*sizeof(char*));						
						execArray[0] = strtok(buf6," "); //proto arg
						for(index=0;index<(words-1);index++)
						{
							execArray[1+index]=strtok(NULL," "); //vazo ta ipoloipa arg
						} 					
						int jobIDname = atoi(execArray[words-1]); //to teleutaio
						execArray[words-1] = NULL;
						
							 
						int jobID = fork();
						if(jobID < 0)
						{
							printf("ERROR in fork from job\n");
							exit(-1);
						} 
						if(jobID == 0)
						{
							//child - job
							printf("--------------------\n");
							printf("	child of job\n");
							int jobPid = getpid();
							char directory[JBSIZE];
							//get time
							char ctime[100];
							getTime(ctime);
							char cdate[100];
							getDate(cdate);
							sprintf(directory,"%s/sdi1400068_%d_%d_%s_%s",path,jobIDname,jobPid,cdate,ctime); 	
							if((mkdir(directory,PERMS2)==-1) && errno!=EEXIST)		
							{
								perror("error in creating directory");
								exit(-1);
							}
							
							printf("	made a directory-%s\n",directory);
						
							char fname[JBSIZE];
							//name tou arxeiou
							sprintf(fname,"%s/stdout",directory);
							creat(fname,PERMS1);
							//kano open gia writeonly
							int out = open(fname,O_WRONLY);
							//kano dup to filedescriptor sto stdout tou job
							dup2(out,1);
							close(out);
							//name tou arxeiou
							sprintf(fname,"%s/stderr",directory);
							creat(fname,PERMS1);
							int err = open(fname,O_WRONLY);
							dup2(err,2);
							close(err);
							if(execvp(execArray[0],execArray) < 0)
							{
								perror("error exec failed");
								exit(-1);
							}
						} 
						else
						{
							sleep(2);
							printf("--------------------\n");
							printf("	parent of job \n");
							char length3[20];
							char buf90[JBSIZE];
							int len3,nwrite1;
							//pateras - pool
							createJob(jobID,jobIDname);
							free(execArray);
							//to pool stelnei apanthsh 
							sprintf(buf90,"job Pid : [%d]",jobID);
							len3 = strlen(buf90)+1;
							sprintf(length3,"%d",len3);
							if((nwrite1 = write(fdpipeinpool,length3,strlen(length3)+1))<0)
							{
								perror("error in writing to pool in[job][submit]");
								exit(-8);	
							}
							if((nwrite1 = write(fdpipeinpool,buf90,len3)) <0)
							{
								perror("error in writing to pool in[job][submit]");
								exit(-6);	
							} 
							printf("	jobID written to coord: %s\n",buf90); 
							
						}
						
					} 
                   //////////////////////////////////////
						/*     status           */
                    ///////////////////////////////
					else if(strcmp("status",command00)==0)
					{
							
						char array1[JBSIZE];
						char array2[JBSIZE];
						strcpy(array1,input);
						char ch = ' ';
						char *ret;
						ret = strrchr(array1,ch);
						int jobID = atoi(ret);
						jobInfo* assistant2;
						assistant2 = jobBucketFinderViaID(jobID);
						if(assistant2->status == 0) //active
						{
							sprintf(array2,"JobID %d Status : Active",assistant2->IDname);
						}
						else if(assistant2->status == 1)// finished
						{
							sprintf(array2,"JobID %d Status : Finished",assistant2->IDname);
						}
						else if(assistant2->status == 2) //suspended
						{
							sprintf(array2,"JobID %d Status : Suspended",assistant2->IDname);
						}		  
						//kano write sto pipe
						char lengtharray2[10];
						int lenarray2 = strlen(array2)+1;
						sprintf(lengtharray2,"%d",lenarray2);
						int nwarray2;
						if((nwarray2 = write(fdpipeinpool,lengtharray2,strlen(lengtharray2)+1)) == -1)
						{
							perror("error in writing to pipe44");
							exit(-6);	
						}
						if((nwarray2 = write(fdpipeinpool,array2,lenarray2)) == -1)
						{
							perror("error in writing to pipe45");
							exit(-7);	
						}  
					}
					//////////////////////////////////////
						/*     status-all           */
                    //////////////////////////////////////
					else if(strcmp("status-all",command00)==0)
					{
						char array00[JBSIZE];
						jobInfo* assistant00 = listOfJobs;
						while(assistant00!=NULL)
						{
							if(assistant00->status == 0) //active
							{
								sprintf(array00,"JobID %d Status : Active",assistant00->IDname);
							}
							else if(assistant00->status == 1)// finished
							{
								sprintf(array00,"JobID %d Status : Finished",assistant00->IDname);
							}
							else if(assistant00->status == 2) //suspended
							{
								sprintf(array00,"JobID %d Status : Suspended",assistant00->IDname);
							}		  
							//kano write sto pipe
							char lengtharray2[10];
							int lenarray2 = strlen(array00)+1;
							sprintf(lengtharray2,"%d",lenarray2);
							int nwarray2;
							if((nwarray2 = write(fdpipeinpool,lengtharray2,strlen(lengtharray2)+1)) == -1)
							{
								perror("error in writing to pipe44");
								exit(-6);	
							}
							if((nwarray2 = write(fdpipeinpool,array00,lenarray2)) == -1)
							{
								perror("error in writing to pipe45");
								exit(-7);	
							}
							assistant00 = assistant00->ptr1;  
						}
						//meta grafo kai ena 0 gia na diloso pos teliosan ta minimata
						char length0[15];
						int nwarray00;
						sprintf(length0," %d",0);
						if((nwarray00 = write(fdpipeinpool,length0,strlen(length0)+1)) == -1)
						{
							perror("error in writing to console");
							exit(-6);	
						} 
					}	
					/////////////////////////////////////////////////////
					/*           showactive */
					///////////////////////////////////////////////////
					else if(strcmp("show-active",command00)==0)
					{
						char buf12[JBSIZE];	
						jobInfo* assistantZ;
						assistantZ = listOfJobs;
						int foundactive=0;
						while(assistantZ!=NULL)
						{
							if(assistantZ->status==0)
							{
								foundactive=1;
								sprintf(buf12,"JOBID %d:",assistantZ->IDname);
							}
							if(foundactive==1)
							{
								char length12[5];
								int len12 = strlen(buf12)+1;
								sprintf(length12," %d",len12);
								int nw12;
								if((nw12 = write(fdpipeinpool,length12,strlen(length12)+1)) == -1)
								{
									perror("error in writing to pipe44");
									exit(-6);	
								}
								if((nw12 = write(fdpipeinpool,buf12,len12)) == -1)	
								{
									perror("error in writing to pipe45");
									exit(-7);	
								}
							}
							assistantZ = assistantZ->ptr1;							
						}
						//an den vrhka kanena acrive job kano write 
						if(foundactive==0)
						{
							char buf13[JBSIZE];
							sprintf(buf13,"No Active Jobs Found");
							int len13 = strlen(buf13)+1;
							char length13[5];
							sprintf(length13,"%d",len13);
							int nw13;
							if((nw13 = write(fdpipeinpool,length13,strlen(length13)+1)) == -1)
							{
								perror("error in writing to pipe44");
								exit(-6);	
							}
							if((nw13 = write(fdpipeinpool,buf13,len13)) == -1)	
							{
								perror("error in writing to pipe45");
								exit(-7);	
							}
						}
						//meta grafo kai ena 0 gia na diloso pos teliosan ta minimata
						char length0[15];
						int nwarray00;
						sprintf(length0," %d",0);
						if((nwarray00 = write(fdpipeinpool,length0,strlen(length0)+1)) == -1)
						{
							perror("error in writing to console");
							exit(-6);	
						} 
					}
					/////////////////////////////////////////////////////////
						/*     showfinished       */
					/////////////////////////////////////////////////////////
					else if(strcmp("show-finished",command00)==0)
					{
						char buf12[JBSIZE];	
						jobInfo* assistantZ;
						assistantZ = listOfJobs;
						int foundfinished=0;
						while(assistantZ!=NULL)
						{
							if(assistantZ->status==1)
							{
								foundfinished=1;
								sprintf(buf12,"JOBID %d",assistantZ->IDname);
							}
							char length12[30];
							int len12 = strlen(buf12)+1;
							sprintf(length12," %d",len12);
							int nw12;
							if((nw12 = write(fdpipeinpool,length12,strlen(length12)+1)) == -1)
							{
								perror("error in writing to pipe44");
								exit(-6);	
							}
							if((nw12 = write(fdpipeinpool,buf12,len12)) == -1)	
							{
								perror("error in writing to pipe45");
								exit(-7);	
							}
							assistantZ = assistantZ->ptr1;							
						}
						//an den vrhka kanena finished job kano write 
						if(foundfinished==0)
						{
							char buf13[JBSIZE];
							sprintf(buf13,"No Finished Jobs Found");
							int len13 = strlen(buf13)+1;
							char length13[5];
							sprintf(length13,"%d",len13);
							int nw13;
							if((nw13 = write(fdpipeinpool,length13,strlen(length13)+1)) == -1)
							{
								perror("error in writing to pipe44");
								exit(-6);	
							}
							if((nw13 = write(fdpipeinpool,buf13,len13)) == -1)	
							{
								perror("error in writing to pipe45");
								exit(-7);	
							}
						}
						//meta grafo kai ena 0 gia na diloso pos teliosan ta minimata
						char length0[15];
						int nwarray00;
						sprintf(length0," %d",0);
						if((nwarray00 = write(fdpipeinpool,length0,strlen(length0)+1)) == -1)
						{
							perror("error in writing to console");
							exit(-6);	
						} 
					}
					//////////////////////////////////////////////////////
					/*			showPools			*/
					/////////////////////////////////////////////////////	
					else if(strcmp("show-pools",command00)==0)
					{
						char ena[JBSIZE];
						// h ipo ektelesi enotiles einai oi suspended kai active
						//ara psaxno gia autes mesa sto listOfJobs
						int numOfJobs=0;
						int foundActiveOrSuspended=0;
						jobInfo* assistantSP;
						assistantSP = listOfJobs;
						while(assistantSP!=NULL)
						{
							if(((assistantSP->status) == 0) || ((assistantSP->status)==2))
							{
								foundActiveOrSuspended=1;
								numOfJobs++;
							}
							assistantSP = assistantSP->ptr1;
						}	
						if(foundActiveOrSuspended==1)
						{//an brethikan jobs ta kano write
							sprintf(ena,"%d-%d",getpid(),numOfJobs);
							char lengthena[20];
							int lenena = strlen(ena)+1;
							if(lenena>=10){sprintf(lengthena,"%d",lenena);}
							else{sprintf(lengthena," %d",lenena);}
							int nwena;
							if((nwena = write(fdpipeinpool,lengthena,strlen(lengthena)+1)) == -1)
							{
								perror("error in writing to pipe show pools");
								exit(-6);	
							}
							if((nwena = write(fdpipeinpool,ena,lenena)) == -1)	
							{
								perror("error in writing to pipe show pools");
								exit(-7);	
							}	
						}
						else if(foundActiveOrSuspended==0)
						{//den brethikan jobs steile to sxetiko minima sto console
							sprintf(ena,"There is no ActiveOrSuspended Jobs");
							char lengthena[20];
							int lenena = strlen(ena)+1;
							if(lenena>=10){sprintf(lengthena,"%d",lenena);}
							else{sprintf(lengthena," %d",lenena);}
							int nwena;
							if((nwena = write(fdpipeinpool,lengthena,strlen(lengthena)+1)) == -1)
							{
								perror("error in writing to pipe show pools");
								exit(-6);	
							}
							if((nwena = write(fdpipeinpool,ena,lenena)) == -1)	
							{
								perror("error in writing to pipe show pools");
								exit(-7);	
							}
						}
						char length0[15];
						int nwarray00;
						sprintf(length0," %d",0);
						if((nwarray00 = write(fdpipeinpool,length0,strlen(length0)+1)) == -1)
						{
							perror("error in writing to console");
							exit(-6);	
						} 
						
					}
					/////////////////////////////////////////////////////////
						/* shutdown*/
					/////////////////////////////////////////////////////////
					else if(strcmp("shutdown",command00)==0)
					{
						int status=0;
						int jobsInProgress=0;
						jobInfo* assistantSD;
						assistantSD = listOfJobs;
						jobInfo* assistantSD2;
						while(assistantSD!=NULL)
						{
							if(assistantSD->status!=1)//!=finished
							{
								jobsInProgress++;
								kill(assistantSD->jobPid,SIGTERM);
								waitpid(assistantSD->jobPid,&status,0);//peirmeno na termatisei			
							}
							assistantSD2 = assistantSD->ptr1;
							free(assistantSD);
							assistantSD = assistantSD2;	
						}
						char lensd[10];
						int nwsd;
						if(jobsInProgress>=10){sprintf(lensd,"%d",jobsInProgress);}	
						else{sprintf(lensd," %d",jobsInProgress);}
						if((nwsd=write(fdpipeinpool,lensd,strlen(lensd)+1)) == -1)
						{
							perror("error in writing to console");
							exit(-6);	
						} 
						close(fdpipeinpool);
						close(fdpipeoutpool);
						exit(1);
					}
				} //end rdresult
				if(termFlag==0)
				{
					int doneJobPid,status;
					while(1)
					{
						doneJobPid = waitpid(-1,&status,WNOHANG);
						if(doneJobPid>0)
						{
							// dino sto status tou antistoixo jobBucker to 1		
							jobInfo* assist1;
							assist1 = jobBucketFinderViaPid(doneJobPid);
							assist1->status=1;
						}
						else {break;}
					}	
										
					if(findFinishedJobsInPool()==1)	//teliosan oles oi jobs
					{							//ara stelno -5
						printf("	Pool with pid [%d] requesting termination cause it completed all jobs \n",getpid());
						char lengthA[20];
						int nwriteA;
						sprintf(lengthA,"%d",-5);
						if((nwriteA = write(fdpipeinpool,lengthA,strlen(lengthA)+1)) == -1)					{
							perror("error in writing for termination of pool");
							exit(-15);	
						}
						termFlag = 1;
					}
				} 
				if(flagSigterm==1)
				{
					//stamatao ipoxreotika
					int status=0;
					int jobsInProgress=0;
					jobInfo* assistantSD;
					assistantSD = listOfJobs;
					jobInfo* assistantSD2;
					while(assistantSD!=NULL)
					{
						if(assistantSD->status!=1)//!=finished
						{
							jobsInProgress++;
							kill(assistantSD->jobPid,SIGTERM);
							waitpid(assistantSD->jobPid,&status,0);//peirmeno na termatisei				
						}
						assistantSD2 = assistantSD->ptr1;
						free(assistantSD);
						assistantSD = assistantSD2;	
					}
					char lensd[10];
					int nwsd;
					if(jobsInProgress>=10){sprintf(lensd,"%d",jobsInProgress);}	
					else{sprintf(lensd," %d",jobsInProgress);}
					if((nwsd=write(fdpipeinpool,lensd,strlen(lensd)+1)) == -1)
					{
						perror("error in writing to console");
						exit(-6);	
					} 
					close(fdpipeinpool);
					close(fdpipeoutpool);
					exit(1);
				} 
			} // end of while
		
		} 
 		close(fdpipeinpool);
		close(fdpipeoutpool);
		createPool(poolPid,poolID,fdpipein,fdpipeout);
		poolIDCounter++; //afou dimiourgisame pool anevazoume to counter 
	}
	//xrisimopoio pool sthn 1h thesi tis listas 
	// gia na tou doso tis entoles apo coord
	sprintf(input,"%s %d",input,jobIDCounter);	
	char buf3[JBSIZE];
	char buf4[20];
	char buf5[20];
	char buf22[50];
	int nwrite,nread,rdresult1;
	int len1 = strlen(input)+1;
	int len2;
	sprintf(buf4,"%d",len1); 
	//grafo entolh sto pipe out pool
	if((nwrite = write(listOfPools->poolfdout,buf4,strlen(buf4)+1)) == -1)
	{
		perror("error in writing to pool out1");
		exit(-6);	
	}
	if((nwrite = write(listOfPools->poolfdout,input,len1)) == -1)
	{
		perror("error in writing to pool out2");
		exit(-7);	
	}
	//pairno apannthsh sto pipe in pool
	sleep(2);
	while(((nread = read(listOfPools->poolfdin,buf5,3*sizeof(char))) == -1))
	{
		sleep(1);
	}
	len2 = atoi(buf5);
	if(len2 == -5)
	{	
		listOfPools->terminationFlag=1;
		if(((nread = read(listOfPools->poolfdin,buf5,3*sizeof(char))) == -1)&&errno!=EAGAIN)
		{
			perror("error in reading from pool in ");
			exit(-6);	
		}
		if(((nread = read(listOfPools->poolfdin,buf3,len2)) == -1)&&errno!=EAGAIN)
		{
			perror("error in reading from pool in");
			exit(-5);
				
		} 
		exit(1);

	}
	else 
	{
		while(((nread = read(listOfPools->poolfdin,buf3,len2)) == -1))
		{
			sleep(1);
		}
	}	
	if(strstr(buf3,"ERROR")!=NULL)
	{
	}
	else
	{
		jobIDCounter ++;
	}
	//tora prepei na steilo apanthsh sto console
	char length4[15];
	int nwrite4;
	int len6 = strlen(buf3)+1;
	sprintf(length4,"%d",len6);
	if((nwrite4 = write(fdout,length4,strlen(length4)+1)) == -1)
	{
		perror("error in writing to console");
		exit(-6);	
	}
	if((nwrite4 = write(fdout,buf3,len6)) == -1)
	{
		perror("error in writing to console");
		exit(-7);	
	} 
	//meta grafo kai ena 0 gia na diloso pos teliosan ta minimata
	char length0[15];
	sprintf(length0," %d",0);
	if((nwrite4 = write(fdout,length0,strlen(length0)+1)) == -1)
	{
		perror("error in writing to console");
		exit(-6);	
	}
	
}


void status(char* input)
{

	char array[JBSIZE];
	strcpy(array,input);
	char ch = ' ';
	char *ret;
	ret = strrchr(array,ch);
	int jobID = atoi(ret);
	printf("	looking for ...#%d... job \n",jobID);	 
	if(jobID<0 || jobID >=jobIDCounter)
	{
		//steile error sto console
		char aq[JBSIZE];
		sprintf(aq,"ERROR, invalid jobID %d[function:status]",jobID);
		char lengthaq[5];
		int lenaq = strlen(aq)+1;
		int nwaq;
		sprintf(lengthaq,"%d",lenaq);
		if((nwaq = write(fdout,lengthaq,strlen(lengthaq)+1)) == -1)
		{
			perror("error in writing to console from status");
			exit(-6);	
		}
		if((nwaq = write(fdout,aq,lenaq)) == -1)
		{
			perror("error in writing to console from status");
			exit(-7);	
		} 
		//kano write 0 sto console
		char length01[15];
		sprintf(length01," %d",0);
		if((nwaq = write(fdout,length01,strlen(length01)+1)) == -1)
		{
			perror("error in writing to console");
			exit(-6);	
		}
		return;
	}
	int poolidnam = jobID/jobspool; //gia na bro se poio pool anikei to job
	poolInfo* assistant;
	assistant = poolBucketFinderViaID(poolidnam);
	if(assistant->terminationFlag==1)
	{
		//tote exei termatisei to pool
		char aq[JBSIZE];
		char lengthaq[5];
		int nwaq;
		sprintf(aq,"job with id %d is finished[function:status]",jobID);	
		int lenaq = strlen(aq)+1;	
		sprintf(lengthaq,"%d",lenaq);
		//printf("[%s] - [%s]\n",lengthaq,aq);
		if((nwaq = write(fdout,lengthaq,strlen(lengthaq)+1)) == -1)
		{
			perror("error in writing to console from status");
			exit(-6);	
		}
		if((nwaq = write(fdout,aq,lenaq)) == -1)
		{
			perror("error in writing to console from status");
			exit(-7);	
		} 
		//kano write to 0
		char length02[15];
		sprintf(length02," %d",0);
		if((nwaq = write(fdout,length02,strlen(length02)+1)) == -1)
		{
			perror("error in writing to console");
			exit(-6);	
		}
	}
	else
	{
		//to pool den exei termatisei
		char aw[JBSIZE];
		char ae[JBSIZE];
		strcpy(ae,input);
		char lengthae[5];
		int lenae = strlen(ae)+1;
		if(lenae>=10){sprintf(lengthae,"%d",lenae);}
		else{sprintf(lengthae," %d",lenae);} 
		int nwae,rdae;
		//grafo to status pou psaxno sto pool
		if((nwae = write(assistant->poolfdout,lengthae,strlen(lengthae)+1)) == -1)
		{
			perror("error in writing to console from status");
			exit(-6);	
		}
		if((nwae = write(assistant->poolfdout,ae,lenae)) == -1)
		{
			perror("error in writing to console from status");
			exit(-7);	
		} 		
		
		//prepei na kano read apanthsh apo pool 
		char buf5[10];	
		char buf3[JBSIZE];
		int len2;
		while(((rdae = read(listOfPools->poolfdin,buf5,3*sizeof(char))) == -1))
		{
			sleep(1);	
		}
		len2 = atoi(buf5);
		if(len2 == -5)
		{	
				listOfPools->terminationFlag=1;
			if(((rdae = read(listOfPools->poolfdin,buf5,3*sizeof(char))) == -1)&&errno!=EAGAIN)
			{
				perror("error in reading from pool in ");
				exit(-6);	
			}
			if(((rdae = read(listOfPools->poolfdin,buf3,len2)) == -1)&&errno!=EAGAIN)
			{
				perror("error in reading from pool in ");
				exit(-6);	
			} 
			exit(1);

		}
		else 
		{
			while(((rdae = read(listOfPools->poolfdin,buf3,len2)) == -1))
			{
				sleep(1);
			}
		}	
		//tora prepei na steilo apanthsh sto console
		char length4[15];
		int nwrite4;
		int len6 = strlen(buf3)+1;
		sprintf(length4,"%d",len6);
		if((nwrite4 = write(fdout,length4,strlen(length4)+1)) == -1)
		{
			perror("error in writing to console");
			exit(-6);	
		}
		if((nwrite4 = write(fdout,buf3,len6)) == -1)
		{
			perror("error in writing to console");
			exit(-7);	
		}  			
	} 
}

void statusAll(char* input)
{

	int rd;
	char at[JBSIZE];
	char ao[JBSIZE];
	char ap[JBSIZE];
	char lengthat[5];
	int lenat;
	int activepools=0;
	int apantiseis=0;
	poolInfo* assistant;
	poolInfo* assistant2;
	assistant = listOfPools;
	while(assistant!=NULL) //elenxo gia ta pool pou exoun teliosei
	{
		if(assistant->terminationFlag==0)//elenxo gia ta pool pou einia active
		{
			activepools++;
			//stelno thn erotisi mou sto pool
			//kano write sto assistant->fdpipeout
			char ae[JBSIZE];
			strcpy(ae,input);
			char lengthae[5];
			int lenae = strlen(ae)+1;
			if(lenae>=10){sprintf(lengthae,"%d",lenae);}
			else{sprintf(lengthae," %d",lenae);} 
			int nwae,rdae;
			//grafo to status pou psaxno sto pool
			if((nwae = write(assistant->poolfdout,lengthae,strlen(lengthae)+1)) == -1)	
			{
				perror("error in writing to pool from status all");
				exit(-6);	
			}
			if((nwae = write(assistant->poolfdout,ae,lenae)) == -1)
			{
				perror("error in writing to pool from status all");
				exit(-7);	
			} 
		}
		else
		{ //allios exoun termatisei
			int i;
			int poolID1 = assistant->IDname;
			for(i=0;i<jobspool;i++)
			{
				//toses jobs(jobspool) eixei to kathe teliomeno pool
				// gia kathe pool poou termatise to jobid 
				//kimenetai apo 0 mexri jobspool-1
				int jobID = (poolID1*jobspool)+i;
				sprintf(at,"JobID %d Status : Finished",jobID);
				//stelno apanthsh sto console
				char length4[15];
				int nwrite4;
				int len6 = strlen(at)+1;
				sprintf(length4,"%d",len6);
				if((nwrite4 = write(fdout,length4,strlen(length4)+1)) == -1)
				{
					perror("error in writing to console");
					exit(-6);	
				}
				if((nwrite4 = write(fdout,at,len6)) == -1)
				{
					perror("error in writing to console");
					exit(-7);	
				}  
			}
		}
		assistant = assistant->ptr;
	}//end of while
	//mazeuo apanthsei kai stelno sto console
	while(apantiseis<activepools)			
	{
		assistant2 = listOfPools;
		while(assistant2!=NULL)
		{
			//dokimazo an exei steilei kati
			rd = read(assistant2->poolfdin,lengthat,3*sizeof(char));
			if(rd>0)
			{
				lenat = atoi(lengthat);
				 if(lenat == -5)
				{
					//to pool exei kanei request gia termination
					assistant2->terminationFlag = 1;
				}
				else if(lenat == 0)
				{
					apantiseis++;
				}
				else
				{
					//kano read auto pou esteile 
					while(((rd = read(assistant2->poolfdin,ao,lenat))== -1))
					{
						sleep(1);						
					}
					strcpy(ap,ao);
					//kai kano write thn apanthsh sto console
					char lengthap[15];
					int nwriteap;
					int lenap = strlen(ap)+1;
					sprintf(lengthap,"%d",lenap);
					if((nwriteap = write(fdout,lengthap,strlen(lengthap)+1)) == -1)
					{
						perror("error in writing to console status all");
						exit(-6);	
					}
					if((nwriteap = write(fdout,ap,lenap)) == -1)
					{
						perror("error in writing to console status all ");
						exit(-7);	
					}  			
				}
			}
			else if(rd == 0)
			{					
				apantiseis++; //gia na min exei monimo while
			}
			assistant2 = assistant2->ptr;
		}
	}	
	//meta grafo kai ena 0 gia na diloso pos teliosan ta minimata
		char length04[15];
		int nw04;
		sprintf(length04," %d",0);
		if((nw04 = write(fdout,length04,strlen(length04)+1)) == -1)
		{
			perror("error in writing to console");
			exit(-6);	
		}
	
	
}

void showActive(char* input)
{
	char one[100];
	sprintf(one,"Active Jobs :");
	//to kano write sto console
	char lengthone[5];
	int lenone;
	int activepools=0;
	lenone = strlen(one)+1;	
	sprintf(lengthone,"%d",lenone);
	int nwone;	

	if((nwone = write(fdout,lengthone,strlen(lengthone)+1)) == -1)
	{
		perror("error in writing to console showactive");
		exit(-6);	
	}
	if((nwone = write(fdout,one,lenone)) == -1)
	{
		perror("error in writing to console showactive ");
		exit(-7);	
	}  		
	poolInfo *assistant;
	assistant = listOfPools;
	//an exei termatisei to pool einai finished ta job sigoura
	while(assistant!=NULL)
	{
		if(assistant->terminationFlag==0)
		{
			activepools++;
			//stelno erotisi sto pool efoson einai active
			int nwae;
			char two[JBSIZE];
			strcpy(two,input);
			char lengthtwo[5];
			int lentwo = strlen(two)+1;
			sprintf(lengthtwo,"%d",lentwo);
			if((nwae = write(assistant->poolfdout,lengthtwo,strlen(lengthtwo)+1)) == -1)	
			{
				perror("error in writing to pool from show active");
				exit(-6);	
			}
			if((nwae = write(assistant->poolfdout,two,lentwo)) == -1)
			{
				perror("error in writing to pool from show active");
				exit(-7);	
			} 
		}
		assistant = assistant -> ptr;
	}
	int apantiseis=0;
	poolInfo* assistant2;
	char at[JBSIZE];
	char ao[JBSIZE];
	char ap[JBSIZE];
	char lengthat[5];
	int lenat;
	int rd;	
	while(apantiseis<activepools)			
	{
		assistant2 = listOfPools;
		while(assistant2!=NULL)
		{
			//dokimazo an exei steilei kati
			rd = read(assistant2->poolfdin,lengthat,3*sizeof(char));
			if(rd>0)
			{
				lenat = atoi(lengthat);
				if(lenat == -5)
				{
					//to pool exei kanei request gia termination
					assistant2->terminationFlag = 1;
				}
				else if(lenat == 0)
				{
					apantiseis++;
				}
				else if(lenat == 21)
				{
					//tote dene xei vrei active jobs kane read kai wrtie minima kai steilo sto console
					while(((rd = read(assistant2->poolfdin,ao,lenat))== -1))
					{
						sleep(1);						
					}
					strcpy(ap,ao);
					//kai kano write thn apanthsh sto console
					char lengthap[15];
					int nwriteap;
					int lenap = strlen(ap)+1;
					sprintf(lengthap,"%d",lenap);
					if((nwriteap = write(fdout,lengthap,strlen(lengthap)+1)) == -1)
					{
						perror("error in writing to console status all");
						exit(-6);	
					}
					if((nwriteap = write(fdout,ap,lenap)) == -1)
					{
						perror("error in writing to console status all ");
						exit(-7);	
					}  		
				}
				else
				{
					//kano read auto pou esteile 
					while(((rd = read(assistant2->poolfdin,ao,lenat))== -1))
					{
						sleep(1);						
					}
					strcpy(ap,ao);
				//	if(strcmp(" 0",ap)==0){continue;}
					//kai kano write thn apanthsh sto console
					char lengthap[15];
					int nwriteap;
					int lenap = strlen(ap)+1;
					if(lenap>=10){sprintf(lengthap,"%d",lenap);}
					else{sprintf(lengthap," %d",lenap);}
					if((nwriteap = write(fdout,lengthap,strlen(lengthap)+1)) == -1)
					{
						perror("error in writing to console status all");
						exit(-6);	
					}
					if((nwriteap = write(fdout,ap,lenap)) == -1)
					{
						perror("error in writing to console status all ");
						exit(-7);	
					}  			
				}
			}
			else if(rd == 0)
			{					//an den exo diabasei epeidh exei kleisei to pipe
				apantiseis++; //gia na min exei monimo while
			}
			assistant2 = assistant2->ptr;
		}
	}	
	//meta grafo kai ena 0 gia na diloso pos teliosan ta minimata
		char length04[15];
		int nw04;
		sprintf(length04," %d",0);
		if((nw04 = write(fdout,length04,strlen(length04)+1)) == -1)
		{
			perror("error in writing to console");
			exit(-6);	
		} 


}

void showFinished(char* input)
{

	char one[100];
	sprintf(one,"Finished Jobs :");
	//to kano write sto console
	char lengthone[5];
	int lenone;
	int activepools=0;
	lenone = strlen(one)+1;	
	sprintf(lengthone,"%d",lenone);
	int nwone;	

	if((nwone = write(fdout,lengthone,strlen(lengthone)+1)) == -1)
	{
		perror("error in writing to console showactive");
		exit(-6);	
	}
	if((nwone = write(fdout,one,lenone)) == -1)
	{
		perror("error in writing to console showactive ");
		exit(-7);	
	}  		
	poolInfo *assistant;
	assistant = listOfPools;
	//an exei termatisei to pool einai finished ta job sigoura
	while(assistant!=NULL)
	{
		if(assistant->terminationFlag==0) //active
		{
			activepools++;
			//stelno erotisi sto pool efoson einai active
			int nwae;
			char two[JBSIZE];
			strcpy(two,input);
			char lengthtwo[5];
			int lentwo = strlen(two)+1;
			sprintf(lengthtwo,"%d",lentwo);
			if((nwae = write(assistant->poolfdout,lengthtwo,strlen(lengthtwo)+1)) == -1)	
			{
				perror("error in writing to pool from show active");
				exit(-6);	
			}
			if((nwae = write(assistant->poolfdout,two,lentwo)) == -1)
			{
				perror("error in writing to pool from show active");
				exit(-7);	
			} 
		}
		else //exei zitisei termatismo
		{
			int jobID = assistant->IDname;
			int i;
			char three[JBSIZE];
			for(i=0;i<jobspool;i++)
			{
				int jobidprint = (jobID*jobspool+i);
				sprintf(three,"JobID %d",jobidprint);
				//kia kano write sth console
				char lengthap[15];
				int nwritethree;
				int lenap = strlen(three)+1;
				sprintf(lengthap," %d",lenap);
				if((nwritethree = write(fdout,lengthap,strlen(lengthap)+1)) == -1)
				{
					perror("error in writing to console status all");
					exit(-6);	
				}
				if((nwritethree = write(fdout,three,lenap)) == -1)
				{
					perror("error in writing to console status all ");
					exit(-7);	
				}  		
				
			}
		}
		assistant = assistant -> ptr;
	}
	int apantiseis=0;
	poolInfo* assistant2;
	char at[JBSIZE];
	char ao[JBSIZE];
	char ap[JBSIZE];
	char lengthat[5];
	int lenat;
	int rd;	
	while(apantiseis<activepools)			
	{
		assistant2 = listOfPools;
		while(assistant2!=NULL)
		{
			//dokimazo an exei steilei kati
			rd = read(assistant2->poolfdin,lengthat,3*sizeof(char));
			if(rd>0)
			{
			
				lenat = atoi(lengthat);
				if(lenat == -5)
				{
					//to pool exei kanei request gia termination
					assistant2->terminationFlag = 1;
				}
				else if(lenat == 0)
				{
					apantiseis++;
				}
				else if(lenat == 23)
				{
					//tote dene xei finished  jobs kane read kai wrtie minima kai steilo sto console
					while(((rd = read(assistant2->poolfdin,ao,lenat))== -1))
					{
						sleep(1);						
					}
					strcpy(ap,ao);
					//kai kano write thn apanthsh sto console
					char lengthap[15];
					int nwriteap;
					int lenap = strlen(ap)+1;
					sprintf(lengthap,"%d",lenap);
					if((nwriteap = write(fdout,lengthap,strlen(lengthap)+1)) == -1)
					{
						perror("error in writing to console status all");
						exit(-6);	
					}
					if((nwriteap = write(fdout,ap,lenap)) == -1)
					{
						perror("error in writing to console status all ");
						exit(-7);	
					}  		
				}
				else
				{
					//kano read auto pou esteile 
					while(((rd = read(assistant2->poolfdin,ao,lenat))== -1))
					{
						sleep(1);						
					}
					strcpy(ap,ao);
				//	if(strcmp(" 0",ap)==0){continue;}
					//kai kano write thn apanthsh sto console
					char lengthap[15];
					int nwriteap;
					int lenap = strlen(ap)+1;
					if(lenap>=10){sprintf(lengthap,"%d",lenap);}
					else{sprintf(lengthap," %d",lenap);}
					if((nwriteap = write(fdout,lengthap,strlen(lengthap)+1)) == -1)
					{
						perror("error in writing to console status all");
						exit(-6);	
					}
					if((nwriteap = write(fdout,ap,lenap)) == -1)
					{
						perror("error in writing to console status all ");
						exit(-7);	
					}  			
				}
			}
			else if(rd == 0)
			{					//an den exo diabasei epeidh exei kleisei to pipe
				apantiseis++; //gia na min exei monimo while
			}
			assistant2 = assistant2->ptr;
		}
	}	
	//meta grafo kai ena 0 gia na diloso pos teliosan ta minimata
		char length04[15];
		int nw04;
		sprintf(length04," %d",0);
		if((nw04 = write(fdout,length04,strlen(length04)+1)) == -1)
		{
			perror("error in writing to console");
			exit(-6);	
		} 
}


void showPools(char *input)
{

	char one[100];
	sprintf(one,"Pools & NumOfJobs :");
	//to kano write sto console
	char lengthone[5];
	int lenone;
	int activepools=0;
	lenone = strlen(one)+1;	
	sprintf(lengthone,"%d",lenone);
	int nwone;	

	if((nwone = write(fdout,lengthone,strlen(lengthone)+1)) == -1)
	{
		perror("error in writing to console showactive");
		exit(-6);	
	}
	if((nwone = write(fdout,one,lenone)) == -1)
	{
		perror("error in writing to console showactive ");
		exit(-7);	
	}  		
	poolInfo *assistant;
	assistant = listOfPools;
	while(assistant!=NULL)
	{
		if(assistant->terminationFlag==0) //active
		{
			activepools++;
			//stelno erotisi sto pool efoson einai active
			int nwae;
			char two[JBSIZE];
			strcpy(two,input);
			char lengthtwo[5];
			int lentwo = strlen(two)+1;
			sprintf(lengthtwo,"%d",lentwo);
			if((nwae = write(assistant->poolfdout,lengthtwo,strlen(lengthtwo)+1)) == -1)	
			{
				perror("error in writing to pool from show active");
				exit(-6);	
			}
			if((nwae = write(assistant->poolfdout,two,lentwo)) == -1)
			{
				perror("error in writing to pool from show active");
				exit(-7);	
			} 
		}
		assistant = assistant->ptr;
	}
	int apantiseis=0;
	poolInfo* assistant2;
	char at[JBSIZE];
	char ao[JBSIZE];
	char ap[JBSIZE];
	char lengthat[5];
	int lenat;
	int rd;	
	while(apantiseis<activepools)			
	{
		assistant2 = listOfPools;
		while(assistant2!=NULL)
		{
			//dokimazo an exei steilei kati
			rd = read(assistant2->poolfdin,lengthat,3*sizeof(char));
			if(rd>0)
			{
			
				lenat = atoi(lengthat);
				if(lenat == -5)
				{
					//to pool exei kanei request gia termination
					assistant2->terminationFlag = 1;
				}
				else if(lenat == 0)
				{
					apantiseis++;
				}
				else if(lenat == 35)
				{
					//tote dene xei finished  jobs kane read kai wrtie minima kai steilo sto console
					while(((rd = read(assistant2->poolfdin,ao,lenat))== -1))
					{
						sleep(1);						
					}
					strcpy(ap,ao);
					//kai kano write thn apanthsh sto console
					char lengthap[15];
					int nwriteap;
					int lenap = strlen(ap)+1;
					sprintf(lengthap,"%d",lenap);
					if((nwriteap = write(fdout,lengthap,strlen(lengthap)+1)) == -1)
					{
						perror("error in writing to console status all");
						exit(-6);	
					}
					if((nwriteap = write(fdout,ap,lenap)) == -1)
					{
						perror("error in writing to console status all ");
						exit(-7);	
					}  		
				}
				else
				{
					//kano read auto pou esteile 
					while(((rd = read(assistant2->poolfdin,ao,lenat))== -1))
					{
						sleep(1);						
					}
					strcpy(ap,ao);
				//	if(strcmp(" 0",ap)==0){continue;}
					//kai kano write thn apanthsh sto console
					char lengthap[15];
					int nwriteap;
					int lenap = strlen(ap)+1;
					if(lenap>=10){sprintf(lengthap,"%d",lenap);}
					else{sprintf(lengthap," %d",lenap);}
					if((nwriteap = write(fdout,lengthap,strlen(lengthap)+1)) == -1)
					{
						perror("error in writing to console status all");
						exit(-6);	
					}
					if((nwriteap = write(fdout,ap,lenap)) == -1)
					{
						perror("error in writing to console status all ");
						exit(-7);	
					}  			
				}
			}
			else if(rd == 0)
			{					//an den exo diabasei epeidh exei kleisei to pipe
				apantiseis++; //gia na min exei monimo while
			}
			assistant2 = assistant2->ptr;
		}
	}	
	//meta grafo kai ena 0 gia na diloso pos teliosan ta minimata
		char length04[15];
		int nw04;
		sprintf(length04," %d",0);
		if((nw04 = write(fdout,length04,strlen(length04)+1)) == -1)
		{
			perror("error in writing to console");
			exit(-6);	
		} 
}

void shutDown(char *input)
{
	char sd[JBSIZE];
	char lengthsd[5];
	int status;
	int jobsInProgress=0;
	poolInfo* assistant;
	assistant = listOfPools;
	poolInfo* assistant2;
	while(assistant!=NULL)
	{
		//prepei gia kathe pool den exei stamahsei na steilo signal
		if(assistant->completeFlag==0)
		{
			kill(assistant->poolPid,SIGTERM);
			int rd = read(assistant->poolfdin,lengthsd,3*sizeof(char));
			int lensd = atoi(lengthsd);
			if(lensd == -5)//to pool eixe steiel pos thelei na termatisei
			{
				rd = read(assistant->poolfdin,lengthsd,3*sizeof(char));
				lensd = atoi(lengthsd);
			}	
			waitpid(assistant->poolPid,&status,0); //perimene na termatiseit o pool
			jobsInProgress=jobsInProgress+lensd;
			close(assistant->poolfdin);
			close(assistant->poolfdout);
			char mirrorPoolfdin[JBSIZE];
			char mirrorPoolfdout[JBSIZE];
			sprintf(mirrorPoolfdin,"pipeforPoolin_%d",assistant->IDname);
			sprintf(mirrorPoolfdout,"pipeforPoolout_%d",assistant->IDname);
			unlink(mirrorPoolfdin);
			unlink(mirrorPoolfdout); 
		}
		assistant2 = assistant->ptr;//pairno ton epomeno 
		free(assistant); //eleytherono ayton
		assistant=assistant2;
	}
	char apant[JBSIZE];
	sprintf(apant,"Served %d jobs , %d were still in progress",jobIDCounter,jobsInProgress);
	//stelno apanthsh sto console
	char lengthap[15];
	int nwriteap;
	int lenap = strlen(apant)+1;
	if(lenap>=10){sprintf(lengthap,"%d",lenap);}
	else{sprintf(lengthap," %d",lenap);}
	if((nwriteap = write(fdout,lengthap,strlen(lengthap)+1)) == -1)
	{
		perror("error in writing to console status all");
		exit(-6);	
	}
	if((nwriteap = write(fdout,apant,lenap)) == -1)
	{
		perror("error in writing to console status all ");
		exit(-7);	
	} 
	//meta grafo kai ena 0 gia na diloso pos teliosan ta minimata
	char length04[15];
	int nw04;
	sprintf(length04," %d",0);
	if((nw04 = write(fdout,length04,strlen(length04)+1)) == -1)
	{
		perror("error in writing to console");
		exit(-6);	
	} 
//	exit(1);
}















