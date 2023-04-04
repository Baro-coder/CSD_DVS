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
int logging_init(void);
int logging_clean(void);

// -- Print log
// ---- log level: DEBUG
void log_debug(const char* owner, const char* fmt, ...);

// ---- log level: INFO
void log_info(const char* owner, const char* fmt, ...);

// ---- log level: WARNING
void log_warn(const char* owner, const char* fmt, ...);

// ---- log level: ERROR
void log_error(const char* owner, const char* fmt, ...);

// ---- log level: FATAL
void log_fatal(const char* owner, const char* fmt, ...);

#endif