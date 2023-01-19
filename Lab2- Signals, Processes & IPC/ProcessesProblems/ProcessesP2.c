#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {
   char command[300];
   //copy the command to download the file
   strncpy(command,"wget --no-check-certificate https://www.c-programming-simple-steps.com/support-files/return-statement.zip",300); 
   //execute the command
   system(command);
   //copy the commmand to list the files
   strncpy(command, "ls | grep return*",300);
   //execute the command
   system(command);
   return(0);
} 