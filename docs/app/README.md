# ***App - Documentation***

## **Content:**

### 1. [Description](#description)

### 2. [Headers](#headers)

### 3. [Workflow chart](#workflow-chart)

### 4. [Component workflow](component-workflow)

---

## **Description**

This is implementation of simulation of Distributed Voting System developed in C language.

Application can handle from 3 to 100 components and malfunctioned components are in (`PROCESS COUNT` - 1 / 3) count.

Firstly master process set up all child processes and creates communication mechanisms. When all the processes are ready, master sends signal to start the voting process.

All components make their decision with random function and distribute their votes to other components through dedicated `pipe` channel.

Next all components distribute already read votes table to other components and creates votes matrix locally.

If all votes in specified column are homogenous, component is considered to be working properly. If votes are different in rows, then this component is considered to be malfunctioned and its vote is ignored (count like vote against).

After analysis component count votes for and votes against to make final decision and process ends work.

When all components processes are ended, master process closes communication mechanisms and ends work.

---

## **Headers**

### `app.h`

Master process functions and definitions. Main header file.

*Source code:*

``` c
/* *** Libraries *** */
// -- Built-in
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>
// -- Custom
#include "logs.h"

/* *** Definitions *** */
#define PROCESSES_COUNT_DEFAULT 6   // Default components process count
#define PROCESSES_COUNT_MIN 3       // Minimal components process count
#define PROCESSES_COUNT_MAX 100 // Maximal components process count
#define MASTER_NAME " App " // Name of master process to logging

/* *** Declarations *** */
// ---- Main operations

// Enable logging lib, parse run args and create sync mechanisms
int app_init(int argc, char** argv);

// Fork child processes
int child_processes_init();

// Wait for chiild processes to end
void child_processes_wait();

// Close sync mechanisms
int clean(int level);
```

### `component.h`

Component processes functions.

*Source code:*

``` c
/* *** Libraries *** */
#include "app.h"

// -- Component process main function
void* component_main(int id, int malfunctioned);
```

### `sync.h`

Inter-Process communication functions.

*Source code:*

``` c
/* *** Libraries *** */
#include "app.h"
#include "votes.h"

/* *** Definitions *** */
#define CH_READ 0   // pipe channel READ
#define CH_WRITE 1  // pipe channel WRITE
#define BUFFER_SIZE 5   // Pipe message buffer size [B]

// -- Functions

// ---- Pipes
int pipes_init();
int pipes_shutdown();

// ---- Sync (Semaphores)
int sync_init();
int sync_shutdown();

// ---- Children
// ---- * Returns randomly generated value [0, 1]
int make_vote(const int id, const char* name);

// ---- * Distributes generated vote over each component and returns `voter_count`
int distribute_vote(const int id, const char* name, int vote, int malfunctioned);

// ---- * Reads all received votes from pipe and store into `votes`
void read_votes(const int id, const char* name, int* votes);

// ---- * Distribute generated vote over each component
void distribute_votes_table(const int id, const char* name, int votes_count, int* votes_table);

// ---- * Reads all received votes tables from pipes and store into `votes_tables`
void read_votes_tables(const int id, const char* name, int votes_count, int** votes_tables);

// ---- * Analyze `votes_table`, check for malfunctions and make final decision
int make_decision(const int id, const char* name, int votes_count, int** votes_tables);
```

### `votes.h`

Votes definitions.

*Source code:*

``` c
#define VOTE_FOR 0x01       // Vote for value
#define VOTE_AGAINST 0x00   // Vote against value
```

---

## **Workflow chart**

---

## **Component workflow**

### Component main function

Component when forked, it initialized all id variables and wait for signal to start voting process.

``` c
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
```

### Component work function

All work subfunctions are implemented in `sync.c` to be able to use communication mechanism.

*Source code:*

``` c
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
    // ---- Memory allocation
    int** votes_tables = (int**) malloc(voters_count * sizeof(int*));
    for (int i = 0; i < voters_count; i++) {
        votes_tables[i] = (int*) malloc(voters_count * sizeof(int));
    }
    // ---- Pipe reading
    read_votes_tables(__id, __name, voters_count, votes_tables);

    // -- Final decision
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
```

---
