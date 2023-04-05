#include "app.h"
#include "component.h"
#include "logs.h"
#include "sync.h"

/* *** Global variables *** */
int processesCount;     // processes count
pid_t* pids;     // PIDs array

/* *** Functions *** */
// -- Signals: Broadcast
void __send_broadcast_signal(int signo) {
    for(int i = 0; i < processesCount; i++) {
        if (pids[i] != -1) {
            kill(pids[i], signo);
        }
    }
}

// -- Signals: Handler
void __master_sig_handler(int signo) {
    if(signo == SIGINT) {
        log_info(MASTER_NAME, "Signal received: %d | Abort!", signo);
        __send_broadcast_signal(SIGUSR1);
    } else if (signo == SIGUSR1) {
        log_error(MASTER_NAME, "Signal received: %d | Abort!", signo);
        __send_broadcast_signal(signo);
    } else if (signo == SIGUSR2) {
        log_info(MASTER_NAME, "Signal received: %d | Voting start.", signo);
        __send_broadcast_signal(signo);
    } else {
        log_warn(MASTER_NAME, "Signal received: %d | Signal ignored.", signo);
    }
}

// -- App: Main init
int app_init(int argc, char** argv) {
    log_info(MASTER_NAME, "Main init...");

    // -- liblogs thread-safety semaphore create
    if (logging_threads_safety_enable("/sem.liblogs") != 0) {
        return 1;
    }

    // -- Args parse: processesCount
    log_debug(MASTER_NAME, "Args parsing...");
    if (argc == 1) {
        processesCount = PROCESSES_COUNT_DEFAULT;
    } else if (argc == 2) {
        processesCount = atoi(argv[1]);
        if (processesCount <  PROCESSES_COUNT_MIN || processesCount > PROCESSES_COUNT_MAX) {
            return 2;
        }
    } else {
        return 2;
    }

    // -- Signals handler
    log_debug(MASTER_NAME, "Signals handling...");
    signal(SIGINT, __master_sig_handler);
    signal(SIGUSR1, __master_sig_handler);
    signal(SIGUSR2, __master_sig_handler);

    // -- Pipes
    log_debug(MASTER_NAME, "Pipes init...");
    if (pipes_init(processesCount)) {
        return 3;
    }

    // -- Semaphores
    log_debug(MASTER_NAME, "Control mechanisms init...");
    if (sync_init(processesCount)) {
        return 4;
    }

    return 0;
}

// -- App: Children init
int child_processes_init() {
    log_info(MASTER_NAME, "Components Pre-init...");

    // Children PIDs array
    pids = (pid_t*) malloc(processesCount * sizeof(pid_t));

    // PIDs array preconfigure
    int i = 0;
    for(; i < processesCount; i++) {
        pids[i] = -1;
    }

    /* *** Children create and start *** */
    // -- Malfunctioned component count
    double a = (((double)processesCount - 1) / 3);
    int mal_count = (int)(a);
    int malfunctioned = 0;

    log_debug(MASTER_NAME, "Malfunctioned components count: %d", mal_count);

    // -- Children fork
    srand(time(NULL));
    pid_t pid = -1;
    for(i = 0; i < processesCount; i++) {
        malfunctioned = 0;
        if (mal_count > 0) {
            mal_count--;
            malfunctioned = 1;
        }

        pid = fork();
        if (pid == 0) { 
            // Child process
            component_main(i, malfunctioned);
            return 0;
        } else if (pid == -1) {
            // fork(): error -- Abort
            __send_broadcast_signal(SIGINT);
            return 1;
        } else {
            // Parent process
            pids[i] = pid;
        }
    }
    return 0;
}

// -- App: Children wait
void child_processes_wait() {
    /* ** Voting init ** */
    // -- Delay (for children init)
    sleep(1);
    // -- Voting start signal
    kill(getpid(), SIGUSR2);

    /* ** Wait for children processes ** */
    for(int i = 0; i < processesCount; i++) {
        wait(NULL);
    }
}

// -- App: Resources clean
int clean(int level) {
    /* ** Cleaning ** */
    log_info(MASTER_NAME, "Cleaning... (%d)", level);
    
    // -- Memory
    free(pids);

    // -- liblogs thread-safety semaphore destroy
    if (level >= 1) {
        log_debug(MASTER_NAME, "Logging thread-safety clear...");
        int logthcl_stat = logging_threads_safety_clear("/sem.liblogs");
        if (logthcl_stat == 1) {
            return 1;
        } else if (logthcl_stat == 2) {
            return 2;
        }
    }
    
    // -- Pipes shutdown
    if (level >= 2) {
        log_debug(MASTER_NAME, "Pipes closing...");
        if (pipes_shutdown(processesCount) != 0) {
            return 3;
        }
    } else {
        return 0;
    }

    // -- Semaphores shutdown
    if (level == 3) {
        log_debug(MASTER_NAME, "Control mechanisms closing...");
        if (sync_shutdown(processesCount) != 0) {
            return 4;
        } else {
            return 0;
        }
    } else {
        return 5;
    }
}