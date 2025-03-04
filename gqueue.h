#include "queue.h"
#include <pthread.h>
#include <stdlib.h>

typedef struct {
  Queue q;
  pthread_mutex_t enqueue_lock;
  pthread_mutex_t dequeue_lock;
} GQueue; // Global queue is just a queue with a mutex lol

// Initializes global queue
void InitGlobalQueue(GQueue *q, Task **data, unsigned capacity);

/*
 * The following functions are equivalent to their normal queue counter parts,
 * But these use have function dependent locks
 */
Task *global_dequeue(GQueue *gq);
void global_enqueue(GQueue *gq, Task *item);
bool global_isEmpty(GQueue *gq);
bool global_isFull(GQueue *gq);

// Cleans queue sys allocated mutexes
void global_queue_clean(GQueue *gq);
