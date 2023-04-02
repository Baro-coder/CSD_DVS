#include "sync.h"

/* *** Global variables *** */
int** pipes;    // Pipes array

/* *** Functions *** */
// -- Pipes: Init
int __pipes_init(int count) {
    pipes = (int**) malloc(count * sizeof(int*));
    for (int i = 0; i < count; i++) {
        pipes[i] = (int*) malloc(2 * sizeof(int));
        if (pipe(pipes[i]) != 0) {
            return 1;
        }
    }
    return 0;
}

