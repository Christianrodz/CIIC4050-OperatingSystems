#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

/* this data is shared by the thread(s) */
int threads;
unsigned long long iterations;
unsigned long long block_size;
double * pi;

typedef struct{
    unsigned long long lower;
    unsigned long long upper;
    int thr;
}thread_data;
thread_data* td;

void * runner(void * param); /* the thread */

int main(int argc, char * argv[]) {
        if (argc != 3) {
            fprintf(stderr, "usage: a.out <iterations> <threads>\n");
            /*exit(1);*/
            return -1;
        }
        if (atoi(argv[1]) < 0 || atoi(argv[2]) < 0) {
            fprintf(stderr, "Arguments must be non-negative\n ");
                /*exit(1);*/
                return -1;
            }
       
   
        /* create the thread identifiers */
        pthread_t  tid[atoi(argv[2])];
   
        /* create set of attributes for the thread */
        pthread_attr_t attr;
   
         /* populate variables... */
        int iterations = atoi(argv[1]);
        int threads = atoi(argv[2]);
        block_size = iterations/threads;
        double piTotal = 0.0;
        td = malloc(threads*sizeof(thread_data));
        pi = malloc(threads*sizeof(double));
   
     
            /* get the default attributes */
            pthread_attr_init(&attr);
           
            /* create threads */
            for(int i = 0; i < threads; i++){
                td[i].lower = (block_size*i) + 1;
                td[i].upper = block_size*(i+1);
                td[i].thr = i;
                pthread_create(&tid[i], &attr, runner, (void *) &td[i].thr);
            }
           
            /* now wait for the threads to exit */
            for(int i = 0; i < threads; i++){
                pthread_join(tid[i],NULL);
            }
           
              /* compute and print results */
              for(int i = 0; i < threads;i++){
                 piTotal += pi[i];
               
              }
              piTotal = (1+piTotal)*4;

             
              printf("pi = %.15f\n", piTotal);
            free(td);

        }
        /**
         * The thread will begin control in this function
         */
        void * runner(void * param) {
            int threadid = *(int *) param;
            pi[threadid] = 0;
            unsigned long long lower = td[threadid].lower;
            unsigned long long upper = td[threadid].upper;

            //complete function
            for(unsigned long long i = lower; i <= upper; i++){
                pi[threadid] += (pow(-1.0,i)/((2.0*i) + 1));
            }
           
            pthread_exit(0);
        }
        /*THE OUTPUT I RECEIVED IS:
        pi = 3.141692643590543
        */