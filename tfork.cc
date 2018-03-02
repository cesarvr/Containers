#include <sys/types.h>  /* Type definitions used by many programs */
#include <stdio.h>      /* Standard I/O functions */
#include <stdlib.h>     /* Prototypes of commonly used library functions,
                           plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>     /* Prototypes for many system calls */
#include <errno.h>      /* Declares errno and defines error constants */
#include <string.h>     /* Commonly used string-handling functions */

void errExit(const char *str){
	printf("Fatal: need to close because of %s", str);
}

static int idata = 111;
int main(int argc, char *argv[])
{
    int istack = 222;
    pid_t childPid;
    switch (childPid = fork()) {
			case -1:
					errExit("fork");
			case 0:
					idata *= 3;
					istack *= 3;
					break;
			default:
					sleep(3);
					break; 
		}

		/* Allocated in data segment */
		/* Allocated in stack segment */
		/* Give child a chance to execute */
		/* Both parent and child come here */
		printf("PID=%ld %s idata=%d istack=%d\n", (long) getpid(), (childPid == 0) ? "(child) " : "(parent)", idata, istack);
		exit(EXIT_SUCCESS);
}