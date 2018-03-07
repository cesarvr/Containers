#include <iostream>
#include <vector>
#include <unistd.h>

using namespace std; 

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE);} while (0)

void endProcess(string message){
   cout << message << endl;
   exit(EXIT_FAILURE);
}

template <typename... P> 
int run(P... params) {
  char *args[] = {(char *)params..., (char *)0};

  int ret = execvp(args[0], args);  // execvp('/bin/bash' , ["-c", "echo hello world"])
  errExit("execve");

  return ret;
}

int main(int argc, char* argv[]){

  run("/bin/bash", "-c", "echo hello Wolrd");

  return 0;
}
