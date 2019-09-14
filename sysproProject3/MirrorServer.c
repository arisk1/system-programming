#include "msFunctions.h"


bool writer = false; 
int reader = 0;
int numDevicesDone=0;
int numOfFilesTr=0; //number of files transfered
long int sumOfBytesSent=0; //sum of sent bytes

void *mirrorManager(void *argp)
{
	mmInfo *my_data = (mmInfo*)argp;
    struct sockaddr_in  servadd; /* The address of server */
    struct hostent *hp;           /* to resolve server ip */
    int    sock, n_read;     /* socket and message length */
    char   buffer[10024];        /* to receive message */
    char lbuffer[1024];
	char helper[10024];
    
    /* Step 1: Get a socket */
	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1 )
	{
		perror_exit( "socket" );
	}
    /* Step 2: lookup server's address and connect there */
	if ((hp = gethostbyname(my_data->mmAddress)) == NULL) 
	{
		herror("gethostbyname"); exit(1);
	}
	memcpy(&servadd.sin_addr, hp->h_addr, hp->h_length);
	servadd.sin_port = htons(my_data->mmPort); /* set port number */
	servadd.sin_family = AF_INET ;     /* set socket type */
	if (connect(sock, (struct sockaddr*) &servadd, sizeof(servadd)) !=0)
	{
		perror_exit( "connect" );
	}
	/* Step 3: send LIST request  */
	sprintf(lbuffer,"LIST %d %d %d",my_data->mmPort,my_data->mmDelay,my_data->IDname);
	if ( write_all(sock, lbuffer, strlen(lbuffer)) == -1)
	{
		perror_exit("write");
	}
	if ( write_all(sock, "\n", 1) == -1 )
	{
		perror_exit("write");
	} 
    /* Step 4: read back results and send them to stdout */
	while( (n_read = read(sock, buffer, 10024)) > 0 )
	{   	
		sprintf(helper+strlen(helper),"%s",buffer);
	/*	if (write_all(STDOUT_FILENO, buffer, n_read)<n_read)
		{
			perror_exit("fwrite");
		} */ 
	}
	//we sent the string to helper.
	int hcount=0;
	int hi=0;
	while(helper[hi]!='\0')
	{
		if(helper[hi]=='-')
		{
			hcount++;
		}hi++;
	}
	//fitaxoume ena array me strings kai vazoume to kathe path
	char harray[hcount][1000];
	int hy=0;
	char *helpEntry;
	char *helpPtr;
	helpEntry = strtok_r(helper,"-",&helpPtr); //proti path
	strcpy(harray[hy],helpEntry);
	hy++;
	while((helpEntry = strtok_r(NULL,"-",&helpPtr))!=NULL)
	{
		strcpy(harray[hy],helpEntry);
		hy++;
	}
	char ch = '/';
	char *ret;
	int z;
	int fflag=0;
	char requestedPath[1000];
	for(z=0;z<hcount;z++)
	{
		ret  = strrchr(harray[z],ch);
		if (ret && *(ret + 1))
		{
			if(strcmp(ret+1,my_data->mmDirOrFile)==0)
			{
				strcpy(requestedPath,harray[z]);
				fflag=1;
				break;
			}
		}
	}
	char stringToBuffer[500];
	if(fflag)
	{
			sprintf(stringToBuffer,"%s-%s-%d",requestedPath,my_data->mmAddress,my_data->mmPort);	

	//	printf("%s\n",stringToBuffer);
		createJInfo(my_data->IDname,my_data->mmDelay,requestedPath);
	}
	else
	{ 
		printf(">>item not found\n");
	}
	close(sock);
	//we got the string we want to write to the buffer!
	//writer
	pthread_mutex_lock(&mtx);
		while((reader>0 || writer )|| pool.count >= POOL_SIZE  )
		{
			pthread_cond_wait(&write_cond,&mtx);
		}
		writer= true; // 1 for true
	pthread_mutex_unlock(&mtx);
	//write data
	//place(&pool,stringToBuffer);
	printf("im writing\n");
	pthread_mutex_lock(&mtx);
		pool.end = (pool.end + 1) % POOL_SIZE;
		strcpy(pool.data[pool.end],stringToBuffer);
		pool.count++;
	pthread_mutex_unlock(&mtx);
	pthread_mutex_lock(&mtx);
		writer=false;	 // 0 for false
		pthread_cond_broadcast(&read_cond);
		pthread_cond_signal(&write_cond);
	pthread_mutex_unlock(&mtx);
	
	
	
	return NULL;
}


