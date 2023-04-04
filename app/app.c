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

    log_info(MASTER_NAME, "Init...");

    pid_t pids[count];
    pid_t pid = -1;

    int i = 0;
    
    log_info(MASTER_NAME, "Forking...");
    for (; i < count; i++) {
        pid = fork();
        if(pid == 0) {
            exit(p_func(i));
        } else if (pid == -1) {
            log_error(MASTER_NAME, "Fork error: i = %d", i);
        } else {
            pids[i] = pid;
        }
    }

    log_info(MASTER_NAME, "Waiting...");
    int stats[count];
    int stat = -1;
    for(i = 0; i < count; i++) {
        waitpid(pids[i], &stat, 0);
        stats[i] = stat / 256;
    }

    log_info(MASTER_NAME, "Getting results...");
    printf(" -- * Stats: [");
    for(i = 0; i < count; i++) {
        printf(" %d", stats[i]);
    }
    printf(" ]\n");

    log_info(MASTER_NAME, "Exit.");
    
    logging_threads_safety_clear("/sem.liblogs");

    return 0;
}

/* *** Main driver *** */
int main(int argc, char** argv) {
    int status = -1;
    int aux_status = -1;
    
    /* *** Init *** */
    status = app_init(argc, argv);
    if (status == 1) {
        log_error(MASTER_NAME, "Logging thread-safety init! Code: %d", status);
        exit(status);
    } else if (status == 2) {
        log_error(MASTER_NAME, "Invalid arguments error! Code: %d", status);
        aux_status = clean(1);
        if (aux_status != 0) {
            log_error(MASTER_NAME, "Cleaning error! Code: %d", aux_status);
            exit(aux_status);
        }
        exit(status);
    } else if (status == 3) {
        log_error(MASTER_NAME, "Pipes init! Code: %d", status);
        aux_status = clean(1);
        if (aux_status != 0) {
            log_error(MASTER_NAME, "Cleaning error! Code: %d", aux_status);
            exit(aux_status);
        }
        exit(status);
    } else if (status == 4) {
        log_error(MASTER_NAME, "Control mechanisms init! Code: %d", status);
        aux_status = clean(2);
        if (aux_status != 0) {
            log_error(MASTER_NAME, "Cleaning error! Code: %d", aux_status);
            exit(aux_status);
        }
        exit(status);
    }

    /* *** Children init *** */
    status = child_processes_init();
    if (status != 0) {
        log_error(MASTER_NAME, "Components Pre-init error! Code: %d", status);
        aux_status = clean(3);
        if (aux_status != 0) {
            log_error(MASTER_NAME, "Cleaning error! Code: %d", aux_status);
            exit(aux_status);
        }
        exit(status);
    }

    /* *** Children wait *** */
    child_processes_wait();

    /* *** Cleaning *** */
    status = clean(3);
    if (status != 0) {
        log_error(MASTER_NAME, "Cleaning error! Code: %d", status);
        exit(status);
    }

    /* *** Exit *** */
    log_info(MASTER_NAME, "Shutdown.");
    exit(0);
}
