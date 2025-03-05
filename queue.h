// Fized size Circular Queue data structure written in C by C.W
//  For the sake of simplicity, generics have been entirely avoided
//  Change the types when needed
#include "flux.h"
#include <stdbool.h>
#include <stddef.h>

// Task on queue
typedef struct {
  flux_fn *fn;
  int ctxt;
  void *args;
  Future future;
} Task;

// Query head & tail must be set to -1 and the capacity to the size of the array
typedef struct {
  int capacity;
  int head;
  int tail;
  Task **data;
} Queue;

void InitQueue(Queue *q, Task **data, unsigned capacity); // Initializes queue
Task *dequeue(Queue *q); // Dequeue from stack, Crashes program if you try to
                         // dequeue when there is no data
void enqueue(Queue *q, Task *func); // Enqueue to stack. Crashes if you try
                                    // to enqueue past capacity
Task *peek(Queue *q); // Peeks whats in front of the queue, returns -1 if empty.
                      // Completely safe
bool peekRear(Queue *q); // Checks if queue is empty
bool isEmpty(Queue *q);  // Checks of queue is full
bool isFull(Queue *q);   // Checks if queue is empty
