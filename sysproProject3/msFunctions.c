#include "msFunctions.h"

void createJInfo(int idname,int delay1,char *str)
{
	jInfo *bckt = malloc(sizeof(jInfo));
	bckt->ID = idname;
	bckt->Delay = delay1;
	strcpy(bckt->fline,str);
	bckt->ptr = listOfPools;
	listOfPools = bckt;
}

void createCsInfo(char *name)
{
	csInfo *bckt = malloc(sizeof(csInfo));
	strcpy(bckt->csName,name);
	bckt->numOfRequests = 1;
	bckt->next = LOI;
	LOI = bckt;
}



int isAdir(char *p)
{
	int status;
	struct stat st_buf;
	status = stat (p, &st_buf);
	if (status != 0) 
	{
		printf ("Error, errno = %d\n", errno);
		return 1;
	}
	if (S_ISDIR (st_buf.st_mode))
	{ 
		return 1;
	}
	else
	{
		return 0;
	}
}

jInfo * findFromList(char* line)
{
	jInfo * assistant;
	assistant = listOfPools;
	while(assistant != NULL)
	{
		if(strcmp(line,assistant->fline)==0)
		{
			return assistant;
		}	
		assistant = assistant->ptr;
	}
	return assistant;
}

csInfo * findFromCsList(char* nname)
{
	csInfo * assistant1;
	assistant1 = LOI;
	while(assistant1 != NULL)
	{
		if(strcmp(nname,assistant1->csName)==0)
		{
			return assistant1;
		}	
		assistant1 = assistant1->next;
	}
	return assistant1;
}

void perror_exit(char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

int write_all(int fd, void *buff, size_t size) 
{
    int sent, n;
    for(sent = 0; sent < size; sent+=n) {
        if ((n = write(fd, buff+sent, size-sent)) == -1) 
            return -1; /* error */
    }
    return sent;
}

void initialize(poolInfo * pool)
{
	pool->start=0;
	pool->end = -1;
	pool->count = 0;
}

void place(poolInfo * pool,char *data)
{
	pthread_mutex_lock(&mtx);
	//while(pool->count >= POOL_SIZE)
	//{
	//	printf(">>full buffer\n");
	//	pthread_cond_wait(&write_cond,&mtx);
	//}
	pool->end = (pool->end + 1) % POOL_SIZE;
	strcpy(pool->data[pool->end],data);
	pool->count++;
	pthread_mutex_unlock(&mtx);

}

char* obtain(poolInfo * pool)
{
	static char datastring[500];
	pthread_mutex_lock(&mtx);
	//while(pool->count <= 0)
	//{
	//	printf(">>empty buffer\n");
	//	pthread_cond_wait(&read_cond,&mtx);
	//}
	strcpy(datastring,pool->data[pool->start]);
	pool->start = (pool->start + 1) % POOL_SIZE;
	pool->count--;
	pthread_mutex_unlock(&mtx);
	return datastring;
}















