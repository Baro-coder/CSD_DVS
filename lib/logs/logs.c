#include "logs.h"

// -- GLOBALS --
// -- Timer
static pthread_mutex_t mtx;

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
    // pthread_mutex_lock(&mtx);

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
    pthread_mutex_init(&mtx, NULL);
    return 0;
}

int logging_clean(void) {
    pthread_mutex_destroy(&mtx);
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

    // pthread_mutex_unlock(&mtx);
}

// ---- INFO LOG
void log_info(const char* owner, const char* fmt, ...) {
    __log_preprint(LOG_LEVEL_INFO, owner);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);

    // pthread_mutex_unlock(&mtx);
}

// ---- WARNING LOG
void log_warn(const char* owner, const char* fmt, ...) {
    __log_preprint(LOG_LEVEL_WARNING, owner);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);

    // pthread_mutex_unlock(&mtx);
}

// ---- ERROR LOG
void log_error(const char* owner, const char* fmt, ...) {
    __log_preprint(LOG_LEVEL_ERROR, owner);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);

    // pthread_mutex_unlock(&mtx);
}

// ---- FATAL LOG
void log_fatal(const char* owner, const char* fmt, ...) {
    __log_preprint(LOG_LEVEL_FATAL, owner);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    va_end(args);

    // pthread_mutex_unlock(&mtx);
}
