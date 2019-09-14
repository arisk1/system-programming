/* tcpclient.c */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


int main(int argc , char *argv[])
{
	int portnum;
	char csString[10000];
	char hhost[100];
	int sock, bytes_recieved;  
	char send_data[10024],recv_data[10024];
	struct hostent *host;
	struct sockaddr_in server_addr;  
	int parameters=1; //agnoow to onoma
	int next = 0;
	while(parameters<argc) 
	{
		if(strcmp(argv[parameters],"-n")==0)
		{
			strcpy(hhost,argv[parameters+1]);
			host = gethostbyname(hhost);
			parameters++;
		//	printf("%s",hhost);
		}	
		else if(strcmp(argv[parameters],"-p")==0)	
		{
			portnum = atoi(argv[parameters+1]);
			parameters++;
		//	printf("%d",portnum);
		}
		else if((strcmp(argv[parameters],"-s")==0)||(next==1))
		{
			if(next==0)
			{
				parameters++;
				sprintf(csString,"%s",argv[parameters]);
			}
			else
			{
				sprintf(csString+strlen(csString),"%s",argv[parameters]);
			}
			strcat(csString,"-");//diaxorizontai me mia paula
		//	printf("%s",csString);
			next = 1;
		}
		else 
		{
			printf("wrong input");
			return -1;
		}
		parameters++;
	}
	//printf("%s",csString);
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		perror("Socket");
		exit(1);
	}
	server_addr.sin_family = AF_INET;     
	server_addr.sin_port = htons(portnum);   
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8); 
	if (connect(sock, (struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1) 
	{
		perror("Connect");
		exit(1);
	}
	while(1)
	{    
		bytes_recieved=recv(sock,recv_data,10024,0);
		recv_data[bytes_recieved] = '\0';
		
		if(strcmp(recv_data,"start")==0)
		{
			//printf("\nRecieved data = %s\n" , recv_data);           
			strcpy(send_data,csString);
			send(sock,send_data,strlen(send_data), 0);   
		}	
		else if(strstr(recv_data,"/")!=0)
		{
			printf("\nRecieved data:\n"); 
			char aaa[100];
			strcpy(aaa,recv_data);
			char *ea;
			ea = strtok(aaa,"/"); //proti leksi
			printf(">>>Files Transfered:%s\n",ea);
			ea = strtok(NULL,"/");//deuteri
			printf(">>>Bytes Transfered:%s\n",ea);
			ea = strtok(NULL,"/");
			printf(">>>Average:%s\n",ea);
			close(sock);
			break;         
		}
		else 
		{
			printf("you didint send |start|\n");
			close(sock);
			break;
		}	   
	}   
	return 0;
}
