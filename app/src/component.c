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
        printf("-- * Process[%d]: Abort!\n", id);
        __component_exit();
    } else if (signo == SIGUSR2) {
        // Voting start
        printf("-- * Process[%d]: Start Voting...\n", id);
        __votingStart = TRUE;
    }
}

// -- Voting
// ---- Decision
int __make_decision(int* votes, int votes_count) {
    int votes_for = 0;          // Votes for counter
    int votes_against = 0;      // Votes against counter
    
    // Votes counting
    for (int i = 0; i < votes_count; i++) {
        if (votes[i] == VOTE_FOR) {
            votes_for++;
        } else if (votes[i] == VOTE_AGAINST) {
            votes_against++;
        }
    }

    // Making decision
    if (votes_for > votes_against) {
        return VOTE_FOR;
    } else {
        return VOTE_AGAINST;
    }
}
// ---- Process
void __voting_process(int id) {
    // -- Make own vote
    int vote = make_vote(id);
    printf("-- * Process[%d]: Vote: %d | %d\n", id, vote, (vote % 2));
    vote %= 2;

    int voters_count = 1;
    // -- Distribute own vote
    voters_count = distribute_vote(id, vote);

    // -- Read others votes
    int* votes = (int*) malloc(voters_count * sizeof(int));
    read_votes(id, votes);

    // -- Make final decision
    int decision = -1;
    decision = __make_decision(votes, voters_count);
    printf("-- * Process[%d]: Decision: %d\n", id, decision);
}


// -- Component Process Init
void __component_init(int id) {
    // -- Signals handler
    signal(SIGINT, SIG_IGN);
    signal(SIGUSR1, __sig_handler);
    signal(SIGUSR2, __sig_handler);

    // Initial delay
    usleep(500);
    printf("-- * Process[%d]: Ready\n", id);
}
// -- Component Process Waiting
void __component_wait(int id) {
    printf("-- * Process[%d]: Waiting...\n", id);
    while(1) { 
        // Waiting for signal to
        //  - abort:            SIGUSR1
        //  - start voting:     SIGUSR2
        if (__votingStart) {
            __voting_process(id);
            break;
        }
    }
}
// -- Component Process Exit
void __component_exit() {
    int id = getpid() - getppid() - 1;
    printf("-- * Process[%d]: Exit.\n", id);
    exit(0);
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
    __component_exit();

    return NULL;
}