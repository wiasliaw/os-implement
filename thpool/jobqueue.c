#include "jobqueue.h"
#include <stdbool.h>

// implement
jobqueue_t* jobqueue_init () {
  jobqueue_t *q = (jobqueue_t*)malloc(sizeof(jobqueue_t));
  if (q==NULL) {
    free(q);
    return NULL;
  }

  q->head = NULL;
  q->tail = NULL;
  q->size = 0;
  pthread_mutex_init(&(q->lock), NULL);
  return q;
}

bool jobqueue_push(jobqueue_t* q, void *(*funciton)(void*), void *args) {
  job_t j = (job_t){funciton, args};
  // create entry
  entry_t *e = (entry_t*)malloc(sizeof(entry_t));
  if (e==NULL) {
    free(e);
    return false;
  }
  e->job = j;
  e->next = NULL;

  // put into jobqueue
  pthread_mutex_lock(&(q->lock));
  if(q->head==NULL) {
    q->head = q->tail = e;
  } else {
    q->tail->next = e;
    q->tail = e;
  }
  pthread_mutex_unlock(&(q->lock));
  return true;
}

job_t* jobqueue_pop (jobqueue_t* q) {
  pthread_mutex_lock(&(q->lock));
  if (q->head==NULL) {
    pthread_mutex_unlock(&(q->lock));
    return NULL;
  }

  entry_t *top = q->head;
  job_t *j = &(top->job);

  if(q->head==q->tail) {
    // only one job
    q->head = q->tail = NULL;
  } else {
    q->head = q->head->next;
  }
  pthread_mutex_unlock(&(q->lock));
  return j;
}

void jobqueue_destroy(jobqueue_t* q) {
  pthread_mutex_lock(&(q->lock));
  entry_t *current = q->head;
  while(current!=NULL) {
    entry_t *next = current->next;
    free(current);
    current = next;
  }
  pthread_mutex_unlock(&(q->lock));
  pthread_mutex_destroy(&(q->lock));
  free(q);
  return;
}

bool jobqueue_isEmpty(jobqueue_t* q) {
  if (q->head==q->tail && q->tail==NULL) {
    return true;
  }
  return false;
}
