#include "flux_scheduler.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t thread_1;
/*
 * This file test the Future implemntation and
 * the overall design form a nomral user standpoint
 *
 * Test 1: Sending a task to the worker thread
 * Test 2: Awaiting the task
 * Test 3: Testing the output of task
 */

void add(void *num, Future *f) {
  int *n = (int *)num;
  *n += 3;
  flux_return(f, n);
}
void sub(void *num, Future *f) {
  int *n = (int *)num;
  *n -= 1;
  flux_return(f, n);
}

void *run_task(void *_) {
  printf("Thread started\n");
  while (1) {
    //////////// Critical Section
    global_queue_lock();
    if (!global_isEmpty()) {
      printf("Handling task\n");
      Task *task = global_dequeue();
      global_queue_unlock();
      ////////////////////

      flux_fn *fn = task->fn;
      void *args = (int *)task->args;
      Future *fut = task->future;
      fn(args, fut);
      free(task);
      printf("Done\n");

    } else {
      global_queue_unlock();
      printf("Sleeping....\n");
      sleep(1);
      ////////////////////
    }
  }
  return NULL;
}

int main() {

  int count = 0;
  Task *data[10] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0};
  InitGlobalQueue(data, 10);
  pthread_create(&thread_1, NULL, &run_task, NULL);

  // TEST 1
  Future *fut = flux_async(add, &count);

  // TEST 2
  printf("Doing other work while waiting...\n");
  sleep(1);
  int *ret = flux_await(fut);

  // TEST 3
  printf("Output after async function is %d\n", *ret);
  assert(*ret == 3 && "Count was not 3");

  // BONUS
  fut = flux_async(sub, ret);
  ret = flux_await(fut);
  printf("Output after async function is %d\n", *ret);
  assert(*ret == 2 && "Count was not 1");

  printf("Orginal count was %d\n", count);
  global_queue_clean();
}
