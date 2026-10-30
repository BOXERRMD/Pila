//
// Created by chronos on 10/28/26.
//

#include "string.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct sString {

    size_t references;
    char* data;
    size_t size;

};

size_t getStringReferences(tString string) {
    /*
     * Returns the current ownership-reference count.
     *
     * A NULL String pointer represents no object and returns zero.
     */
    if (string == NULL) return 0;

    return string->references;
}

char* getStringData(tString string) {
    /*
     * Returns the first borrowed character node in the string list.
     *
     * A NULL String pointer or an empty string returns NULL. The returned
     * node remains owned by the String object.
     */
    if (string == NULL) return NULL;

    return string->data;
}

static void stringFree(tString* string) {
    /*
     * Destroys a string whose reference count is already zero.
     *
     * This helper is private to this translation unit. It frees the complete
     * character list and the String object, and does nothing for NULL
     * pointers or objects that still have outstanding references.
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
     * Drops one ownership reference and destroys the String at zero.
     *
     * A zero reference count is treated as an invalid release state and is
     * left unchanged. Passing NULL is allowed and ignored.
     */
    if (string == NULL || *string == NULL) {
        return;
    }

    tString current = *string;

    if (current->references == 0) {
        /* A zero count indicates an invalid or already released object. */
        return;
    }

    current->references--;

    stringFree(string); // free only if references = 0
}

void stringRetain(tString string) {
    /* Adds one ownership reference to a live String object. */
    if (string == NULL) return;

    string->references++;
}

tString stringCreate(const char* data) {
    /*
     * Creates a reference-counted String from a NUL-terminated C string.
     *
     * Characters are inserted into a singly linked list in source order.
     * Returns NULL when data is NULL or when any allocation fails. The
     * returned object starts with one ownership reference.
     */
    if (data == NULL) return NULL;

    tString new_string = malloc(sizeof(struct sString));
    if (new_string == NULL) return NULL;

    new_string->references = 1;
    new_string->size = 0;
    new_string->data = NULL;

    size_t len_data = strlen(data);

    char* malloced_string = malloc(sizeof(char)* len_data+1);
    if (malloced_string == NULL) {
        stringFree(&new_string);
        return NULL;
    }

    strncpy(malloced_string, data, len_data+1);
    new_string->data = malloced_string;

    return new_string;
}
