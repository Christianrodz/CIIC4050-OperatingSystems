//#include "ex3.h"

#define MAX_CHARS 10000

void decode(char* cmd){
    //TODO
    int sqr = 0, strItr = 0, len;
    float Fsqr = 0;
    len = (unsigned long) strlen(cmd) - 1;
    Fsqr = sqrt((double) len);
    sqr = Fsqr;

    if(sqr == Fsqr){
        //Convert string into a Column Major Order Matrix
        char cmoMatrix[sqr][sqr];
        for(int j = 0; j < sqr; j++){
            for(int i = 0; i < sqr; i++){
                cmoMatrix[i][j] = cmd[strItr];
                strItr++;
            }
        }
        //After obtaining Column Major Order Matrix, print out the matrix by reading in Row Major Order
        for(int i = 0; i < sqr; i++){
            for(int j = 0; j < sqr; j++){
                printf("%c",cmoMatrix[i][j]);
            }
        }
        printf("\n");//After printing throw a new line for next input.
    }else{
        printf("INVALID\n");
    }
}