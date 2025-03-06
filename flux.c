#include "flux_scheduler.h"
#include <pthread.h>
#include <threads.h>

// Structure of asyc function
void flux_async(Future *f, flux_fn func, void *args) {
  f->poll = PENDING;
  pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
  pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
  f->cond = &cond;
  f->mut = &mut;

  Task task;
  task.args = args;
  task.fn = func;
  task.future = *f;
  task.ctxt = 0;

  global_enqueue(&task);
}
// handler for return of async function
void flux_return(Future *f, void *ret) {
  f->poll = READY;
  f->ret = ret;
  pthread_cond_signal(f->cond);
}

// waits for the asyc function to finish
void *flux_await(Future *f) {
  while (f->poll != READY)
    pthread_cond_wait(f->cond, f->mut);
  pthread_mutex_destroy(f->mut);
  return f->ret;
}
