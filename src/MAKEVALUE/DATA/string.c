//
// Created by chronos on 10/28/26.
//

#include "string.h"
#include <stdlib.h>

static void stringFree(tString* string) {
    /* Free a string and put his pointer to NULL */
    if (string == NULL || *string == NULL) return;

    tString tmp = *string;

    if (tmp->references != 0) return;

    if (tmp->data != NULL) free(tmp->data);
    free(tmp);

    *string = NULL;
}

void stringRelease(tString* string) {
    if (string == NULL || *string == NULL) {
        return;
    }

    tString current = *string;

    if (current->references == 0) {
        // A string MUST be init at 1 reference when it was created.
        // If references = 0, it's a pile created by the system.
        return;
    }

    current->references--;

    stringFree(string); // free only if references = 0
}

void stringRetain(tString string) {
    /* Add 1 to references */
    if (string == NULL) return;

    string->references++;
}
