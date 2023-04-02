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
        if (pids[i] != -1) {
            kill(pids[i], signo);
        }
    }
}

// -- Signals: Handler
void __master_sig_handler(int signo) {
    if(signo == SIGINT) {
        fprintf(stdout, "\n-- App: Abort!\n");
        __send_broadcast_signal(SIGUSR1);
    } else if (signo == SIGUSR1) {
        fprintf(stdout, "\n-- App: Abort!\n");
        __send_broadcast_signal(signo);
    } else if (signo == SIGUSR2) {
        fprintf(stdout, "\n-- App: Start Voting.\n\n");
        __send_broadcast_signal(signo);
    }
}

// -- App: Main init
int app_init(int argc, char** argv) {
    // -- Args parse: processesCount
    if (argc == 1) {
        processesCount = THREADS_COUNT_DEFAULT;
    } else if (argc == 2) {
        processesCount = atoi(argv[1]);
        if (processesCount == 0) {
            return 3;
        }
    } else {
        return 3;
    }

    // -- Signals handler
    signal(SIGINT, __master_sig_handler);
    signal(SIGUSR1, __master_sig_handler);
    signal(SIGUSR2, __master_sig_handler);

    // -- Pipes
    if (pipes_init(processesCount)) {
        return 1;
    }

    // -- Semaphores
    if (sync_init(processesCount)) {
        return 2;
    }

    return 0;
}

// -- App: Children init
int child_processes_init() {
    // Children PIDs array
    pids = (pid_t*) malloc(processesCount * sizeof(pid_t));

    // PIDs array preconfigure
    int i = 0;
    for(; i < processesCount; i++) {
        pids[i] = -1;
    }

    /* *** Children create and start *** */
    pid_t pid = -1;
    for(i = 0; i < processesCount; i++) {
        pid = fork();
        if (pid == 0) { 
            // Child process
            component_main();
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
int clean() {
    /* ** Cleaning ** */
    // -- Pipes shutdown
    if (pipes_shutdown(processesCount) != 0) {
        return 1;
    }

    // -- Semaphores shutdown
    if (sync_shutdown(processesCount) != 0) {
        return 2;
    }

    return 0;
}