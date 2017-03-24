/*
  Authour: Alex Kiernan
  Date: 14/03/17
  
  Desc: Module to lock and unlock dev dir.
**/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <syslog.h>
#include "routes.h"
#include "lock_unlock.h"

void lock_dir() {
  char mode[4] = "0555";
  int i = strtol(mode, 0, 8);
  
  if (chmod(dev_dir, i) == 0) {
    openlog("change_tracker", LOG_PID|LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "Directory locked");
    closelog();
  } else {
    openlog("change_tracker", LOG_PID|LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "Error locking directory");
    closelog();
  }
}

void unlock_dir() {
  char mode[4] = "0777";
  int i = strtol(mode, 0, 8);
  if (chmod(dev_dir, i) == 0) {
    openlog("change_tracker", LOG_PID|LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "Directory unlocked");
    closelog();
  } else {
    openlog("change_tracker", LOG_PID|LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "Error unlocking directory");
    closelog();
  }
}
