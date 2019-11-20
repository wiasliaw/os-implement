#ifndef _THPOOL_
#define _THPOOL_

#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include "thpool.h"
#include "../jobqueue/jobqueue.h"

typedef struct thpool {
  pthread_t *threads;
  jobqueue_t *queue;
  unsigned int num_threads;
  pthread_mutex_t update_mutex;
  pthread_cond_t  update_cond;
  bool running;
} thpool_t;

static void* thread_do(thpool_t *p);

thpool_t* thpool_init(int num_threads);
bool thpool_add_job(thpool_t *p, void *(*function)(void*), void *args);
void thpool_destroy(thpool_t *p);

#endif
