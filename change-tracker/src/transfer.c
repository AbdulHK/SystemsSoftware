/*
  Authour: Alex Kiernan
  Date: 16/03/17
  
  Desc: Module to find changed files for the previous 24 hours.
    
    1. Find the changed files using unix 'find'
    2. Redirect 'find' output data to file
    3. Open newly update file and store contents in array
    4. Loop through array and copy each file to the live dir
**/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <mqueue.h>
#include "routes.h"
#include <sys/types.h>
#include <sys/wait.h>

#define QUEUE_NAME "/queue"

void get_transfers();

void transfer() {
  get_transfers();
  // Sleep to allow above function to finish
  sleep(5);
  
  // Create local versions of file path constants to nullify warning
  char local_transfers_dir[100];
  strcpy(local_transfers_dir, transfers_dir);
  
  char local_live_html_dir[100];
  strcpy(local_live_html_dir, live_html_dir);
  
  FILE * fp;
  
  int i = 0;
  int num_lines = 0;
  
  fp = fopen(local_transfers_dir, "r");
  if (fp == NULL) {
    printf("Could not open file");
    exit(EXIT_FAILURE);
  }
  
  char records[10][100];
  
  // Store contents of file in array
  while(fscanf(fp, "%s", records[i]) != EOF) {
    i++;
  }
  
  fclose(fp);
  
  num_lines = i;
  
  // Loop through each element of the array and copy file to live dir
  for (int i = 0; i < num_lines; i++) {
    pid_t pid = fork();
    if (pid == 0) {
      char *command = "/bin/cp";
      char *arguments[] = {"cp", "-f", records[i], local_live_html_dir, NULL};
      execvp(command, arguments);
    } else {
      
    }
  }
}

// Get the changed files using unix 'find' and store them to a file
void get_transfers() {
  int pid;
  int pipefd[2];
  int fd;
  char data[4096];

  FILE *file = fopen(transfers_dir, "w");
  
  // Get file descriptor for pointer
  fd = fileno(file);
  
  // Redirect stdout to file in order to print exec find data
  dup2(fd,STDOUT_FILENO);
  close(fd);
  
  pipe(pipefd);

  // fork (find)
  if ((pid = fork()) == -1) {
    perror("Error find fork");
    exit(1);
  } else if (pid == 0) {
    close(pipefd[0]);
    close(pipefd[1]);
    // exec find
    execlp("find", "find", dev_html_dir, "-mtime", "-1", "-type", "f", NULL);

    perror("Error with ls -al");
    _exit(1);
  } else {
    int status;
    pid = wait(&status);
    
    close(pipefd[1]);
    // Read exec output and print to stdout (redirects to file)
    int nbytes = read(pipefd[0], data, sizeof(data));
    printf("%.*s", nbytes, data);
    close(pipefd[0]);
    
    if (WIFEXITED(status)) {
      mqd_t mq;
      char buffer[1024];
      mq = mq_open(QUEUE_NAME, O_WRONLY);
      mq_send(mq, "transfer_success", 1024, 0);
      mq_close(mq);
      
      openlog("change_tracker", LOG_PID|LOG_CONS, LOG_USER);
      syslog(LOG_INFO, "Transfer success");
      closelog();
    } else {
      mqd_t mq;
      char buffer[1024];
      mq = mq_open(QUEUE_NAME, O_WRONLY);
      mq_send(mq, "transfer_failure", 1024, 0);
      mq_close(mq);
      
      openlog("change_tracker", LOG_PID|LOG_CONS, LOG_USER);
      syslog(LOG_INFO, "Transfer failure");
      closelog();
    }
  }
}
