#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "app.h"


int main(int argc, char** argv) {
    
    /* *** Program init *** */
    int threads_count;
    
    if (argc == 1) {
        threads_count = THREADS_COUNT_DEFAULT;
    } else if (argc == 2) {
        threads_count = atoi(argv[1]);
    } else {
        printf("Invalid arguments!\n");
        return 1;
    }

    if (threads_count == 0) {
        printf("Invalid arguments!\n");
        return 1;
    }

    printf("Threads: %d\n\n", threads_count);

    // Threads dynamic array
    pthread_t *threads = (pthread_t*)malloc(threads_count * sizeof(pthread_t));
    // Threads starting args
    int* pthread_args = (int*)malloc(threads_count * sizeof(int));

    /* *** Threads create and start *** */
    int ret = 0;
    int i = 0;
    for(; i < threads_count; i++) {
        pthread_args[i] = i;
        ret = pthread_create(&threads[i], NULL, component_main, (void*)&pthread_args[i]);
        if (ret) {
            printf("pthread_create : [%d] : Failure!\n", i);
        }
    }

    /* *** Threads join *** */
    for(i = 0; i < threads_count; i++) {
        ret = pthread_join(threads[i], NULL);
        if (ret) {
            printf("pthread_join : [%d] : Failure!\n", i);
        }
    }
    
    return 0;
}
