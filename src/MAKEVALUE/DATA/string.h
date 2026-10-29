//
// Created by chronos on 10/28/26.
//

#ifndef PILA_STRING_H
#define PILA_STRING_H
#include <stddef.h>

typedef struct sString *tString;
typedef struct sChar *tChar;

/*
 * Return the number of reference for this string.
 * Return 0 if the string doesn't exist
 */
size_t getStringReferences(tString string);

/*
 * Return the first character of the string.
 * Return NULL if the string doesn't exist or if the string is empty.
 */
struct sChar* getStringData(tString string);

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


tString stringCreate(char* data);

#endif //PILA_STRING_H
