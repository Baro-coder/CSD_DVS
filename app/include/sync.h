#ifndef SYNC_H
#define SYNC_H

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


#endif