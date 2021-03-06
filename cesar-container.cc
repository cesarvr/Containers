#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/utsname.h>

#include <iostream>
using namespace std;


#define PROCESS_NAME 2
#define CONTAINER_PARAM 1
#define STACK_SIZE 65536
#define PNAME "container"

#ifndef CHILD_SIG
#define CHILD_SIG SIGUSR1
#endif

static char *cmd = NULL;

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                           } while (0)

char* buildStackMemory(int stackSize) {
  char *stack;                    /* Start of stack buffer */
  char *stackTop;                 /* End of stack buffer */

  /* Allocate stack for child */
	stack = (char *)malloc(stackSize);
	if (stack == NULL)
		 perror("Error allocating memory.");

	stackTop = stack + stackSize;  /* Assume stack grows downward */

  return stackTop;
}


static int execute_command(void *arg) {
	 char *newargv[] = { NULL, NULL };
	 char *newenviron[] = { NULL };

   if (sethostname(PNAME, strlen(PNAME)) == -1)
     errExit("sethostname");

   newargv[0] = (char *)arg;
   execve(newargv[0], newargv, newenviron);
   errExit("execve");   /* execve() returns only on error */
	return 0;
}

pid_t contain(string processName, int (*executor)(void *)) {
  cout << "Containing: " << processName << endl; 
   
  auto topOfStack = buildStackMemory(STACK_SIZE);  
	auto pid = clone(executor, topOfStack, CLONE_NEWUTS | CLONE_NEWPID | SIGCHLD, (void *)processName.c_str());

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
