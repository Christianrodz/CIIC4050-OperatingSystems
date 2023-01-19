/* Example of use of fork system call */
#include <stdio.h>
main()
{
  int pid;  
  pid = fork();
  if (pid < 0) {
      fprintf(stderr, "Fork failed!\n");
      exit(-1);
  }
  else if (pid==0) {
    execlp("/bin/ps", "ps", NULL);
    printf("I am a child and my own process id is: %d. My father's process id is %d\n", getpid(), getppid());
    exit(0); //Exit before the still around message can be printed
    printf("Still around...\n");
  }
  else {
    wait(NULL);
    printf("My child's process id is: %d\n", pid);
    exit(0);
  }
}