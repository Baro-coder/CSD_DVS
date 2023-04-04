#include "component.h"
#include "sync.h"
#include "votes.h"

/* *** Declarations *** */
void __component_exit();


/* *** Global Variables *** */
int __malfunctioned = 0;
int __votingStart = 0;


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
        __votingStart = 1;
    }
}

// -- Voting
// ---- Process
void __voting_process(int id) {
    // -- Make own vote
    int vote = make_vote(id);

    int voters_count = 1;
    // -- Distribute own vote
    voters_count = distribute_vote(id, vote, __malfunctioned);

    // -- Read others votes
    int* votes = (int*) malloc(voters_count * sizeof(int));
    read_votes(id, votes);

    // -- Distribute votes table
    distribute_votes_table(id, voters_count, votes);

    // -- Read others votes tables
    int** votes_tables = (int**) malloc(voters_count * sizeof(int*));
    for (int i = 0; i < voters_count; i++) {
        votes_tables[i] = (int*) malloc(voters_count * sizeof(int));
    }

    read_votes_tables(id, voters_count, votes_tables);

    // -- Make final decision
    int decision = -1;
    decision = make_decision(id, voters_count, votes_tables);
    // printf("-- * P[%d]: Decision: %d\n", id, decision);

    // -- Memory free
    free(votes);
    for (int i = 0; i < voters_count; i++) {
        free(votes_tables[i]);
    }
    free(votes_tables);

    // -- Exit with: status == decision
    __component_exit(decision);
}


// -- Component Process Init
void __component_init(int id, int malfunctioned) {
    printf("-- * P[%d]: Init.\n", id);

    // -- Malfuntion
    __malfunctioned = malfunctioned;

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
void* component_main(int malfunctioned) {
    // Current thread ID
    int id = getpid() - getppid() - 1;

    /* *** INIT *** */
    __component_init(id, malfunctioned);

    /* *** WORK *** */
    __component_wait(id);

    /* *** EXIT *** */
    __component_exit(0);

    return NULL;
}