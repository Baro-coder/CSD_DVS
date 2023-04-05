#include "sync.h"

/* *** Global variables *** */
// -- Common
int voters_count; // Processes count

// -- Communication
int** pipes;        // Pipes array

// -- Sync
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
    // semaphore close
    if (sem_close(semaphores[i]) < 0) {
        return 2;
    }
    // semaphore unlink
    if(sem_unlink(sem_names[i]) < 0) {
        return 1;
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
    for (int i = count - 1; i >= 0; i--) {
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
    /* ** Semaphores ** */
    for (int i = count - 1; i >= 0; i--) {
        __shutdown_control_mechanism(i);
        free(sem_names[i]);
    }
    free(semaphores);
    free(sem_names);

    return 0;
}

// --- Components
int make_vote(const int id, const char* name) {
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
    log_info(name, "Vote: %d", vote);

    sem_post(semaphores[next_id]);
    
    return vote;
}

int distribute_vote(const int id, const char* name, int vote, int malfunctioned) {
    // Next process semaphore id
    int next_id = id + 1;
    if (next_id == voters_count) {
        next_id = 0;
    }

    // Wait for appropriate semaphore
    sem_wait(semaphores[id]);

    log_info(name, "Distributing vote...");

    // Vote distribution
    char* buffer = (char*) malloc(BUFFER_SIZE);

    for (int i = 0; i < voters_count; i++) {
        memset(buffer, 0, BUFFER_SIZE);

        if (malfunctioned) {
            sprintf(buffer, "%d", ((vote + i) % 2));
        } else {
            sprintf(buffer, "%d", vote);
        }
        write(pipes[i][CH_WRITE], buffer, BUFFER_SIZE);
    }
    free(buffer);

    // Release next semaphore
    sem_post(semaphores[next_id]);

    return voters_count;
}

void read_votes(const int id, const char* name, int* votes) {
    // Next process semaphore id
    int next_id = id + 1;
    if (next_id == voters_count) {
        next_id = 0;
    }

    // Wait for appropriate semaphore
    sem_wait(semaphores[id]);

    log_info(name, "Reading votes...");

    // Votes reading
    int n_bytes = -1;
    char* buffer = (char*) malloc(BUFFER_SIZE);
    for (int i = 0; i < voters_count; i++) {
        memset(buffer, 0, BUFFER_SIZE);
        n_bytes = read(pipes[id][CH_READ], buffer, BUFFER_SIZE);
        if(n_bytes <= 0) {
            log_fatal(name, "Pipe read error! n_bytes = %d", n_bytes);
            kill(getppid(), SIGUSR1);
        }
        votes[i] = atoi(buffer);
    }
    free(buffer);

    // Release next semaphore
    sem_post(semaphores[next_id]);
}

void distribute_votes_table(const int id, const char* name, int votes_count, int* votes_table) {
    // Next process semaphore id
    int next_id = id + 1;
    if (next_id == votes_count) {
        next_id = 0;
    }

    // Wait for appropriate semaphore
    sem_wait(semaphores[id]);

    log_info(name, "Distributing votes table...");

    // Votes table distribution
    char* buffer = (char*) malloc(BUFFER_SIZE);

    for (int i = 0; i < votes_count; i++) {
        for (int j = 0; j < votes_count; j++) {
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "%d", votes_table[i]);
            write(pipes[j][CH_WRITE], buffer, BUFFER_SIZE);
        }
    }
    free(buffer);

    // Release next semaphore
    sem_post(semaphores[next_id]);
}

void read_votes_tables(const int id, const char* name, int votes_count, int** votes_tables) {
    // Next process semaphore id
    int next_id = id + 1;
    if (next_id == votes_count) {
        next_id = 0;
    }

    // Wait for appropriate semaphore
    sem_wait(semaphores[id]);

    log_info(name, "Reading votes tables...");

    // Votes reading
    int n_bytes = -1;
    char* buffer = (char*) malloc(BUFFER_SIZE);

    for(int i = 0; i < votes_count; i++) {
        for (int j = 0; j < votes_count; j++) {
            memset(buffer, 0, BUFFER_SIZE);
            n_bytes = read(pipes[id][CH_READ], buffer, BUFFER_SIZE);
            if(n_bytes <= 0) {
                log_fatal(name, "Pipe read error! n_bytes = %d", n_bytes);
                kill(getppid(), SIGUSR1);
            }
            votes_tables[i][j] = atoi(buffer);
        }
    }
    free(buffer);

    // Release next semaphore
    sem_post(semaphores[next_id]);
}

int make_decision(const int id, const char* name, int votes_count, int** votes_tables) {
    // Next process semaphore id
    int next_id = id + 1;
    if (next_id == voters_count) {
        next_id = 0;
    }

    // // Wait for appropriate semaphore
    sem_wait(semaphores[id]);

    log_info(name, "Making decision...");
    
    log_debug(name, "Checking for malfunctions...");

    int* malfunctions = (int*) malloc(votes_count * sizeof(int));
    memset(malfunctions, 0, votes_count * sizeof(int));

    int i = 1;

    for(; i < votes_count; i++) {
        for(int j = 0; j < votes_count; j++) {
            if(votes_tables[0][j] != votes_tables[i][j]) {
                malfunctions[j]++;
            }
        }
    }

    int votes_for = 0;
    int votes_against = 0;
    int votes_ignored = 0;

    log_debug(name, "Votes counting...");
    for (i = 0; i < votes_count; i++) {
        if (malfunctions[i] == 0) {
            if (votes_tables[0][i] == VOTE_FOR) {
                votes_for++;
            } else {
                votes_against++;
            }
        } else {
            votes_ignored++;
        }
    }

    free(malfunctions);

    // Release next semaphore
    sem_post(semaphores[next_id]);

    // Making decision
    log_debug(name, "Counted votes: FOR(%d), AGAINST(%d), IGNORED(%d)", 
        votes_for,
        votes_against,
        votes_ignored);
    
    if (votes_for > (votes_against + votes_ignored)) {
        return VOTE_FOR;
    } else {
        return VOTE_AGAINST;
    }
}
