#include "queue.h"
#include <pthread.h>
#include <unistd.h>
#define RTHREAD_COUNT 3

Queue *global_queue;
Queue *local_queues[RTHREAD_COUNT];

void start_runtime() {}
void dispatcher_thread() {}
void init_rthread(int *queue_num) {
  Queue *local_queue = local_queues[*queue_num];

  int ticks = 0;
  for (;;) {
    if (ticks % 61 == 0) {
      // Poll the global queue
      //  poll_global_queue();
      ticks = 0;
    }

    if (!isEmpty(local_queue)) {
      // Run next task
      fn func = dequeue(local_queue);
      func();
    } else {
      // attempt to work steal
      for (int i = 0; i < RTHREAD_COUNT; ++i) {
        if (!isEmpty(local_queues[i])) {
          // Run next task
          fn func = dequeue(local_queues[i]);
          func();
        }
      }
      sleep(1);
    }
    ticks++;
  }
}
void init_bthread(void *local_queue) { local_queue = (Queue *)local_queue; }
void init_ethread(void *local_queue) { local_queue = (Queue *)local_queue; }

void async_read();
void async_write();
void async_socket();
