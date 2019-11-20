#include "jobqueue.h"
#include <stdio.h>
#include <stdlib.h>

void task1() {
  printf("hello world\n");
}
int task2() {
  printf("this is two\n");
  return 0;
}

int main(int argc, char const *argv[]) {
  jobqueue_t *q = jobqueue_init();
  job_t *j = NULL;

  jobqueue_push(q, (void*)task1, NULL);
  jobqueue_push(q, (void*)task2, NULL);

  while(!jobqueue_isEmpty(q)) {
    j = jobqueue_pop(q);
    j->function(j->function);
  }

  jobqueue_destroy(q);
  return 0;
}
