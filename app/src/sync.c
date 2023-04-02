#include "sync.h"
#include "votes.h"
#include <sys/types.h>

/* *** Global variables *** */
// -- Common
int voters_count; // Processes count

// -- Communication
int** pipes;        // Pipes array

// -- Sync
// ---- Shared memory
key_t shmkey;   // Shared memory key
int shmid;      // Shared memory ID
int* p;         // Shared var

// ---- Sempahores
sem_t** semaphores;  // Semaphores array
char ** sem_names;  // Semaphores' names


/* *** Functions *** */
// -- PRIVATE

// ---- Semaphore init
int __init_control_mechanism(int i) {
    // semaphore create
    semaphores[i] = sem_open(sem_names[i], O_CREAT | O_EXCL, 0644, 0);
    if (semaphores[i] == SEM_FAILED) {
        return 1;
    }
    return 0;
}
// ---- Semaphore shutdown
int __shutdown_control_mechanism(int i) {
    // semaphore unlink
    if(sem_unlink(sem_names[i]) < 0) {
        return 1;
    }

    // semaphore close
    if (sem_close(semaphores[i]) < 0) {
        return 2;
    }

    return 0;
}


// -- PUBLIC

// --- Pipes
// ---- Init
int pipes_init(int count) {
    pipes = (int**) malloc(count * sizeof(int*));
    for (int i = 0; i < count; i++) {
        pipes[i] = (int*) malloc(2 * sizeof(int));
        if (pipe(pipes[i]) != 0) {
            return 1;
        }
    }
    return 0;
}
// ---- Shutdown
int pipes_shutdown(int count) {
    for (int i = 0; i < count; i++) {
        close(pipes[i][CH_READ]);
        close(pipes[i][CH_WRITE]);
        free(pipes[i]);
    }
    free(pipes);

    return 0;
}

// --- Semaphores
// ---- Init
int sync_init(int count) {
    voters_count = count; // Rewirite for children

    /* ** Shared memory ** */
    // -- Key
    shmkey = ftok("/dev/shm", 5);
    // -- ID
    shmid = shmget(shmkey, sizeof(int), 0644 | IPC_CREAT);
    if (shmid < 0) {
        return 1;
    }
    // -- Attach
    p = (int*) shmat(shmid, NULL, 0);
    *p = 0;

    /* ** Semaphores ** */
    // -- Allocation
    semaphores = (sem_t**) malloc(count * sizeof(sem_t*));
    // Semaphores names
    sem_names = (char**) malloc(count * sizeof(char*));

    // -- Creation (initial value: 0)
    for(int i = 0; i < count; i++) {
        sem_names[i] = (char*) malloc(16 * sizeof(char));
        sprintf(sem_names[i], "/sem%d", i);
        __init_control_mechanism(i);
    }

    // First semaphore: initial value: 1
    sem_post(semaphores[0]);

    return 0;
}
// ---- Shutdown
int sync_shutdown(int count) {
    /* ** Shared memory ** */
    // -- Detach
    shmdt(p);
    // -- Remove
    shmctl(shmid, IPC_RMID, 0);

    /* ** Semaphores ** */
    for (int i = 0; i < count; i++) {
        __shutdown_control_mechanism(i);
        free(sem_names[i]);
    }
    free(semaphores);
    free(sem_names);

    return 0;
}

// --- Children
// ---- Make Vote with random values from `/dev/urandom`
int make_vote(int id) {
    int vote = 0;   // Vote

    // Next process semaphore id
    int next_id = id + 1;
    if (next_id == voters_count) {
        next_id = 0;
    }
    
    // Vote making
    sem_wait(semaphores[id]);

    vote = (getpid() + getppid()) % 100;

    int r = 10;
    int c;
    FILE * fp = fopen("/dev/urandom", "r");

    while(r--) {
        c = fgetc(fp) % 48;
        vote += c;
    }
    fclose(fp);

    vote %= 2;
    printf("-- * P[%d]: Vote: [%d]\n", id, vote);

    sem_post(semaphores[next_id]);
    
    return vote;
}
// ---- Distribute vote through pipes and return `voters_count`
int distribute_vote(int id, int vote) {
    // Next process semaphore id
    int next_id = id + 1;
    if (next_id == voters_count) {
        next_id = 0;
    }

    // Wait for appropriate semaphore
    sem_wait(semaphores[id]);

    printf("-- * P[%d]: Distributing vote...\n", id);

    // Vote distribution
    char* buffer = (char*) malloc(BUFFER_SIZE);
    for (int i = 0; i < voters_count; i++) {
        memset(buffer, 0, BUFFER_SIZE);
        sprintf(buffer, "%d", vote);
        write(pipes[i][CH_WRITE], buffer, BUFFER_SIZE);
    }
    free(buffer);

    // Release next semaphore
    sem_post(semaphores[next_id]);

    return voters_count;
}
// ---- Read votes from pipe
void read_votes(int id, int* votes) {
    // Next process semaphore id
    int next_id = id + 1;
    if (next_id == voters_count) {
        next_id = 0;
    }

    // Wait for appropriate semaphore
    sem_wait(semaphores[id]);

    printf("-- * P[%d]: Reading votes... [", id);

    // Votes reading
    int n_bytes = -1;
    char* buffer = (char*) malloc(BUFFER_SIZE);
    for (int i = 0; i < voters_count; i++) {
        n_bytes = read(pipes[id][CH_READ], buffer, BUFFER_SIZE);
        votes[i] = atoi(buffer);
    }
    free(buffer);

    for(int i = 0; i < voters_count; i++) {
        printf(" %d", votes[i]);
    }
    printf(" ]\n");

    // Release next semaphore
    sem_post(semaphores[next_id]);
}
// ---- Count votes FOR and AGAINST and make final decision
int make_decision(int id, int* votes) {
    // Next process semaphore id
    int next_id = id + 1;
    if (next_id == voters_count) {
        next_id = 0;
    }

    // Wait for appropriate semaphore
    sem_wait(semaphores[id]);

    printf("-- * P[%d]: Making decision... [ ", id);

    int votes_for = 0;          // Votes for counter
    int votes_against = 0;      // Votes against counter
    
    // Votes counting
    for (int i = 0; i < voters_count; i++) {
        if (votes[i] == VOTE_FOR) {
            votes_for++;
        } else if (votes[i] == VOTE_AGAINST) {
            votes_against++;
        }
    }

    printf("For: %d | Against: %d ]\n", votes_for, votes_against);

    // Release next semaphore
    sem_post(semaphores[next_id]);

    // Making decision
    if (votes_for > votes_against) {
        return VOTE_FOR;
    } else {
        return VOTE_AGAINST;
    }
}