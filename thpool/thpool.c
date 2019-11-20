#include "thpool.h"
#include <stdbool.h>

thpool_t* thpool_init(int num_threads) {
  thpool_t *p = (thpool_t*)malloc(sizeof(thpool_t));
  if (p==NULL) return NULL;

  p->threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
  if (p->threads==NULL) return NULL;

  p->queue = jobqueue_init();
  p->num_threads = num_threads;
  p->running = true;
  pthread_mutex_init(&(p->update_mutex), NULL);
  pthread_cond_init(&(p->update_cond), NULL);

  for(int i=0; i<num_threads; i++) {
    int rc = pthread_create(&(p->threads[i]), NULL, (void*)thread_do, p);
    if (rc) return NULL;
    pthread_detach(p->threads[i]);
  }
  return p;
}

bool thpool_add_job(thpool_t *p, void *(*function)(void*), void *args) {
  bool res = jobqueue_push(p->queue, function, args);
  if (res) {
    pthread_mutex_lock(&(p->update_mutex));
    pthread_cond_broadcast(&(p->update_cond));
    pthread_mutex_unlock(&(p->update_mutex));
  }
  return res;
}

void thpool_destroy(thpool_t *p) {
  p->running = false;

  pthread_mutex_lock(&(p->update_mutex));
  pthread_cond_broadcast(&(p->update_cond));
  pthread_mutex_unlock(&(p->update_mutex));

  free(p->threads);
  jobqueue_destroy(p->queue);
  pthread_mutex_destroy(&p->update_mutex);
  pthread_cond_destroy(&p->update_cond);
  free(p);
}

static void* thread_do(thpool_t *p) {
  job_t *j;
  while (true) {
    while (p->running) {
      j = jobqueue_pop(p->queue);
      if (j!=NULL) {
        j->function(j->function);
      }
    }
    if (!p->running) break;
    pthread_mutex_lock(&(p->update_mutex));
    pthread_cond_wait(&(p->update_cond), &(p->update_mutex));
    pthread_mutex_unlock(&(p->update_mutex));
  }
  pthread_exit(NULL);
}