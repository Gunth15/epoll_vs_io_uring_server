#include "flux_scheduler.h"
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>

// Structure of asyc function
bool flux_async(Future *f, flux_fn func, void *args) {
  f->poll = PENDING;

  if (sem_init(&f->sem, 0, 0) != 0) {
    fprintf(stderr, "Error: %s", strerror(errno));
  }

  /*Task is stored on heap b/c memory needs to exist outside of thread's stack*/
  Task *task = malloc(sizeof(Task));
  task->args = args;
  task->fn = func;
  task->future = *f; // Future only needs to exist on calling thread's stack, so
                     // only need a pointer
  task->ctxt = 0;
  global_enqueue(task);
  return true;
}

// handler for return of async function
void flux_return(Future *f, void *ret) {
  f->poll = READY;
  f->ret = ret;
  sem_post(&f->sem);
}

// waits for the asyc function to finish
void *flux_await(Future *f) {
  if (f->poll != READY) {
    sem_wait(&f->sem);
  }
  sem_destroy(&f->sem);
  return f->ret;
}
