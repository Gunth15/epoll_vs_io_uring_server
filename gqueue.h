// API for accesing the gloabl queue
#include "queue.h"
#include <pthread.h>
#include <stdlib.h>

typedef struct {
  Queue q;
  pthread_mutex_t lock;
} GQueue; // Global queue is just a queue with a mutex lol

// Initializes global queue
void InitGlobalQueue(Task **data, unsigned capacity);

// WARNING: MUST REQUIRE MUTEX LOCK BEFORE ANY READ/WRITE OPERATIONS
//
//  Mutex control wrappers
void global_queue_lock();
void global_queue_unlock();

/*
 * The following functions are equivalent to their normal queue counter parts,
 */
Task *global_dequeue();
void global_enqueue(Task *item);
bool global_isEmpty();
bool global_isFull();

// Cleans queue sys allocated mutexes
void global_queue_clean();
