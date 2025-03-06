#include "flux_scheduler.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

Future test_f;
/*
 * This file test the Future implemntation and
 * the overall design form a nomral user standpoint
 *
 * Test 1: Sending a task to the worker thread
 * Test 2: Awaiting the task
 * Test 3: Testing the output of task
 */

void add(void *num) {
  int n = *(int *)num;
  int r = n + 3;
  flux_return(&test_f, &r);
}
void sub(void *num) {
  int *n = (int *)num;
  *n -= 1;
  flux_return(&test_f, n + 1);
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

  int count = 0;
  Task *data[10] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0};
  pthread_t thread_1;
  pthread_create(&thread_1, NULL, &run_task, NULL);
  InitGlobalQueue(data, 10);

  // TEST 1
  flux_async(&test_f, add, &count);

  // TEST 2
  int *ret = (int *)flux_await(&test_f);
  printf("%d", *ret);

  // assert(count == 3 && "Count was not 3");
  // printf("Output after queueing %d\n", count);

  // Make sure to cleanup before exiting
  // Shold not needs these if mutex works properly
  // pthread_join(thread_2, NULL);
  // pthread_join(thread_2, NULL);
  global_queue_clean();
}