void *worker(void *argp)
{
	char* dirname;
	char line[500];
	dirname = (char*) argp;
   
	//reader
	while(1)
	{
		pthread_mutex_lock(&mtx);
			while(writer || reader>0 || pool.count <= 0 )
			{	
				pthread_cond_wait(&read_cond,&mtx);
			}
			reader++;
		pthread_mutex_unlock(&mtx);
		//read
		pthread_mutex_lock(&mtx);
			strcpy(line,pool.data[pool.start]);
			printf("gotta read>>%s\n",line);
			pool.start = (pool.start + 1) % POOL_SIZE;
			pool.count--;
		pthread_mutex_unlock(&mtx);
		pthread_mutex_lock(&mtx);
			reader--;
			if(reader==0)
			{
				pthread_cond_signal(&write_cond);
			}
		pthread_mutex_unlock(&mtx);
		//tora exoume to line kai to dirname 
		//gia kathe worker theloume ena pinaka me strings me treis theseis na xorisoume to line
		char whelp[3][10000];
		int wy=0;
		char *whelpEntry;
		char *whelpPtr;
		whelpEntry = strtok_r(line,"-",&whelpPtr); 
		strcpy(whelp[wy],whelpEntry);
		wy++;							//proti thesi exei dirorfilename
		while((whelpEntry = strtok_r(NULL,"-",&whelpPtr))!=NULL) //deuteri thesi contentServerAdress
		{							//trith thesi conntent Server Port
			strcpy(whelp[wy],whelpEntry);
			wy++;
		}
		//
		//find the request number of this cs

		csInfo * assd;
		assd = findFromCsList(whelp[1]);	

		// prepei na sindethei o worker sto katallhlo content server
		struct sockaddr_in  servadd; // The address of server 
    	struct hostent *hp;           // to resolve server ip 
    	int    sock, n_read,file_size,bytes_received;     // socket and message length 
    	char   buffer[10024];        // to receive message 
		char   abuffer[10024];  
    	char lbuffer[1024];
		char helper[10024];
		char buf[1024];
		ssize_t fsize;
		size_t nread=0;
		int total_bytes_read=0;
		ssize_t ret_val;
		ssize_t ret_val2;
		int remain_data = 0;
		int fd,fd1;
		FILE *received_file;
    	// Step 1: Get a socket 
		if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1 )
		{
			perror_exit( "socket" );
		}

    	// Step 2: lookup server's address and connect there 
		if ((hp = gethostbyname(whelp[1])) == NULL) 
		{
			herror("gethostbyname"); exit(1);
		}
		memcpy(&servadd.sin_addr, hp->h_addr, hp->h_length);
		servadd.sin_port = htons(atoi(whelp[2])); // set port number 
		servadd.sin_family = AF_INET ;     // set socket type 
		if (connect(sock, (struct sockaddr*) &servadd, sizeof(servadd)) !=0)
		{
			perror_exit( "connect" );
		}
		// Step 3: send FETCH request  
		//mkdir
		char directory[1000];
		sprintf(directory,"%s/%s_%s",dirname,whelp[1],whelp[2]);
		if(mkdir(directory,PERMS)== -1 && errno!=EEXIST)
		{
			perror("mkdir error");	
		}
		int justAnID=0;
		jInfo * assistant1;
		assistant1 = findFromList(whelp[0]);
		justAnID = assistant1 -> ID; 
		sprintf(lbuffer,"FETCH %s %d",whelp[0],justAnID);
		if ( write_all(sock, lbuffer, strlen(lbuffer)) == -1)
		{
			perror_exit("write");
		}
		if ( write_all(sock, "\n", 1) == -1 )
		{
			perror_exit("write");
		} 
		if(!isAdir(whelp[0]))//not a directory
		{
			printf("ggggggggggg>>$#>$>$#$>\n");
			//Step 4: receiving file size
			char buf11[1024];
			ret_val = recv(sock,buf11,1024,0);
			if(ret_val == -1)
			{
				printf("error receiving size ");
				close(sock);	
				exit(1);
			}
			char fpathname[500];
			char ch1 = '/';
			char *ret1;
			int len;
			ret1 = strrchr(whelp[0],ch1);
			sprintf(fpathname,"%s/%s",directory,ret1+1);
				
			//Step 5: getting the file
			fd = open(fpathname, O_CREAT | O_WRONLY,0644);
			FILE *fp;
			fp = NULL;
			fp = fopen(fpathname,"w+");
			int file_siz=atoi(buf11);
			if(fd<0)
			{
				perror("open file");
				exit(1);
			}		
			if(fp!=NULL)
			{		
				//fprintf(stdout,"IM HERE\n");
				int remaindata = file_siz;
				while(((len = recv(sock,buf,1024,0))>0) && (remaindata >0 ))
				{
					fwrite(buf,sizeof(char),len,fp);
					remaindata -= len;			
				}
				pthread_mutex_lock(&mutex2);
					numOfFilesTr++;
					sumOfBytesSent += file_siz;
					assd->numOfRequests--;
					if(assd->numOfRequests == 0)
					{
						numDevicesDone++;
					}
				pthread_mutex_unlock(&mutex2);
				fprintf(stdout,"Done receiving\n");
				fflush(stdout);
			 	fclose(fp);
				
 			} 
		}
		else //its a directory
		{
						
			struct dirent *pdirent;
			DIR *pdir;
			pdir = opendir(whelp[0]);
			int count=0;
			char hparray[1000][1000];
			while((pdirent = readdir(pdir))!=NULL)
				{
					if((strcmp (pdirent->d_name, "..")==0)||(strcmp (pdirent->d_name, ".") == 0)) 
						{
						}
						else
						{
							char hparrayline[500];
							sprintf(hparrayline,"%s/%s",whelp[0],pdirent->d_name);
							strcpy(hparray[count],hparrayline);
							count++;
						}	
				}
			char ch1 = '/';
			char *ret1;
			ret1 = strrchr(whelp[0],ch1);
			char directory1[1000];
			sprintf(directory1,"%s/%s",directory,ret1);
			if(mkdir(directory1,PERMS)== -1 && errno!=EEXIST) //creating the directory
			{
				perror("mkdir error");	
			} //so now we got a new path the directory1
			int ccount=0;
			while(ccount<count)
			{
				/*char buf111[1024];
				int ret_val1 = recv(sock,buf111,1024,0);
				//sleep(2);
				printf("MILAE KANE KATI %d|%s\n",ret_val1,buf111); 
				if(ret_val1 == -1)
				{
					printf("error receiving size ");
					close(sock);	
					exit(1);
				} */
				char buf1[100024];
				char buf12[1024];
				ssize_t len;
				ret_val2 = recv(sock,buf12,sizeof(buf12),0);
				if(ret_val2 == -1)
				{
					printf("error receiving size ");
					close(sock);	
					exit(1);
				}
				char fpathname1[500];
				char ch = '/';
				char *ret;
				ret = strrchr(hparray[ccount],ch);
				sprintf(fpathname1,"%s/%s",directory1,ret+1); 
				fd1 = open(fpathname1, O_CREAT | O_WRONLY,0644);
				
				//Step 5: getting the file
				FILE *fp1;
				fp1 = NULL;
				fp1 = fopen(fpathname1,"w+");
				int file_siz=atoi(buf12);
				if(fd<0)
				{
					perror("open file");
					exit(1);
				}		
				if(fp1!=NULL)
				{		
					int remaindata = file_siz;
					while(((len = recv(sock,buf1,sizeof(buf1),0))>0) && (remaindata >0 ))
					{
						fwrite(buf1,sizeof(char),len,fp1);
						remaindata -= len;		
						if(len<=sizeof(buf1)){break;}
					}
					pthread_mutex_lock(&mutex2);	
						numOfFilesTr++;
						sumOfBytesSent += file_siz;
					pthread_mutex_unlock(&mutex2);
					fprintf(stdout,"Done receiving\n");
					fflush(stdout);
				 	fclose(fp1);
					close(fd1); 
 				}     
				ccount++; 
			}
			pthread_mutex_lock(&mutex2);
				assd->numOfRequests--;
				if(assd->numOfRequests == 0)
				{
					numDevicesDone++;
				}	
			pthread_mutex_unlock(&mutex2);	
		}	
		
 	}	
	
}

