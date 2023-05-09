/*
*  Copyright (c) 2023 Baro-coder
* 
*  This library is free software; you can redistribute it and/or modify it
*  under the terms of the GNU GENERAL PUBLIC LICENSE v3. See `logs.c` for details.
*/

/* *** Info *** */
/*
*   -- Formats --
*  Log:          [DATETIME] : [LEVEL] : [OWNER] : MESSAGE
*  Datetime:     %dd-%MM-%yyyy %hh:%mm:%ss
*  
*   -- Examples --
*   [5-04-2023 17:49:41] : [ DEBUG ] : [ App ] : Control mechanisms init...
*   [5-04-2023 17:52:32] : [ INFO  ] : [ C-4 ] : Init...
*   [5-04-2023 17:53:16] : [ INFO  ] : [ C-4 ] : Ready.
*/

#ifndef LOGS_H
#define LOGS_H

/* *** INCLUDES *** */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>


/* *** DEFINITIONS *** */

#define TIMESTAMP_BUFF_SIZE 20

// Custom boolean type
typedef int bool_t;

// -- Log levels
// Log levels enumeration
enum __log_level_t{
    LOG_LEVEL_TRACE, 
    LOG_LEVEL_DEBUG, 
    LOG_LEVEL_INFO, 
    LOG_LEVEL_WARNING, 
    LOG_LEVEL_ERROR, 
    LOG_LEVEL_FATAL};

typedef enum __log_level_t log_level_t;


/* *** INTERFACE *** */

/* --- OPTIONS */

// Creating semaphore with name as `sem_name` to sync threads printing.
//  `Attention`: Remember to call `logs_threads_safety_disable()` to remove created semaphore!
//  Returns: `0` on success; `1` - sem_open error
int logs_threads_safety_enable(const char* sem_name);

// Removing semaphore with name as `sem_name` to sync threads printing.
//  Returns: `0` on success; `1` - sem_unlink error; `2` - sem_close error
int logs_threads_safety_disable(const char* sem_name);

// Set minimal log level to `level`
void logs_set_minimal_log_level(log_level_t level);

// Set log output stream to `stream`
void logs_set_output_stream(FILE* stream);

// Set log output stream to `stderr`
void logs_set_output_stream_default();

/* --- LOG WRITERS */

// Write `level` log with `owner` and message as formatted output
void logs_log(log_level_t level, const char* owner, const char* fmt, ...);

// Write TRACE log with `owner` and message as formatted output
void logs_log_trace(const char* owner, const char* fmt, ...);

// Write DEBUG log with `owner` and message as formatted output
void logs_log_debug(const char* owner, const char* fmt, ...);

// Write INFO log with `owner` and message as formatted output
void logs_log_info(const char* owner, const char* fmt, ...);

// Write WARNING log with `owner` and message as formatted output
void logs_log_warn(const char* owner, const char* fmt, ...);

// Write ERROR log with `owner` and message as formatted output
void logs_log_error(const char* owner, const char* fmt, ...);

// Write FATAL log with `owner` and message as formatted output
void logs_log_fatal(const char* owner, const char* fmt, ...);

#endif