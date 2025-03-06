// Static Queue data structure written in C by C.W
#include "flux_scheduler.h"
#include <stdio.h>
#include <stdlib.h>

void InitQueue(Queue *q, Task *data[], unsigned capacity) {
  q->capacity = capacity;
  q->head = -1;
  q->tail = -1;
  q->data = data;
};

Task *dequeue(Queue *q) {
  if (isEmpty(q)) {
    fprintf(stderr, "Error: Queue is empty\n");
    exit(EXIT_FAILURE);
  } else {
    Task *data = q->data[q->head];
    if (q->head == q->tail) {
      q->head = -1;
      q->tail = -1;
    } else {
      q->head = (q->head + 1) % q->capacity;
    }
    return data;
  }
}

void enqueue(Queue *q, Task *func) {
  if (isFull(q)) {
    fprintf(stderr, "Error: Queue is full\n");
    exit(EXIT_FAILURE);
  }

  if (isEmpty(q))
    q->head = 0;
  q->tail = (q->tail + 1) % q->capacity;
  q->data[q->tail] = func;
}

Task *peek(Queue *q) {
  if (isEmpty(q)) {
    return NULL;
  } else {
    return q->data[q->head];
  }
}

bool isEmpty(Queue *q) { return q->head == -1; }

bool isFull(Queue *q) {
  return (q->head == (q->tail + 1) % q->capacity) ||
         (q->head == 0 && q->tail == q->capacity - 1);
}
