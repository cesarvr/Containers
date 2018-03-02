#include <stdio.h>


void execFn(int (*fn)(void *)) {

  printf("\n execFn -> %d \n " fn());
}

int main() {

  puts("Testing anonymous function \n"); 

  int a = 1; 
  int b = 1; 

  execFn(int (*max)(int, int) =
          ({
            return a+b;
           })) 

  return 0; 
}
