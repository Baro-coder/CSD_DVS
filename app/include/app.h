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
#define PROCESSES_COUNT_DEFAULT 6   // Default components process count
#define PROCESSES_COUNT_MIN 3       // Minimal components process count
#define PROCESSES_COUNT_MAX 100 // Maximal components process count
#define MASTER_NAME " App " // Name of master process to logging

/* *** Declarations *** */
// ---- Main operations

// Enable logging lib, parse run args and create sync mechanisms
int app_init(int argc, char** argv);

// Fork child processes
int child_processes_init();

// Wait for chiild processes to end
void child_processes_wait();

// Close sync mechanisms
int clean(int level);

#endif