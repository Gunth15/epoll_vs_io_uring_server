#include "queue.h"
#include <pthread.h>
#include <unistd.h>
#define RTHREAD_COUNT 3

Queue *global_queue;
Queue *local_queues[RTHREAD_COUNT];

void start_runtime() {}
void dispatcher_thread() {}
void init_thread(int *queue_num) {
  Queue *local_queue = local_queues[*queue_num];

  int ticks = 0;
  for (;;) {
    ticks++;
    if ((ticks %= 61) == 0) {
      // Poll the global queue
      //  poll_global_queue();
    }

    if (!isEmpty(local_queue)) {
      // Run next task
      Task *task = dequeue(local_queue);
      task->fn(task->args);
    } else {
      // attempt to work steal
      for (int i = 0; i < RTHREAD_COUNT; ++i) {
        if (!isEmpty(local_queues[i])) {
          // Run next task
          Task *task = dequeue(local_queue);
          task->fn(task->args);
        }
      }
      sleep(1);
    }
  }
}
void init_blockingT(void *local_queue) { local_queue = (Queue *)local_queue; }
void init_eventT() {}

void async_read();
void async_write();
void async_socket();
