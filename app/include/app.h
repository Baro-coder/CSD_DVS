#ifndef APP_H
#define APP_H

/* *** Libraries *** */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

/* *** Definitions *** */
#define THREADS_COUNT_DEFAULT 6

/* *** Declarations *** */
// ---- Main operations
int app_init(int argc, char** argv);
int child_processes_init();
int child_processes_wait();

#endif