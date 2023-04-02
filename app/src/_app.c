#include "app.h"
#include "component.h"
#include "sync.h"

/* *** Global variables *** */
int processesCount;     // processes count
pid_t* pids;            // PIDs array

/* *** Functions *** */
// -- Signals: Broadcast
void __send_broadcast_signal(int signo) {
    for(int i = 0; i < processesCount; i++) {
        kill(pids[i], signo);
    }
}

// -- Signals: Handler
void __master_sig_handler(int signo) {
    if(signo == SIGINT) {
        printf("MASTER[%d]: Sending broadcast: %d\n", getpid(), signo);
        __send_broadcast_signal(SIGUSR1);
    } else if (signo == SIGUSR1) {
        printf("MASTER[%d]: Sending broadcast: %d\n", getpid(), signo);
        __send_broadcast_signal(signo);
    } else if (signo == SIGUSR2) {
        printf("MASTER[%d]: Sending broadcast: %d\n", getpid(), signo);
        __send_broadcast_signal(signo);
    }
}

// -- App: Main init
int app_init(int argc, char** argv) {
    // -- Args parse: processes count
    if (argc == 1) {
        processesCount = THREADS_COUNT_DEFAULT;
    } else if (argc == 2) {
        processesCount = atoi(argv[1]);
        if (processesCount == 0) {
            fprintf(stdout, "Failure!\n");
            fprintf(stderr, "-- Error: Invalid argument!\n");
            return -1;
        }
    } else {
        fprintf(stdout, "Failure!\n");
        fprintf(stderr, "-- Error: Invalid argument!\n");
        return -1;
    }

    // -- Signals handler
    signal(SIGINT, __master_sig_handler);
    signal(SIGUSR1, __master_sig_handler);
    signal(SIGUSR2, __master_sig_handler);

    // -- Pipes
    if (__pipes_init(processesCount)) {
        fprintf(stdout, "Failure!\n");
        fprintf(stderr, "-- Error: Pipes create!\n");
        return -2;
    }

    return 0;
}

// -- App: Children init
int child_processes_init() {
    // Threads dynamic array
    pids = (pid_t*) malloc(processesCount * sizeof(pid_t));
    // Threads create func result
    int res = 0;

    /* *** Threads create and start *** */
    int i = 0;
    for(; i < processesCount; i++) {
        pids[i] = fork();
        if (pids[i] == 0) { 
            // Child process
            component_main();
            exit(0);
        }
    }

    return 0;
}

// -- App: Children wait
int child_processes_wait() {
    for(int i = 0; i < processesCount; i++) {
        wait(NULL);
    }
    return 0;
}