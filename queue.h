// Fized size Circular Queue data structure written in C by C.W
//  For the sake of simplicity, generics have been entirely avoided
//  Change the types when needed
#include <stdbool.h>
#include <stddef.h>

typedef void (*fn)(); // function def

// Query head must be set to -1 and the capacity to the size of the array
typedef struct {
  int capacity;
  int head;
  int tail;
  fn *data;
} Queue;

void InitQueue(Queue *q, fn *data, unsigned capacity); // Initializes queue
fn dequeue(Queue *q); // Dequeue from stack, Crashes program if you try to
                      // dequeue when there is no data
void enqueue(Queue *q, fn func); // Enqueue to stack. Crashes if you try
                                 // to enqueue past capacity
void *peek(Queue *q); // Peeks whats in front of the queue, returns -1 if empty.
                      // Completely safe
bool peekRear(Queue *q); // Checks if queue is empty
bool isEmpty(Queue *q);  // Checks of queue is full
bool isFull(Queue *q);   // Checks if queue is empty
