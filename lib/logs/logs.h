#ifndef LOGS_H
#define LOGS_H

/* *** Includes *** */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>

/* *** Definitions *** */
// -- Log levels
#define LOG_LEVEL_DEBUG     0
#define LOG_LEVEL_INFO      1
#define LOG_LEVEL_WARNING   2
#define LOG_LEVEL_ERROR     3
#define LOG_LEVEL_FATAL     4


/* *** Functions *** */
// -- Sync

// Creating semaphore with name as `sem_name` to sync threads printing. 
//  Returns: `0` on success; `1` - sem_open error
int logging_threads_safety_enable(const char* sem_name);

// Removing semaphore with name as `sem_name` to sync threads printing.
//  Returns: `0` on success; `1` - sem_unlink error; `2` - sem_close error
int logging_threads_safety_clear(const char* sem_name);


// log level: DEBUG
void log_debug(const char* owner, const char* fmt, ...);

// log level: INFO
void log_info(const char* owner, const char* fmt, ...);

// log level: WARNING
void log_warn(const char* owner, const char* fmt, ...);

// log level: ERROR
void log_error(const char* owner, const char* fmt, ...);

// log level: FATAL
void log_fatal(const char* owner, const char* fmt, ...);

#endif