#include "gqueue.h"
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

/*
 Initializes global queue.
 This will panic if it cannot inititialize the mutex locks
*/
static GQueue gq;
void InitGlobalQueue(Task *data[], unsigned capacity) {
  InitQueue(&gq.q, data, capacity);

  // Create locks for enque-ing and dequeue-ing
  if (pthread_mutex_init(&gq.lock, NULL) != 0) {
    fprintf(stderr, "Error initializing global queue lock: %s",
            strerror(errno));
    exit(EXIT_FAILURE);
  }
}

Task *global_dequeue() { return dequeue(&gq.q); }

void global_enqueue(Task *item) { enqueue(&gq.q, item); }

bool global_isEmpty() { return isEmpty(&gq.q); }

bool global_isFull() { return isFull(&gq.q); }

// Muex control wrappers
void global_queue_lock() { pthread_mutex_lock(&gq.lock); }
void global_queue_unlock() { pthread_mutex_unlock(&gq.lock); }

// Cleans the queue mutexes create dby the queue
void global_queue_clean() { pthread_mutex_destroy(&gq.lock); }
