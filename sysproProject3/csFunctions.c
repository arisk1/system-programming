#include "csFunctions.h"



void sanitize(char *str)
{
	char *src, *dest;
	for ( src = dest = str ; *src ; src++ )
		if ( *src == '/' || isalnum(*src) )
			*dest++ = *src;
	*dest = '\0';
}

int write_all(int fd, void *buff, size_t size) {
    int sent, n;
    for(sent = 0; sent < size; sent+=n) {
        if ((n = write(fd, buff+sent, size-sent)) == -1) 
            return -1; /* error */
    }
    return sent;
}

/* Print error message and exit */
void perror_exit(char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

void createJInfo1(int idname,int delay1)
{
	jInfo1 *bckt = malloc(sizeof(jInfo1));
	bckt->ID = idname;
	bckt->Delay = delay1;
	bckt->ptr = listOfPools1;
	listOfPools1 = bckt;
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

jInfo1 * findFromList1(int aplha)
{
	jInfo1 * assistant;
	assistant = listOfPools1;
	while(assistant != NULL)
	{
		if(aplha == (assistant->ID))
		{
			return assistant;
		}	
		assistant = assistant->ptr;
	}
	return assistant;
}

void
list_dir (const char * dir_name,char p[][1000])
{
    DIR * d;
	char ena[1000][1000];
	char dio[1000];
	int i=0;
    /* Open the directory specified by "dir_name". */

    d = opendir (dir_name);

    /* Check it was opened. */
    if (! d) {
        fprintf (stderr, "Cannot open directory '%s': %s\n",
                 dir_name, strerror (errno));
        exit (EXIT_FAILURE);
    }
	//printf("\n$$1$$\n");
    while (1) {
        struct dirent * entry;
        const char * d_name;

        /* "Readdir" gets subsequent entries from "d". */
        entry = readdir (d);
        if (! entry) {
            /* There are no more entries in this directory, so break
               out of the while loop. */
            break;
        }
        d_name = entry->d_name;
        /* Print the name of the file and directory. */
//	printf ("%s/%s\n", dir_name, d_name);

	/* If you don't want to print the directories, use the
	   following line: */

       if (! (entry->d_type & DT_DIR)) {
	//    printf ("%s/%s\n", dir_name, d_name);
		sprintf(dio,"%s/%s", dir_name, d_name);
		strcpy(ena[i],dio);
			i++;
	} 



        if (entry->d_type & DT_DIR) {

            /* Check that the directory is not "d" or d's parent. */
            if (strcmp (d_name, "..") != 0 &&
                strcmp (d_name, ".") != 0) {
                int path_length;
                char path[PATH_MAX];
 
                path_length = snprintf (path, PATH_MAX,
                                        "%s/%s", dir_name, d_name);
          //     printf ("%s\n", path);
		sprintf(dio,"%s",path);
		strcpy(ena[i],dio);
			i++;
                if (path_length >= PATH_MAX) {
                    fprintf (stderr, "Path length has got too long.\n");
                    exit (EXIT_FAILURE);
                }
                /* Recursively call "list_dir" with the new path. */
                list_dir (path,p);
            }
	}
    }
    /* After going through all the entries, close the directory. */
    if (closedir (d)) {
        fprintf (stderr, "Could not close '%s': %s\n",
                 dir_name, strerror (errno));
        exit (EXIT_FAILURE);
    }
	int x;
	static int counter;
	for( x=0;x<1000;x++)
	{
		if(strcmp("",ena[x])!=0)
		{
		//	printf(">>>%s\n",ena[x]);
			strcpy(p[counter],ena[x]);
						counter++;
		}
	}
	//printf("\n >>> %d\n",counter);
}

