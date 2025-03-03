#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

sigjmp_buf env;

void handler(int sig) {
  printf("Signal %d received. Suspending...\n", sig);
  siglongjmp(env, 1);
}

int main() {
  struct sigaction sa;
  sa.sa_handler = handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  sigaction(SIGUSR1, &sa, NULL); // Use SIGUSR1 to trigger suspension

  if (sigsetjmp(env, 1) == 0) {
    printf("Process running. Send SIGUSR1 to suspend it.\n");
    while (1) {
      sleep(1);
      printf("Working...\n");
    }
  } else {
    printf("Resuming process after suspension.\n");
  }

  return 0;
}
