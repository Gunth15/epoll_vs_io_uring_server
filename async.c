#include "queue.h"
#include <pthread.h>
#include <unistd.h>

Queue *global_queue;

void start_runtime(int event_threads, int blokcing_threads, int run_threads,
                   int queue_size) {}

void init_bthread(void *local_queue) {
  local_queue = (Queue *)local_queue;

  int ticks = 0;
  for (;;) {
    if (ticks % 61 == 0) {
      // poll_global_queue();
      ticks = 0;
    }

    if (!isEmpty(local_queue)) {
      fn func = dequeue(local_queue);
      func();
    } else {
      sleep(1);
    }
    ticks++;
  }
}
void init_rthread(void *local_queue) { local_queue = (Queue *)local_queue; }
void init_ethread(void *local_queue) { local_queue = (Queue *)local_queue; }

void async_read();
void async_write();
void async_socket();
