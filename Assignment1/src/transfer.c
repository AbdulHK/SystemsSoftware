
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
  // stop process for a while to run this function
  sleep(5);
  
  // debug: bug appeard when using the global versions.
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
  
  while(fscanf(fp, "%s", records[i]) != EOF) {
    i++;
  }
  
  fclose(fp);
  
  num_lines = i;
  
  //copy data
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

// use "find" to get changes
void get_transfers() {
  int pid;
  int pipefd[2];
  int fd;
  char data[4096];

  FILE *file = fopen(transfers_dir, "w");
  
  fd = fileno(file);
  
  dup2(fd,STDOUT_FILENO);
  close(fd);
  
  pipe(pipefd);

  
  if ((pid = fork()) == -1) {
    perror("Error find fork");
    exit(1);
  } else if (pid == 0) {
    close(pipefd[0]);
    close(pipefd[1]);
    execlp("find", "find", dev_html_dir, "-mtime", "-1", "-type", "f", NULL);

    perror("Error with ls -al");
    _exit(1);
  } else {
    int status;
    pid = wait(&status);
    
    close(pipefd[1]);
    
    int nbytes = read(pipefd[0], data, sizeof(data));
    printf("%.*s", nbytes, data);
    close(pipefd[0]);
    
    if (WIFEXITED(status)) {
      //get queue
      mqd_t mq;
      char buffer[1024];
      mq = mq_open(QUEUE_NAME, O_WRONLY);
      mq_send(mq, "transfer_success", 1024, 0);
      mq_close(mq);
      
      openlog("Change_tracker", LOG_PID|LOG_CONS, LOG_USER);
      syslog(LOG_INFO, "Transfer success");
      closelog();
    } else {
      mqd_t mq;
      char buffer[1024];
      mq = mq_open(QUEUE_NAME, O_WRONLY);
      mq_send(mq, "transfer_failure", 1024, 0);
      mq_close(mq);
      
      openlog("Change_tracker", LOG_PID|LOG_CONS, LOG_USER);
      syslog(LOG_INFO, "Transfer failure");
      closelog();
    }
  }
}
