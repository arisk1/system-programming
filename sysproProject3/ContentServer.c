#include "csFunctions.h"

int main(int argc, char *argv[])
{
	listOfPools1=NULL;
	int port;
	char dirorfilename[200];
	int parameters=1; //agnoow to onoma
	while(parameters<argc) 
	{
		if(strcmp(argv[parameters],"-p")==0)	
		{
			port = atoi(argv[parameters+1]);
			parameters++;
		//	printf("%d",portnum);
		}
		else if((strcmp(argv[parameters],"-d")==0))
		{
			strcpy(dirorfilename,argv[parameters+1]);
			parameters++;
		}
		else 
		{
			printf("wrong input");
			return -1;
		}
		parameters++;
	}
	struct sockaddr_in myaddr,client_addr;  /* build our address here */
	int	c, lsock, csock;  /* listening and client sockets */
	FILE	*sock_fp;             /* stream for socket IO */
	char    request[1024];               
	char    buffer[10024];               /* for popen() */
	char    abuffer[10024];
	int sin_size;

    /** create a TCP a socket **/
	if ((lsock = socket( AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror_exit( "socket" );
	}
    /** bind address to socket. **/
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(port);/*port to bind socket*/
	myaddr.sin_family = AF_INET;  /* internet addr family */
	if(bind(lsock,(struct sockaddr *)&myaddr, sizeof(myaddr)))
	{
		perror_exit( "bind" );
	}
	/** listen for connections with Qsize=5 **/
	if ( listen(lsock, 5) != 0 )
	{
		perror_exit( "listen" );
	}
	printf("\nTCPServer Waiting for client on port %d",port);
	fflush(stdout);
			char p[1000][1000];
			char ar[10000];
			char ch1 = '/';
			char *ret1;
			ret1  = strrchr(dirorfilename,ch1);
			char ena1[100];
			strcpy(ena1,ret1);
			//printf("kollisa\n");
			int o1=1;	//first time			
			list_dir(dirorfilename,p);
			int z=0;
			int y=0;
			for(y=0;y<1000;y++)
			{
				if((strlen(p[y])!=0)&&( strstr(p[y],ena1)!=NULL ))
				{
					if(o1)
						{
							sprintf(ar,"%s",p[y]);
							ar[strlen(ar)]='-';	
							o1=0;
							continue;
						}
					sprintf(ar+strlen(ar),"%s",p[y]);
					ar[strlen(ar)]='-';				
				}
			}
	while ( 1 )
	{ 
		/* main loop: accept - read - write */
		/* accept connection, ignore client address */
		sin_size = sizeof(client_addr);
		if ( (csock = accept(lsock, (struct sockaddr *)&client_addr,&sin_size)) < 0 )
		{
			perror_exit("accept");
		}
		/* open socket as buffered stream */
		if ((sock_fp = fdopen(csock,"r+")) == NULL)
		{
			perror_exit("fdopen");
		}
		/* read request  */
		if (fgets(request, 1024, sock_fp) == NULL)
		{
			perror_exit("reading request");
		} 
		//LIST
		if(strstr(request,"LIST")!=NULL)
		{
			printf("\nrequest %s\n",request);
			//apothikeuo se mia lista to delay-id oxi to port pou einai to id tou server
			char * entry;
			char request1[500];
			strcpy(request1,request);
			entry = strtok(request1," ");		//LIST
			entry = strtok(NULL," "); //PORT
			entry = strtok(NULL," "); //DELAY
			char delay222[10];
			strcpy(delay222,entry);	
			entry = strtok(NULL, " "); //ID
			char id222[10];
			strcpy(id222,entry);
			createJInfo1(atoi(id222),atoi(delay222));
			strcpy(buffer,ar);
			write_all(csock,buffer,strlen(buffer));
		} 
		//FETCH
		else if(strstr(request,"FETCH")!=NULL)
		{
			printf("\nrequest %s\n",request);
			//perinoume to id pou steilame to trito arg
			char aplha[10];
			char request2[500];
			char path[500];
			strcpy(request2,request);
			char *entry1;
			entry1 = strtok(request2," "); //FETCH
			entry1 = strtok(NULL," "); //PATH
			strcpy(path,entry1);
			entry1 = strtok(NULL, " "); //id
			strcpy(aplha,entry1);
			jInfo1 * assistant2;
			assistant2 = findFromList1(atoi(aplha));
			printf("$$delay:%d\n",assistant2->Delay);
			int fetchDelay = assistant2->Delay; 
			if(isAdir(path))
			{
				//its a directory
				struct dirent *pdirent;
				DIR *pdir;
				pdir = opendir(path);
				while((pdirent = readdir(pdir))!=NULL)
				{
					if((strcmp (pdirent->d_name, "..")==0)||(strcmp (pdirent->d_name, ".") == 0)) 
						{
						}
						else
						{
							char fpath[500];
							sprintf(fpath,"%s/%s",path,pdirent->d_name);
							printf(">>%s\n",fpath);
					//		ssize_t len1=send(csock,fpath,strlen(fpath),0);
						//	printf(">>%d\n",len1);
							//sleep(fetchDelay);
							char fileToSend[200];
							strcpy(fileToSend,fpath);
							int peer_socket;
							struct sockaddr_in peer_addr;
							socklen_t sock_len;
							int fd,rc;
							off_t offset;
							char buf[1024];
							int remaindata;
							int fsize[10240]={};
							struct stat file_stat;
							fd = open(fileToSend, O_RDONLY);
							if (fd == -1)
							{
								fprintf(stderr, "Error opening file --> %s", strerror(errno));
								exit(EXIT_FAILURE);
							}
							if(fstat(fd,&file_stat)<0)
							{
								fprintf(stderr,"error fstat --> %s",strerror(errno));
								exit(EXIT_FAILURE);
							}
							*fsize = file_stat.st_size;
				
							fprintf(stdout,"File Size \n%d bytes\n",*fsize);
					
							//send size
							//sleep(fetchDelay);
							char file_size[100];
							sprintf(file_size,"%d",(int)file_stat.st_size);
							int len=send(csock,file_size,strlen(file_size),0);
							char send_buf[100000];
							int sent_count=0;
							ssize_t sent_bytes=0;
							ssize_t		read_bytes=0;
							ssize_t		sent_file_size=0;
								offset = 0;
							remaindata = file_stat.st_size;
							int send_bytes=0;
							sleep(fetchDelay);
									while((read_bytes = read(fd,send_buf ,sizeof(send_buf))) >0){
							//rc = sendfile(csock,fd,&offset,file_stat.st_size);
							//if(rc <= 0)	
							//{
							//	break;
							//}				
							//remaindata -= rc;
								if((sent_bytes = send(csock,send_buf,read_bytes,0))<read_bytes){return -1;}
							sent_count++;
							sent_file_size+= read_bytes;
						}	
							close(fd);
						fprintf(stdout,"Done sending\n");
						//fprintf(stdout,">>>>>>>>%ld\n",sent_file_size);
						fflush(stdout);
						
							
					}	
				}
				
			}
			else		
			{
				//its not a directory so i send it immediatly
				printf(">>>$$$$$$ \n");
				char fileToSend[200];
				strcpy(fileToSend,path);
				int peer_socket;
				struct sockaddr_in peer_addr;
				socklen_t sock_len;
				int fd,rc;
				off_t offset;
				int remaindata;
				int fsize[10240]={};
				struct stat file_stat;
				fd = open(fileToSend, O_RDONLY);
				if (fd == -1)
				{
					fprintf(stderr, "Error opening file --> %s", strerror(errno));
					exit(EXIT_FAILURE);
				}
				if(fstat(fd,&file_stat)<0)
				{
					fprintf(stderr,"error fstat --> %s",strerror(errno));
					exit(EXIT_FAILURE);
				}
				*fsize = file_stat.st_size;
				
				fprintf(stdout,"File Size \n%d bytes\n",*fsize);
				
				//send size
				char file_size[100];
				sprintf(file_size,"%d",(int)file_stat.st_size);
				ssize_t len=send(csock,file_size,sizeof(file_size),0);
				char send_buf[256];
				int sent_count;
				ssize_t sent_bytes=0;
				ssize_t	read_bytes=0;
				ssize_t	sent_file_size=0;
				offset = 0;
				remaindata = file_stat.st_size;
				int send_bytes=0;
				sleep(fetchDelay);
				while((read_bytes = read(fd,send_buf ,256)) >0){
					//rc = sendfile(csock,fd,&offset,file_stat.st_size);
					//if(rc <= 0)	
					//{
					//	break;
					//}				
					//remaindata -= rc;
					if((sent_bytes = send(csock,send_buf,read_bytes,0))<read_bytes){return -1;}
					sent_count++;
					sent_file_size+= read_bytes;
				}	
				close(fd);
				fprintf(stdout,"Done sending\n");
				//fprintf(stdout,">>>>>>>>%ld\n",sent_file_size);
				fflush(stdout);
			}
			
		}

		fclose(sock_fp);
	}
	
	return 0;
}
