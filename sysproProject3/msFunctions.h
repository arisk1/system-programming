#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <netdb.h>
#include <stdbool.h>
#include <dirent.h>
#include <time.h>
#include <limits.h>
#include <ctype.h>

#define perror2(s, e) fprintf(stderr, "%s: %s\n", s, strerror(e))
#define PERMS 0777
#define POOL_SIZE 10

typedef struct mmInfo
{
	char fullLine[1000];
	int IDname;
	char mmAddress[100];
	int mmPort;
	char mmDirOrFile[500];	
	int mmDelay;
}mmInfo;


typedef struct jInfo
{
	int ID;
	char fline[10000];
	int Delay;
	struct jInfo *ptr;
}jInfo;

typedef struct csInfo
{
	char csName[100];
	int numOfRequests;
	struct csInfo *next;

}csInfo;

typedef struct poolInfo
{
	char data[POOL_SIZE][1000];
	int start;
	int end;
	int count;
}poolInfo;


pthread_mutex_t mtx;
pthread_mutex_t mutex2;
pthread_cond_t write_cond;
pthread_cond_t read_cond;
poolInfo pool;
jInfo * listOfPools;
csInfo * LOI;

void createJInfo(int idname,int delay,char* str);
void createCsInfo(char *name);
jInfo * findFromList(char* line);
csInfo * findFromCsList(char* nname);
void perror_exit(char *message);
int isAdir(char *p);
int write_all(int fd, void *buff, size_t size);
void initialize(poolInfo * pool);
void place(poolInfo * pool,char *data);
char* obtain(poolInfo * pool);
