//
// Created by chronos on 10/28/26.
//

#ifndef PILA_STRING_H
#define PILA_STRING_H
#include <stddef.h>

typedef struct sString {

    size_t references;
    char* data;

} *tString;

/*
 * Releases one ownership reference to a string.
 *
 * When the reference count reaches zero, both the character buffer and the
 * String object are destroyed. On return, the caller's pointer is set to
 * NULL when destruction occurred. Passing NULL is allowed and ignored.
 */
void stringRelease(tString* string);

/*
 * Adds one ownership reference to string.
 *
 * Passing NULL is allowed and ignored. The caller must ensure that string
 * points to a live reference-counted String object.
 */
void stringRetain(tString string);

#endif //PILA_STRING_H
