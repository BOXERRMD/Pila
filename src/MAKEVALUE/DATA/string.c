//
// Created by chronos on 10/28/26.
//

#include "string.h"
#include <stdlib.h>

struct sChar {
    char character;
    struct sChar* next;
};

struct sString {

    size_t references;
    tChar data;

};

size_t getStringReferences(tString string) {
    /* Return the number of reference for this string.
     * Return 0 if the string doesn't exist
     */
    if (string == NULL) return 0;

    return string->references;
}

struct sChar* getStringData(tString string) {
    /*
     * Return the first character of the string.
     * Return NULL if the string doesn't exist or if the string is empty.
     */
    if (string == NULL) return NULL;

    return string->data;
}

tString stringCreate(char* data) {
    /*
     *  Create a new string and gave it the current data
     *
     */
}

static void stringFree(tString* string) {
    /*
     * Destroys a string whose reference count is already zero.
     *
     * This helper is private to this translation unit. It does nothing for
     * NULL pointers or for strings that still have outstanding references.
     */
    if (string == NULL || *string == NULL) return;

    tString tmp = *string;

    if (tmp->references != 0) return;

    if (tmp->data != NULL) free(tmp->data);
    free(tmp);

    *string = NULL;
}

void stringRelease(tString* string) {
    /*
     * Drops one reference and destroys the string when no owner remains.
     *
     * A zero reference count is treated as an invalid release state and is
     * left unchanged. Passing NULL is allowed and ignored.
     */
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
    /* Adds one ownership reference to a live string object. */
    if (string == NULL) return;

    string->references++;
}
