#include "gqueue.h"
#include <stdio.h>

/*
 * The follwing test make sure the global queue works a expected
 * Test 1: A task can be queued and dequeued on a single thread
 * Test 2: Mutiple threads can enqueue and dequeue w/o encounter a race...
 * condition
 */

GQueue gq;

void add(void *num) {
  int *n = (int *)num;
  *n += 1;
}
void sub(void *num) {
  int *n = (int *)num;
  *n -= 1;
}

int main() {

  int count = 0;
  Task *data[5] = {NULL, NULL, NULL, NULL, NULL};

  InitGlobalQueue(&gq, data, 5);

  Task t1 = {add, 0, &count};
  Task t2 = {sub, 0, &count};

  global_enqueue(&gq, &t1);

  Task *task = global_dequeue(&gq);

  flux_fn *fn = task->fn;
  int *args = (int *)task->args;

  fn(args);
  printf("%d\n", count);

  global_queue_clean(&gq);
}
