#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "thpool.h"

void task1(){
  printf("Thread #%u working on task1\n", (int)pthread_self());
}

void task2(void *in){
  char* str = (char*)in;
  printf("Thread #%u working on task2, get %s\n", (int)pthread_self(), str);
}

int main(){
  char *str = "ss";
  puts("Making threadpool with 4 threads");
  thpool_t *pool = thpool_init(4);

  puts("Adding 40 tasks to threadpool");
  int i;
  for (i=0; i<20; i++){
    thpool_add_job(pool, (void*)task1, NULL);
    thpool_add_job(pool, (void*)task2, str);
  };

  puts("Killing threadpool");
  for(;;) {
    if(jobqueue_isEmpty(pool->queue)){
      thpool_destroy(pool);
      break;
    }
  }

  return 0;
}