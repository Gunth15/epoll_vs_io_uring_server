#include "gqueue.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

/*
 Initializes global queue.
 This will panic if it cannot inititialize the mutex locks
*/
void InitGlobalQueue(GQueue *q, Task *data[], unsigned capacity) {
  InitQueue(&q->q, data, capacity);

  // Create locks for enque-ing and dequeue-ing
  if (pthread_mutex_init(&q->enqueue_lock, NULL) != 0) {
    fprintf(stderr, "Error initializing global queue lock: %s",
            strerror(errno));
    exit(EXIT_FAILURE);
  }
  if (pthread_mutex_init(&q->dequeue_lock, NULL) != 0) {
    fprintf(stderr, "Error initializing global queue lock: %s",
            strerror(errno));
    exit(EXIT_FAILURE);
  }
}

/*
Locks the queue before attempting to dequeue
This only matters if a worker thread tries to poll the global queue
*/
Task *global_dequeue(GQueue *gq) {
  // CRITICAL SECTION
  pthread_mutex_lock(&gq->dequeue_lock);
  Task *task = dequeue(&gq->q);
  pthread_mutex_unlock(&gq->dequeue_lock);
  ///////////////////
  return task;
}

/*
 * Locks the queue before enqueue-ing data
 * This stops mutltiple threads from adding to the queue if nested asyc calls
 * are made.
 */
void global_enqueue(GQueue *gq, Task *item) {
  // CRITICAL SECTION
  pthread_mutex_lock(&gq->enqueue_lock);
  enqueue(&gq->q, item);
  pthread_mutex_unlock(&gq->enqueue_lock);
  ///////////////////
}

// better safe then sorry locking
bool global_isEmpty(GQueue *gq) {
  // CRITICAL SECTION
  pthread_mutex_lock(&gq->enqueue_lock);
  bool empty = isEmpty(&gq->q);
  pthread_mutex_unlock(&gq->enqueue_lock);
  ///////////////////
  return empty;
}

// better safe then sorry locking
bool global_isFull(GQueue *gq) {
  // CRITICAL SECTION
  pthread_mutex_lock(&gq->dequeue_lock);
  bool full = isFull(&gq->q);
  pthread_mutex_unlock(&gq->dequeue_lock);
  ///////////////////
  return full;
}

// Cleans the queue mutexes create dby the queue
void global_queue_clean(GQueue *gq) {
  pthread_mutex_destroy(&gq->enqueue_lock);
  pthread_mutex_destroy(&gq->dequeue_lock);
}
