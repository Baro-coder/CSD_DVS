#include "component.h"
#include "sync.h"
#include "votes.h"

/* *** Declarations *** */
void __component_exit();


/* *** Global Variables *** */
int __votingStart = FALSE;


/* *** Functions*** */
// PRIVATE
// -- Signal handler
void __sig_handler(int signo) {
    int id = getpid() - getppid() - 1;
    
    if (signo == SIGUSR1) {
        // Abort
        printf("-- * P[%d]: Abort!\n", id);
        __component_exit(-1);
    } else if (signo == SIGUSR2) {
        // Voting start
        __votingStart = TRUE;
    }
}

// -- Voting
// ---- Process
void __voting_process(int id) {
    // -- Make own vote
    int vote = make_vote(id);

    int voters_count = 1;
    // -- Distribute own vote
    voters_count = distribute_vote(id, vote);

    // -- Read others votes
    int* votes = (int*) malloc(voters_count * sizeof(int));
    read_votes(id, votes);

    // -- Make final decision
    int decision = -1;
    decision = make_decision(id, votes);
    // printf("-- * P[%d]: Decision: %d\n", id, decision);

    // -- Exit with: status == decision
    __component_exit(decision);
}


// -- Component Process Init
void __component_init(int id) {
    printf("-- * P[%d]: Init.\n", id);

    // -- Signals handler
    signal(SIGINT, SIG_IGN);
    signal(SIGUSR1, __sig_handler);
    signal(SIGUSR2, __sig_handler);
}
// -- Component Process Waiting
void __component_wait(int id) {
    printf("-- * P[%d]: Ready.\n", id);
    while(1) { 
        // Waiting for signal to
        //  - abort:            SIGUSR1
        //  - start voting:     SIGUSR2
        if (__votingStart) {
            __voting_process(id);
        }
    }
}
// -- Component Process Exit
void __component_exit(int status) {
    int id = getpid() - getppid() - 1;
    printf("-- * P[%d]: Exit: Decision: [%d]\n", id, status);
    exit(status);
}


// PUBLIC
// -- Component Main Driver
void* component_main() {
    // Current thread ID
    int id = getpid() - getppid() - 1;

    /* *** INIT *** */
    __component_init(id);

    /* *** WORK *** */
    __component_wait(id);

    /* *** EXIT *** */
    __component_exit(0);

    return NULL;
}