#ifndef _JOBQUEUE_
#define _JOBQUEUE_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

// type
typedef struct job {
  void *(*function)(void* args);
  void *args;
} job_t;

typedef struct entry {
  job_t job;
  void* next;
} entry_t;

typedef struct jobqueue {
  entry_t* head;
  entry_t* tail;
  unsigned int size;
  pthread_mutex_t lock;
} jobqueue_t;

// api
/**
 * job queue
 * # jobqueue_t* jobqueue_init ();
 * # bool jobqueue_push (jobqueue_t* q, job_t j);
 * # job_t* jobqueue_pop (jobqueue_t* q);
 * # void jobqueue_destroy (jobqueue_t* q);
 */

jobqueue_t* jobqueue_init(void);
bool jobqueue_push(jobqueue_t* q, void *(*funciton)(void*), void *args);
void jobqueue_destroy(jobqueue_t* q);
job_t* jobqueue_pop (jobqueue_t* q);
bool jobqueue_isEmpty(jobqueue_t* q);

#endif
