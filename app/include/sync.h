#ifndef SYNC_H
#define SYNC_H

/* *** Libraries *** */
#include "app.h"

/* *** Definitions *** */
#define CH_READ 0   // pipe channel READ
#define CH_WRITE 1  // pipe channel WRITE
#define TRUE 1      // bool True  value
#define FALSE 0     // bool False value

/* *** Declarations *** */
// -- Functions
int __pipes_init();


#endif