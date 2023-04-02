/* *** Main header *** */
#include "app.h"

/* *** Main driver *** */
int main(int argc, char** argv) {
    /* *** App INIT *** */
    fprintf(stdout, "-- App: Init: ");
    if (app_init(argc, argv)) {
        exit(1);
    }
    fprintf(stdout, "Done.\n");

    /* *** Threads init *** */
    fprintf(stdout, "-- App: Children init...\n");
    child_processes_init();

    /* *** Threads join *** */
    child_processes_wait();

    /* *** App EXIT *** */
    fprintf(stdout, "-- App: Exit\n");
    exit(0);
}
