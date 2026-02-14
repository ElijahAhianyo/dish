#include "memory.h"


void *reallocate(void *pointer, size_t old_size, size_t new_size){
    // free when new size is 0
    if (new_size == 0){
        free(pointer);
        return NULL;
    }

    void *res = realloc(pointer, new_size);
    if (res == NULL){
        perror("realloc failed");
        exit(1);
    }
    return res;
}