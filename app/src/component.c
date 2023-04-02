#include "component.h"
#include "sync.h"

/* *** Global Variables *** */
int __abort = FALSE;
int __votingStart = FALSE;

/* *** Private process functions *** */
// -- Signal handler
void __sig_handler(int signo) {
    int id = getpid() - getppid() - 1;
    
    if (signo == SIGUSR1) {
        // Abort
        printf("Process[%d]: Abort\n", id);
        __abort = TRUE;
    } else if (signo == SIGUSR2) {
        // Voting start
        printf("Process[%d]: Start Voting\n", id);
        __votingStart = TRUE;
    }
}


// -- Voting
void __vote(int id) {
    int own_vote = prng_rand_int();
    printf("Process[%d]: Vote: %d | %d\n", id, own_vote, own_vote % 2);
}


/* *** Main milestones *** */
void __component_init(int id) {
    printf("Process[%d]: Init\n", id);

    // -- Signals handler
    signal(SIGINT, SIG_IGN);
    signal(SIGUSR1, __sig_handler);
    signal(SIGUSR2, __sig_handler);
}

void __component_work(int id) {
    printf("Process[%d]: Ready.\n", id);
    while(1) { 
        // Waiting for signal to
        //  - abort:            SIGUSR1
        //  - start voting:     SIGUSR2
        if(__abort) {
            break;
        } else if (__votingStart) {
            __vote(id);
            break;
        }
    }
}

void __component_exit(int id) {
    printf("Process[%d]: Exit\n", id);
}

/* *** Component Main Driver *** */
void* component_main() {
    // Current thread ID
    int id = getpid() - getppid() - 1;

    /* *** INIT *** */
    __component_init(id);

    /* *** WORK *** */
    __component_work(id);

    /* *** EXIT *** */
    __component_exit(id);

    return NULL;
}