#include "logs.h"

// -- GLOBALS --
// -- Timer
static sem_t* mtx;

// -- Log levels
static char* __LEVEL_NAMES[] = {
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR",
    "FATAL"
};

// -- PRIVATE --
void __log_preprint(int level, const char* owner) {
    sem_wait(mtx);

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
// ---- MUTEX
int logging_init(void) {
    mtx = sem_open("/sem.liblogs", O_CREAT | O_EXCL, 0666, 1);

    return 0;
}

int logging_clean(void) {
    sem_unlink("/sem.liblogs");
    sem_close(mtx);

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

    sem_post(mtx);
}

// ---- INFO LOG
void log_info(const char* owner, const char* fmt, ...) {
    __log_preprint(LOG_LEVEL_INFO, owner);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);

    sem_post(mtx);
}

// ---- WARNING LOG
void log_warn(const char* owner, const char* fmt, ...) {
    __log_preprint(LOG_LEVEL_WARNING, owner);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);

    sem_post(mtx);
}

// ---- ERROR LOG
void log_error(const char* owner, const char* fmt, ...) {
    __log_preprint(LOG_LEVEL_ERROR, owner);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);

    sem_post(mtx);
}

// ---- FATAL LOG
void log_fatal(const char* owner, const char* fmt, ...) {
    __log_preprint(LOG_LEVEL_FATAL, owner);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);

    sem_post(mtx);
}
