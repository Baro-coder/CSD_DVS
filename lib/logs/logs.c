#include "logs.h"

// -- GLOBALS --
// -- Timer
static sem_t* mtx = NULL;

// -- Log levels
const char* __LEVEL_NAMES[] = {
    " DEBUG ",
    " INFO  ",
    "WARNING",
    " ERROR ",
    " FATAL "
};

// -- PRIVATE --
void __log_preprint(int level, const char* owner) {
    if (mtx != NULL) {
        sem_wait(mtx);
    }

    time_t now;
    time(&now);
    struct tm *tm = localtime(&now);

    fprintf(stderr, "[%d-%02d-%02d %02d:%02d:%02d] : [%s] : [%s] : ", 
        tm->tm_mday, 
        tm->tm_mon + 1,
        tm->tm_year + 1900, 
        tm->tm_hour, 
        tm->tm_min, 
        tm->tm_sec, 
        __LEVEL_NAMES[level], 
        owner);
}

// -- PUBLIC --
// ---- Binary semaphore init
int logging_threads_safety_enable(const char* sem_name) {
    mtx = sem_open(sem_name, O_CREAT | O_EXCL, 0666, 1);
    if (mtx == SEM_FAILED) {
        return 1;
    }
    return 0;
}

// ---- Binary semaphore destroy
int logging_threads_safety_clear(const char* sem_name) {
    if (sem_unlink(sem_name) < 0) {
        return 1;
    }
    if (sem_close(mtx) < 0) {
        return 2;
    }

    mtx = NULL;

    return 0;
}

// ---- DEBUG LOG
void log_debug(const char* owner, const char* fmt, ...) {
    __log_preprint(LOG_LEVEL_DEBUG, owner);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);

    if(mtx != NULL) {
        sem_post(mtx);
    }
}

// ---- INFO LOG
void log_info(const char* owner, const char* fmt, ...) {
    __log_preprint(LOG_LEVEL_INFO, owner);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);

    if(mtx != NULL) {
        sem_post(mtx);
    }
}

// ---- WARNING LOG
void log_warn(const char* owner, const char* fmt, ...) {
    __log_preprint(LOG_LEVEL_WARNING, owner);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);

    if(mtx != NULL) {
        sem_post(mtx);
    }
}

// ---- ERROR LOG
void log_error(const char* owner, const char* fmt, ...) {
    __log_preprint(LOG_LEVEL_ERROR, owner);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);

    if(mtx != NULL) {
        sem_post(mtx);
    }
}

// ---- FATAL LOG
void log_fatal(const char* owner, const char* fmt, ...) {
    __log_preprint(LOG_LEVEL_FATAL, owner);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);

    if(mtx != NULL) {
        sem_post(mtx);
    }
}
