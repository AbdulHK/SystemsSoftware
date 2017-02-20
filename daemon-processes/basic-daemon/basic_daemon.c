/**
 * Orphan process
 * The child process is adopted by init process, when parent process dies
 **/

#include <stdio.h>
#include <stdlib.h>

int main() {
  // Create a child process
  int pid = fork();

  // If PID > 0, then it is the parent
  if (pid > 0) {
    printf("Parent process\n");
    sleep(10);
    exit(EXIT_SUCCESS);
  } else {
    // Step 1: Create the orphan process
    printf("Child process\n");

    // Orphan logic goes here!
    // The process is kept running in an infinite loop.
    // When the parent finishes after 10 seconds,
    // the getppid() will return 1 as the parent (init process)

    while(1) {
      sleep(1);
      printf("Child 1: My parent is: %i\n", getppid());

    }
  }

  return 0;
}
