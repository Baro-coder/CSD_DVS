#ifndef APP_H
#define APP_H

/* *** Libraries *** */
// -- Built-in
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>
// -- Custom
#include "logs.h"

/* *** Definitions *** */
#define PROCESSES_COUNT_DEFAULT 6
#define PROCESSES_COUNT_MIN 1
#define PROCESSES_COUNT_MAX 100

/* *** Declarations *** */
// ---- Main operations
int app_init(int argc, char** argv);
int child_processes_init();
void child_processes_wait();
int clean();

#endif