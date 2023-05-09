/* *** Main header *** */
#include "app.h"

/* *** Main driver *** */
int main(int argc, char** argv) {
    int status = -1;
    int aux_status = -1;
    
    /* *** Init *** */
    status = app_init(argc, argv);
    if (status == 1) {
        logs_log_error(MASTER_NAME, "Logging thread-safety init! Code: %d", status);
        exit(status);
    } else if (status == 2) {
        logs_log_error(MASTER_NAME, "Invalid arguments error! Code: %d", status);
        aux_status = clean(1);
        if (aux_status != 0) {
            logs_log_error(MASTER_NAME, "Cleaning error! Code: %d", aux_status);
            exit(aux_status);
        }
        exit(status);
    } else if (status == 3) {
        logs_log_error(MASTER_NAME, "Pipes init! Code: %d", status);
        aux_status = clean(1);
        if (aux_status != 0) {
            logs_log_error(MASTER_NAME, "Cleaning error! Code: %d", aux_status);
            exit(aux_status);
        }
        exit(status);
    } else if (status == 4) {
        logs_log_error(MASTER_NAME, "Control mechanisms init! Code: %d", status);
        aux_status = clean(2);
        if (aux_status != 0) {
            logs_log_error(MASTER_NAME, "Cleaning error! Code: %d", aux_status);
            exit(aux_status);
        }
        exit(status);
    }

    /* *** Children init *** */
    status = child_processes_init();
    if (status != 0) {
        logs_log_error(MASTER_NAME, "Components Pre-init error! Code: %d", status);
        aux_status = clean(3);
        if (aux_status != 0) {
            logs_log_error(MASTER_NAME, "Cleaning error! Code: %d", aux_status);
            exit(aux_status);
        }
        exit(status);
    }

    /* *** Children wait *** */
    child_processes_wait();

    /* *** Cleaning *** */
    status = clean(3);
    if (status != 0) {
        logs_log_error(MASTER_NAME, "Cleaning error! Code: %d", status);
        exit(status);
    }

    /* *** Exit *** */
    logs_log_info(MASTER_NAME, "Shutdown.");
    exit(0);
}
