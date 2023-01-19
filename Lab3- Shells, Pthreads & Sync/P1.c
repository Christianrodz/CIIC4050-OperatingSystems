#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 20
int count = 0, in = 0, out = 0;
int buffer [BUFFER_SIZE];
pthread_t tid;
sem_t empty, fill, buf_mutex;
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
      while(count >= BUFFER_SIZE){
         sem_wait(&empty);//if full, dont produce
         sem_wait(&buf_mutex);
      }
         item = rand() % BUFFER_SIZE ;
         insert(item);
         //unlock
         sem_post(&buf_mutex);
         sem_post(&fill);
         printf("inserted: %d\n", item);
     

   }
}
void * consumer(void * param){
   int item;
   while(1){
      //todo
      //Lock
      while(count == 0){
         sem_wait(&fill);//if empty, dont produce
         sem_wait(&buf_mutex);
      }
      item = remove_item();
      //Unlock
      sem_post(&empty);
      sem_post(&buf_mutex);
    printf("removed: %d\n", item);
     
   }
}
int main(int argc, char * argv[])
{
    sem_init(&buf_mutex,0,1);
    sem_init(&fill,0,0);
    sem_init(&empty,0,BUFFER_SIZE);
    int producers = atoi(argv[1]);
    int consumers = atoi(argv[2]);
    int i;
    for (i = 0; i < producers; i++)
       pthread_create(&tid, NULL, producer,NULL);
    for (i = 0; i < consumers; i++)
       pthread_create(&tid, NULL, consumer, NULL);
    pthread_join(tid,NULL);
}
