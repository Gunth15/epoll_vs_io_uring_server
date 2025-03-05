#include "flux.h"
#include <pthread.h>
#include <threads.h>

void flux_async(flux_fn func, void *args, Future *f) {
  f->poll = PENDING;
  f->tid = pthread_self();
}
void flux_await(Future *f) {}
void flux_ret(Future *f) { f->poll = READY; }
