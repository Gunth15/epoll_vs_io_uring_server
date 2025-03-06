#include "flux_scheduler.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

/*
 * The follwing test make sure the global queue works a expected
 * Test 1: A task can be queued and dequeued on a single thread
 * Test 2: Mutiple threads can enqueue and dequeue w/o encounter a race...
 * condition
 */

// NOTE: This test dos not make use of futire because they were implemented a
// little afterwards
void add(void *num) {
  int *n = (int *)num;
  *n += 1;
}
void sub(void *num) {
  int *n = (int *)num;
  *n -= 1;
}

void *run_task(void *_) {
  sleep(1);
  bool stop = false;
  while (!stop) {
    // Critical Section
    global_queue_lock();
    if (!(stop = global_isEmpty())) {
      Task *task = global_dequeue();
      flux_fn *fn = task->fn;
      void *args = (int *)task->args;
      fn(args);
    }
    global_queue_unlock();
    ////////////////////
  }
  return NULL;
}

int main() {

  // TEST 1
  int count = 0;
  Task *data[10] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0};

  InitGlobalQueue(data, 10);

  Task t1 = {add, &count, 0};
  Task t2 = {sub, &count, 0};

  global_enqueue(&t1);
  global_enqueue(&t2);
  global_enqueue(&t1);

  Task *task;
  flux_fn *fn;
  void *args;

  for (int i = 0; i < 3; ++i) {
    task = global_dequeue();
    fn = task->fn;
    args = task->args;
    fn(args);
  }

  printf("Output after queueing %d\n", count);
  assert(count == 1 && "Count was not 1");

  // TEST 2

  global_enqueue(&t1);
  global_enqueue(&t2);
  global_enqueue(&t1);
  global_enqueue(&t1);
  global_enqueue(&t2);
  global_enqueue(&t1);

  // Make sure to cleanup before exiting

  pthread_t thread_1, thread_2;
  pthread_create(&thread_1, NULL, &run_task, NULL);
  pthread_create(&thread_2, NULL, &run_task, NULL);
  pthread_join(thread_1, NULL);
  pthread_join(thread_2, NULL);

  assert(count == 3 && "Count was not 3");
  printf("Output after queueing %d\n", count);

  global_queue_clean();
}
