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

#include <iostream>
using namespace std;

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

pid_t contain(string processName, int (*executor)(void *)){

  cout << "Containing: " << processName << endl; 

	pid_t pid; 
  const int STACK_SIZE = 65536;
  char *stack;                    /* Start of stack buffer */
  char *stackTop;                 /* End of stack buffer */

  /* Allocate stack for child */
	stack = (char *)malloc(STACK_SIZE);
	if (stack == NULL)
		 perror("Error allocating memory.");

  cout << "stack size: " << STACK_SIZE << endl;
  cout << "stack memory position (after): \t" << &stack << endl;

	stackTop = stack + STACK_SIZE;  /* Assume stack grows downward */

  cout << "stack memory position (before):\t" << &stackTop << endl;

	pid = clone(executor, stackTop, CLONE_NEWUTS | SIGCHLD, (void *)processName.c_str());

  if(pid == -1)
   perror("error: clone()");
  
  cout << "child PID: " << pid << endl; 


  return pid;
}


int main(int argc, char *argv[]) {
  
  puts("Cesar's Container \n");

  if(argc <= PROCESS_NAME) {
    cout << "Usage: container run <process-name>" << endl;
    cout << "Example:" << endl; 
    cout << "container run /bin/bash   # this will contain the bash process" << endl; 
    return 1;
  }

  // Check if we pass run in the command line. 
  if(strcmp(argv[CONTAINER_PARAM], "run") == 0) {

	  cmd = argv[PROCESS_NAME]; 
    auto pid = contain(argv[PROCESS_NAME], execute_command);

    if(waitpid(pid, NULL, 0) == -1)    /* Wait for child */
      errExit("waitpid");
    printf("child has terminated \n");
  }

  return 0; 
}
