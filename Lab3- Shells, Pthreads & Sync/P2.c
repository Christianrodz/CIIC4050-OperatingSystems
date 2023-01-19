#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 20
int count = 0, in = 0, out = 0;
int buffer [BUFFER_SIZE];
pthread_t tid;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t has_space = PTHREAD_COND_INITIALIZER, is_full = PTHREAD_COND_INITIALIZER;


void insert(int item){
   in = (in+1)%BUFFER_SIZE;
   while (count == BUFFER_SIZE);
   buffer[in] = item;
   count++;
   sleep(1);
   printf("in: %d\t", in);
}
int remove_item(){
   out = (out+1)%BUFFER_SIZE;
   int item;
   while (count == 0);
   item = buffer[out];
   count--;
   sleep(1);
   printf("out: %d\t", out);
   return item;
}
void * producer(void * param){
   int item, r;
   while(1){
      //todo
      //Lock
      pthread_mutex_lock (&mutex);
      while(count == BUFFER_SIZE ){
        pthread_cond_wait(&has_space, &mutex);//Wait for consumer to consume, buffer is full
      }
         item = rand() % BUFFER_SIZE ;
         insert(item);
         pthread_cond_signal(&is_full); // Signal that there is an item to be consumed
         //unlock
         pthread_mutex_unlock(&mutex);
         printf("inserted: %d\n", item);
     

   }
}
void * consumer(void * param){
   int item;
   while(1){
      //todo
      //Lock
     pthread_mutex_lock(&mutex);
      while(count == 0) {
        //If I'm empty, we wait to have a spot
         pthread_cond_wait(&is_full,&mutex);
      }
        item = remove_item();
       pthread_cond_signal(&has_space); //signal that there is space to produce item
        pthread_mutex_unlock(&mutex);  //Unlock
        printf("removed: %d\n", item);
     
   }
}
int main(int argc, char * argv[])
{  
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&has_space,NULL);
    pthread_cond_init(&is_full,NULL);
    int producers = atoi(argv[1]);
    int consumers = atoi(argv[2]);
    int i;
    for (i = 0; i < producers; i++)
       pthread_create(&tid, NULL, producer,NULL);
    for (i = 0; i < consumers; i++)
       pthread_create(&tid, NULL, consumer, NULL);
    pthread_join(tid,NULL);
}