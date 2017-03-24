/*
  Authour: Alex Kiernan
  Date: 14/03/17
  
  Desc: Module to continuously monitor the dev dir.
    
    1. Redirect stdout to text file
    2. Once every second, execute find on dev dir and filter by user, date and
       filename  
**/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>
#include "routes.h"
#include "changes.h"

int pid;
int pipefd1[2];
int pipefd2[2];

void get_changes();

void dev_tracker() {
  int fd;
  
  int saved_stdout = dup(STDOUT_FILENO);

  FILE *file = fopen(changes_dir, "a");

  fd = fileno(file);

  dup2(fd,STDOUT_FILENO);
  close(fd);

  get_changes();
  
  fflush(stdout);
  dup2(saved_stdout, STDOUT_FILENO);
  close(saved_stdout);
}

void get_changes() {
  char data[4096];

  // Create pipe between find and awk
  if (pipe(pipefd1) == -1) {
    perror("Error Init Pipe");
    exit(1);
  }

  // fork (find)
  if ((pid = fork()) == -1) {
    openlog("change_tracker", LOG_PID|LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "dev_tracker: Error find fork");
    closelog();
    
    perror("Error find fork");
    exit(1);
  } else if (pid == 0) {
    dup2(pipefd1[1], 1);

    // close fds
    close(pipefd1[0]);
    close(pipefd1[1]);

    // exec (find)
    execlp("find", "find", dev_dir, "-cmin", "-0.017", "-type", "f", "-ls",  NULL);
    
    // error check
    perror("Error with ls -al");
    _exit(1);
  }

  // Create pipe between awk and sort
  if (pipe(pipefd2) == -1) {
    perror("Error Init Pipe");
    exit(1);
  }

  // fork (awk)
  if ((pid = fork()) == -1) {
    openlog("change_tracker", LOG_PID|LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "dev_tracker: Error awk fork");
    closelog();
    
    perror("Error awk fork");
    exit(1);
  } else if (pid == 0) {
    // get the input from pipe 1
    dup2(pipefd1[0], 0);

    // set the output to pipe 2
    dup2(pipefd2[1], 1);

    // close fds
    close(pipefd1[0]);
    close(pipefd1[1]);
    close(pipefd2[0]);
    close(pipefd2[1]);

    // exec (awk), specifying user, date and filepath
    execlp("awk", "awk", "{print $5,$8,$9,$10,$11}", NULL);

    // error check
    perror("bad exec grep root");
    _exit(1);
  }

  // close unused fds
  close(pipefd1[0]);
  close(pipefd1[1]);

  // fork (sort)
  if ((pid = fork()) == -1) {
    openlog("change_tracker", LOG_PID|LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "dev_tracker: Error sort fork");
    closelog();
    
    perror("Error sort fork");
    exit(1);
  } else if (pid == 0) {
    // get the input from pipe 2
    dup2(pipefd2[0], 0);

    // close fds
    close(pipefd2[0]);
    close(pipefd2[1]);
    // exec
    execlp("sort", "sort", "-u", NULL);

    // error check
    perror("Error with sort");
    _exit(1);
  } else {
    close(pipefd2[1]);
    int nbytes = read(pipefd2[0], data, sizeof(data));
    printf("%.*s", nbytes, data);
    close(pipefd2[0]);
  }
}
