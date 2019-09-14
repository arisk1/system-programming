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
/*******coord***********/
typedef struct poolInfo
{
	int poolPid;
	int IDname;
	int poolfdin; //read apo to coord
	int poolfdout; //write sto coord
	int terminationFlag;
	int completeFlag;
	struct poolInfo *ptr;	
}poolInfo;

typedef struct jobInfo
{
	int jobPid;
	int IDname;
	int status; // 0 active - 1 finished - 2 suspended
	struct jobInfo *ptr1;

}jobInfo;
char jmsin[80];
char jmsout[80];
char path[300];
int jobspool;

poolInfo *listOfPools; //head of the list of pools
jobInfo *listOfJobs;	//head of the list of jobs
int poolIDCounter;
int jobIDCounter;
int flagSigterm;
int poolID; //to "onoma" tou kathe pool
int fdin; //read apo console
int fdout; //write sto console
int fdpipeinpool;//write to pool
int fdpipeoutpool;//read to pool


int userCommand(char* input);
void createPool(int poolPid,int IDname , int poolfdin ,int poolfdout);
void createJob(int jobPid,int IDname);
poolInfo* poolBucketFinderViaID(int pidNum);
poolInfo* poolBucketFinderViaPid(int pidNum);
jobInfo* jobBucketFinderViaID(int pidNum);
jobInfo* jobBucketFinderViaPid(int pidNum);
void f(int sigflag);
int findFinishedJobsInPool();
void rqForPoolTermination();
char* getTime(char* time00);
char* getDate(char* date00);
void submit(char* input);
void status(char *input);
void statusAll(char* input);
void showActive(char* input);
void showFinished(char* input);
void showPools(char *input);
void shutDown(char *input);

