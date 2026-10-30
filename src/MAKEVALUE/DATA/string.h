//
// Created by chronos on 10/28/26.
//

#ifndef PILA_STRING_H
#define PILA_STRING_H
#include <stddef.h>

typedef struct sString *tString;
typedef struct sChar *tChar;

/*
 * Returns the current number of ownership references for string.
 *
 * Returns zero when string is NULL. The returned count is informational and
 * must not be modified directly by callers.
 */
size_t getStringReferences(tString string);

/*
 * Returns the first node in string's internal character list.
 *
 * Returns NULL when string is NULL or empty. The returned node is borrowed;
 * callers must not free it or modify the list through it.
 */
struct sChar* getStringData(tString string);

/*
 * Releases one ownership reference to a string.
 *
 * When the reference count reaches zero, all character nodes and the String
 * object are destroyed. On return, the caller's pointer is set to NULL when
 * destruction occurs. Passing NULL is allowed and ignored.
 */
void stringRelease(tString* string);

/*
 * Adds one ownership reference to string.
 *
 * Passing NULL is allowed and ignored. The caller must ensure that string
 * points to a live reference-counted String object.
 */
void stringRetain(tString string);

/*
 * Creates a reference-counted String from a NUL-terminated C string.
 *
 * Characters are stored in a private singly linked list in source order.
 * The returned object starts with one ownership reference. Returns NULL when
 * data is NULL or when an allocation fails.
 */
tString stringCreate(const char* data);

#endif //PILA_STRING_H
