#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/sendfile.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <limits.h>
#include <stdbool.h>

typedef struct jInfo1
{
	int ID;
	int Delay;
	struct jInfo1 *ptr;
}jInfo1;

jInfo1 * listOfPools1;

void perror_exit(char *msg);
void createJInfo1(int idname,int delay);
jInfo1 * findFromList1(int aplha);
int write_all(int fd, void *buff, size_t size);
int isAdir(char *p);
void sanitize(char *str);
void
list_dir (const char * dir_name,char p[][1000]);
