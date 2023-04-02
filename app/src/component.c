#include "component.h"

void* component_main(void* arg)
{
    printf("Thread[%d]: Hello!\n", *(int*)arg);
    return NULL;
}