#include <pthread.h>
// The status realated to poll calls
typedef enum { PENDING = 0, READY = 1 } Status;

// function def
typedef void(flux_fn)(void *);

// What is to be returned from the flux function
typedef struct {
  void *ret;
  Status poll;
  pthread_t tid;
} Future;
