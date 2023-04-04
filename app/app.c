/* *** Main header *** */
#include "app.h"

int p_func(int id) {
    char name[5];
    sprintf(name, "C(%d)", id);

    log_info(name, "Init.");

    log_info(name, "Ready.");
    sleep(2);

    int status = id % 2;

    log_info(name, "Exit (%d).", status);

    return status;
}

int test_main(int argc, char** argv) {
    int count = PROCESSES_COUNT_DEFAULT;

    if (argc == 2) {
        count = atoi(argv[1]);
    }

    logging_threads_safety_enable("/sem.liblogs");

    log_info("App", "Init...");

    pid_t pids[count];
    pid_t pid = -1;

    int i = 0;
    
    log_info("App", "Forking...");
    for (; i < count; i++) {
        pid = fork();
        if(pid == 0) {
            exit(p_func(i));
        } else if (pid == -1) {
            log_error("App", "Fork error: i = %d", i);
        } else {
            pids[i] = pid;
        }
    }

    log_info("App", "Waiting...");
    int stats[count];
    int stat = -1;
    for(i = 0; i < count; i++) {
        waitpid(pids[i], &stat, 0);
        stats[i] = stat / 256;
    }

    log_info("App", "Getting results...");
    printf(" -- * Stats: [");
    for(i = 0; i < count; i++) {
        printf(" %d", stats[i]);
    }
    printf(" ]\n");

    log_info("App", "Exit.");
    
    logging_threads_safety_clear("/sem.liblogs");

    return 0;
}

/* *** Main driver *** */
int main(int argc, char** argv) {
    
    return test_main(argc, argv);

    /* *** OLD CODE *** */

    int status = 0;
    
    /* *** Init *** */
    // fprintf(stdout, "-- App: Init: ");
    // status = app_init(argc, argv);
    // if (status != 0) {
    //     fprintf(stdout, "Failure: %d\n", status);
    //     exit(status);
    // }
    // fprintf(stdout, "Done.\n");

    // /* *** Children init *** */
    // fprintf(stdout, "-- App: Children init:\n");
    // status = child_processes_init();
    // if (status != 0) {
    //     fprintf(stdout, "Failure: %d\n", status);
    //     exit(status);
    // }

    // /* *** Children wait *** */
    // child_processes_wait();

    // /* *** Cleaning *** */
    // fprintf(stdout, "\n-- App: Cleaning: ");
    // status = clean();
    // if (status != 0) {
    //     fprintf(stdout, "Failure: %d\n", status);
    //     exit(status);
    // }
    // fprintf(stdout, "Done.\n");

    // /* *** Exit *** */
    // fprintf(stdout, "-- App: Exit.\n");

    // exit(0);

    /* *** OLD CODE *** */
}
