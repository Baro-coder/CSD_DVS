#ifndef SYNC_H
#define SYNC_H

/* *** Libraries *** */
#include "app.h"

/* *** Definitions *** */
#define CH_READ 0   // pipe channel READ
#define CH_WRITE 1  // pipe channel WRITE

#define TRUE 1      // bool True  value
#define FALSE 0     // bool False value

#define BUFFER_SIZE 5   // Pipe message buffer size [B]


/* *** Declarations *** */
// -- Functions

// ---- Pipes
int pipes_init();
int pipes_shutdown();

// ---- Sync (Semaphores)
int sync_init();
int sync_shutdown();

// ---- Children
int make_vote(int id);
int distribute_vote(int id, int vote);
void read_votes(int id, int* votes);
int make_decision(int id, int* votes);

#endif