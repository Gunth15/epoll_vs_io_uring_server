#include <pthread.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

pthread_t thread;
sigjmp_buf env;

void restart(int sig) {
  printf("Restarting thread lol\n");
  siglongjmp(env, 1);
}
void *handler(void *none) {
  printf("Thread started\n");
  struct sigaction sig_a;

  sig_a.sa_handler = restart;
  sigemptyset(&sig_a.sa_mask);
  sig_a.sa_flags = 0;

  sigaction(SIGABRT, &sig_a, NULL); // Use SIGUSR1 to trigger suspension

  sigsetjmp(env, 1);
  int count = 0;
  while (1) {
    printf("Runningx%d\n", ++count);
  }
}

int main() {
  pthread_create(&thread, NULL, handler, NULL);
  for (;;) {
    printf("Restarting thread in 1 second\n");
    sleep(4);
    pthread_kill(thread, SIGABRT);
  }
}
