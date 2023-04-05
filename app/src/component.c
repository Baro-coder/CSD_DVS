#include "component.h"
#include "sync.h"
#include "votes.h"

#define NAME_BUFF_SIZE 8

/* *** Declarations *** */
void __component_exit(int status);


/* *** Global Variables *** */
int __id = -1;
int __malfunctioned = 0;
int __votingStart = 0;
char __name[NAME_BUFF_SIZE];

/* *** Functions*** */
// PRIVATE
// -- Signal handler
void __sig_handler(int signo) {
    if (signo == SIGUSR1) {
        // Abort
        log_info(__name, "Abort!");
        __component_exit(-1);
    } else if (signo == SIGUSR2) {
        // Voting start
        __votingStart = 1;
    }
}

// -- Voting
// ---- Process
void __voting_process() {
    // -- Make own vote
    int vote = make_vote(__id, __name);

    int voters_count = 1;
    // -- Distribute own vote
    voters_count = distribute_vote(__id, __name, vote, __malfunctioned);

    // -- Read others votes
    int* votes = (int*) malloc(voters_count * sizeof(int));
    read_votes(__id, __name, votes);

    // -- Distribute votes table
    distribute_votes_table(__id, __name, voters_count, votes);

    // -- Read others votes tables
    int** votes_tables = (int**) malloc(voters_count * sizeof(int*));
    for (int i = 0; i < voters_count; i++) {
        votes_tables[i] = (int*) malloc(voters_count * sizeof(int));
    }

    read_votes_tables(__id, __name, voters_count, votes_tables);

    // -- Make final decision
    int decision = -1;
    decision = make_decision(__id, __name, voters_count, votes_tables);

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
    // -- ID and NAME
    memset(__name, 0, NAME_BUFF_SIZE);
    if(malfunctioned) {
        sprintf(__name, "*C-%d ", id);
    } else {
        sprintf(__name, " C-%d ", id);
    }

    log_info(__name, "Init...");

    __id = id;

    // -- Malfuntion
    __malfunctioned = malfunctioned;

    // -- Signals handler
    signal(SIGINT, SIG_IGN);
    signal(SIGUSR1, __sig_handler);
    signal(SIGUSR2, __sig_handler);
}
// -- Component Process Waiting
void __component_wait() {
    log_info(__name, "Ready.");

    while(1) { 
        // Waiting for signal to
        //  - abort:            SIGUSR1
        //  - start voting:     SIGUSR2
        if (__votingStart) {
            __voting_process();
        }
    }
}
// -- Component Process Exit
void __component_exit(int status) {
    

    log_info(__name, "Exit: Decision => %d", status);
    exit(status);
}


// PUBLIC
// -- Component Main Driver
void* component_main(int id, int malfunctioned) {
    /* *** INIT *** */
    __component_init(id, malfunctioned);

    /* *** WORK *** */
    __component_wait();

    /* *** EXIT *** */
    __component_exit(0);

    return NULL;
}