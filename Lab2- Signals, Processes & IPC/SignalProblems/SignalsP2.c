#include  <stdio.h>
#include  <sys/types.h>
#include  <signal.h>

void generate_SIGSEGV() {
   
  char *mistakes = "Hi";
  mistakes[20] = 'N';
}

void generate_SIGFPE() {
  
   int number = 20;
   int zero = 0;
   zero = number/zero;
}