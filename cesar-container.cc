#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define PROCESS_NAME 2
#define CONTAINER_PARAM 1

#ifndef CHILD_SIG
#define CHILD_SIG SIGUSR1
#endif


static char *cmd = NULL;

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                           } while (0)

static int execute_command(void *arg) {
	printf("Boom!");
	 char *newargv[] = { NULL, NULL };
	 char *newenviron[] = { NULL };

	 if(arg != NULL) {
		 newargv[0] = (char *)arg;
		 execve(newargv[0], newargv, newenviron);
		 perror("execve");   /* execve() returns only on error */
		 printf("Error!!");
		 exit(EXIT_FAILURE);
	 }else{
		 printf("Error!!");
		 perror("Error not command was provided");   /* execve() returns only on error */
		 exit(EXIT_FAILURE);
	 }
	return 0;
}


int main(int argc, char *argv[]) {
  
  puts("Cesar's Container \n");

  for(int i=0; i < argc; i++ ){
    printf("arg -> %s \n", argv[i]);
  }

  // Check if we pass run in the command line. 
  if(strcmp(argv[CONTAINER_PARAM], "run") == 0) {
    printf("Running... %s \n", argv[PROCESS_NAME]);

		const int STACK_SIZE = 65536;
    char *stack;                    /* Start of stack buffer */
    char *stackTop;                 /* End of stack buffer */
		pid_t pid; 

		/* Allocate stack for child */
		stack = (char *)malloc(STACK_SIZE);
		if (stack == NULL)
			 perror("Error allocating memory.");
		stackTop = stack + STACK_SIZE;  /* Assume stack grows downward */

    cmd = argv[PROCESS_NAME]; 

		/* Create child that has its own UTS namespace;
              child commences execution in childFunc() */

	  pid = clone(execute_command, stackTop, CLONE_NEWUTS | SIGCHLD, argv[PROCESS_NAME]);
		printf("clone() returned %ld\n", (long) pid);

    if(pid == -1)
      perror("error: clone()");

    if(waitpid(pid, NULL, 0) == -1)    /* Wait for child */
      errExit("waitpid");
    printf("child has terminated \n");
  }

  return 0; 
}
