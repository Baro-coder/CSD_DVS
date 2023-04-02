/* *** Main header *** */
#include "app.h"

/* *** Main driver *** */
int main(int argc, char** argv) {
    int status = 0;
    /* *** Init *** */
    fprintf(stdout, "-- App: Init: ");
    status = app_init(argc, argv);
    if (status != 0) {
        fprintf(stdout, "Failure: %d\n", status);
        exit(status);
    }
    fprintf(stdout, "Done.\n");

    /* *** Children init *** */
    fprintf(stdout, "-- App: Children init:\n\n");
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
