// API for accesing the gloabl queue
#include <semaphore.h>
#include <stdbool.h>
#include <stdlib.h>
// The status realated to poll calls
typedef enum { PENDING = 0, READY = 1 } Status;

// What is to be returned from the flux function
typedef struct {
  void *ret;
  Status poll;
  sem_t sem;
} Future;

// function def
typedef void(flux_fn)(void *, Future *);

// Task on queue
typedef struct {
  flux_fn *fn;
  void *args;
  int ctxt;
  Future *future;
} Task;

typedef struct {
  int capacity;
  int head;
  int tail;
  Task **data;
} Queue;

typedef struct {
  Queue q;
  pthread_mutex_t lock;
} GQueue; // Global queue is just a queue with a mutex lol

//////////////////////FLUX//////////////////////////////////////////////
// How a async function is declared
Future *flux_async(flux_fn func, void *args);
// Declares the value returned by the future
void flux_return(Future *f, void *ret);
// Tells the calling thread to wait for output of future
void *flux_await(Future *f);
// Destroys the flux Future. Make sure to copy any m
void flux_destroy_future(Future *f);
////////////////////////////////////////////////////////////////////////

/////////////////////////////QUEUE/////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////

//////////////////////GLOBAL QUEUE///////////////////////////////////
// WARNING: MUST REQUIRE MUTEX LOCK BEFORE ANY READ/WRITE OPERATIONS
//
//  Mutex control wrappers
void global_queue_lock();
void global_queue_unlock();

// Initializes global queue
void InitGlobalQueue(Task **data, unsigned capacity);
/*
 * The following functions are equivalent to their normal queue counter parts,
 */
Task *global_dequeue();
void global_enqueue(Task *item);
bool global_isEmpty();
bool global_isFull();
void global_queue_clean();
////////////////////////////////////////////////////////////////////////
