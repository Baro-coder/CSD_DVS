/* *** Main header *** */
#include "app.h"

/* *** Main driver *** */
int main(int argc, char** argv) {
    // int a = 3;
    // double n = ((double)a - 1) / 3;
    // int np = (int)n;
    // printf("a  = %d\n", a);
    // printf("n  = %f\n", n);
    // printf("np = %d\n", np);

    int status = 0;
    /* *** Init *** */
    log_info("App", "Init...");
    fprintf(stdout, "-- App: Init: ");
    status = app_init(argc, argv);
    if (status != 0) {
        fprintf(stdout, "Failure: %d\n", status);
        exit(status);
    }
    fprintf(stdout, "Done.\n");

    /* *** Children init *** */
    fprintf(stdout, "-- App: Children init:\n");
    status = child_processes_init();
    if (status != 0) {
        fprintf(stdout, "Failure: %d\n", status);
        exit(status);
    }

    /* *** Children wait *** */
    child_processes_wait();

    /* *** Cleaning *** */
    fprintf(stdout, "\n-- App: Cleaning: ");
    status = clean();
    if (status != 0) {
        fprintf(stdout, "Failure: %d\n", status);
        exit(status);
    }
    fprintf(stdout, "Done.\n");

    /* *** Exit *** */
    fprintf(stdout, "-- App: Exit.\n");
    exit(0);
}