int main(int argc ,char *argv[])
{
	int mesoMegethos=0; // bytesTr/filesTr
	int csNum=0;
	int port;
	int threadnum;
	char dirname[200];
	int parameters=1; //agnoow to onoma
	int next = 0;
	listOfPools=NULL;
	LOI = NULL;
	while(parameters<argc) 
	{
		if(strcmp(argv[parameters],"-m")==0)
		{
			strcpy(dirname,argv[parameters+1]);
			parameters++;
		//	printf("%s",hhost);
		}	
		else if(strcmp(argv[parameters],"-p")==0)	
		{
			port = atoi(argv[parameters+1]);
			parameters++;
		//	printf("%d",portnum);
		}
		else if((strcmp(argv[parameters],"-w")==0))
		{
			threadnum = atoi(argv[parameters+1]);
			parameters++;
		}
		else 
		{
			printf("wrong input");
			return -1;
		}
		parameters++;
	}
	int sock, connected, bytes_recieved , truee = 1;  
	char send_data [1024] , recv_data[1024];   
	char send_data1[1024];  
	size_t size;  
	struct sockaddr_in server_addr,client_addr;    
	int sin_size;
	   
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror_exit("Socket");
	}
	if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&truee,sizeof(int)) == -1)	
	{
		perror_exit("Setsockopt");
	}        
	server_addr.sin_family = AF_INET;         
	server_addr.sin_port = htons(port);     
	server_addr.sin_addr.s_addr = INADDR_ANY; 
	bzero(&(server_addr.sin_zero),8); 
	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
                                                                       == -1) 
	{
		perror_exit("Unable to bind");
	}
	if (listen(sock, 5) == -1)
	{
		perror_exit("Listen");
	}		
	printf("\nTCPServer Waiting for client on port %d",port);
	fflush(stdout);
		
	char input[50];     
	int thrCounter=0;
	int i=0;	
		
	sin_size = sizeof(struct sockaddr_in);
	connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);
	printf("\nI got a connection from (%s , %d)",
	inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
	printf("\nSEND |start| to begin : ");
	fgets(input,sizeof(input),stdin);
	sprintf(send_data,"%s",input);
	send_data[strlen(send_data)-1]='\0';
	send(connected, send_data,strlen(send_data), 0);  
	bytes_recieved = recv(connected,recv_data,1024,0);
    	recv_data[bytes_recieved] = '\0';  
	while(recv_data[i]!='\0')
	{
		if(recv_data[i]=='-')
		{
			thrCounter++;
		}
		i++;
	}
	printf("Number Of MirrorManagers >> %d\n",thrCounter);          
	//create an array with info of mm
	mmInfo mm[thrCounter];
	int h=0;
	char line1[500];
	strcpy(line1,recv_data);
	char *ena;
	ena = strtok(line1,"-");
	strcpy(mm[h].fullLine,ena);
	mm[h].IDname=h;
	h++;
	while((ena = strtok(NULL,"-")) != NULL)
	{
		strcpy(mm[h].fullLine,ena);
		mm[h].IDname=h;
		h++; 
	}
	int y=0;
	for(y=0;y<thrCounter;y++)
	{
		char line[1000];
		strcpy(line,mm[y].fullLine);
		char* entry;
		entry  = strtok(line,":");
		strcpy(mm[y].mmAddress,entry);
		int x=0;	
		while((entry = strtok(NULL,":")) != NULL)
		{
			if(x==0){mm[y].mmPort = atoi(entry); x++;}
			else if(x==1){strcpy(mm[y].mmDirOrFile,entry); x++;}
			else if(x==2){mm[y].mmDelay = atoi(entry); x++;}		
		}
	}
	int zzz=0;
	int sflag=0;
	for(zzz;zzz<thrCounter;zzz++)
	{
		int yyy=0;
		sflag=0;
		if(LOI!=NULL)
		{
			csInfo * ass2;
			ass2 = findFromCsList(mm[zzz].mmAddress);
			if(ass2 != NULL)
			{
				continue;
			}
			else
			{
				createCsInfo(mm[zzz].mmAddress);
			}
		} 
		else
		{ 
			createCsInfo(mm[zzz].mmAddress);
		} 
		for(yyy;yyy<thrCounter;yyy++)
		{
			if(strcmp(mm[zzz].mmAddress,mm[yyy].mmAddress)==0)	
			{
				sflag++;
			}
		} 
		if(sflag > 1)
		{
			csInfo * ass1;
			ass1 = findFromCsList(mm[zzz].mmAddress);
			ass1->numOfRequests += (sflag-1);
		} 	
	} 
	csInfo * kkk;
	kkk = LOI;
	while(kkk!=NULL)
	{
		csNum++;
		printf(">>> %d\n",kkk->numOfRequests);
		kkk = kkk->next;
	} 

	//info about content servers are in the array of structs
	//we shoud create as many trheads as the thrCounter
	initialize(&pool);
	pthread_mutex_init(&mtx,0);
	pthread_cond_init(&write_cond,0);
	pthread_cond_init(&read_cond,0);
	int err;
	pthread_t threads[thrCounter];
	pthread_t workers[threadnum];
	for(i=0;i<thrCounter;i++)
	{
		if(err = pthread_create(&threads[i],NULL,mirrorManager,&mm[i]))
		{
			perror2("pthread_create",err);
			exit(1);
		}
	}
	for(i=0;i<threadnum;i++)
	{
													
		if(err = pthread_create(&workers[i],NULL,worker,(void*)dirname))
		{
			perror2("pthread_create",err);
			exit(1);
		}
	}	
	for(i=0;i<thrCounter;i++)
	{
		if(err = pthread_join(threads[i],NULL))
		{
			perror2("pthread_create",err);
			exit(1);
		}
	}
	while(csNum!=numDevicesDone){}
	char lineToSend[100];
	mesoMegethos = sumOfBytesSent/numOfFilesTr;
	sprintf(lineToSend,"%d/%ld/%d",numOfFilesTr,sumOfBytesSent,mesoMegethos);
	strcpy(send_data1,lineToSend);
	send(connected, send_data1,strlen(send_data1), 0);  
	/*for(i=0;i<threadnum;i++)
	{
		if(err = pthread_join(workers[i],NULL))
		{
			perror2("pthread_create",err);
			exit(1);
		}
	} */

	pthread_cond_destroy(&write_cond);
	pthread_cond_destroy(&read_cond);
	pthread_mutex_destroy(&mtx);
	close(sock);
	return 0;
} 
