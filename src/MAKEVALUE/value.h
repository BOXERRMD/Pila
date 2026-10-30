//
// Created by chronos on 10/28/26.
//

#ifndef PILA_VALUE_H
#define PILA_VALUE_H

#include <stdbool.h>
#include "./DATA/string.h"

/* A required Value pointer is NULL. */
#define VALUE_NULL_ERROR 1
/* A string-related allocation failed while constructing a Value. */
#define MAKE_VALUE_STRING_MALLOC_ERROR 2

typedef enum {
    TYPE_NULL,
    TYPE_INTEGER,
    TYPE_FLOAT,
    TYPE_BOOLEAN,
    TYPE_STRING,
    TYPE_PILE
} eValueType;

typedef struct sPile* tPile;

typedef struct sValue {
    eValueType type;

    union {
        long long int integer;
        double floating;
        bool boolean;
        tString string;
        tPile pile;
        int null;
    } data;

} Value;

/*
 * Releases the resources owned by a value and resets it to TYPE_NULL.
 *
 * Scalar values do not own external resources. String and pile values
 * release one reference to the referenced object. The Value structure
 * itself is never freed by this function.
 *
 * Passing NULL is allowed and has no effect.
 */
void freeValue(Value* value);

/*
 * Initializes a Value as TYPE_NULL.
 *
 * The destination must either be uninitialized storage or a Value whose
 * resources have already been released. Passing NULL is allowed and has
 * no effect.
 */
void valueInit(Value* value);

/*
 * Copies a Value into destination and retains referenced strings or piles.
 *
 * Scalar fields are copied directly. String and pile objects are shared and
 * retained through their reference counters. Any resource already referenced
 * by destination is released before it is overwritten.
 *
 * Returns 0 on success and VALUE_NULL_ERROR when destination or source is
 * NULL.
 */
int valueCopy(Value* destination, const Value* source);

/* Creates an integer Value containing data. */
Value makeValueInteger(long long int data);

/* Creates a boolean Value containing data. */
Value makeValueBoolean(bool data);

/* Creates a floating-point Value containing data. */
Value makeValueFloat(double data);

/* Creates a TYPE_NULL Value with no owned resource. */
Value makeValueNull(void);

/*
 * Creates a string Value from a NUL-terminated string.
 *
 * The input text is copied into the String representation. On allocation
 * failure, the returned Value has TYPE_STRING with a NULL string pointer.
 */
Value makeValueString(const char* data);

/*
 * Creates a pile Value containing a newly allocated empty pile.
 *
 * On allocation failure, the returned Value contains a NULL pile pointer.
 */
Value makeValuePile(void);

#endif //PILA_VALUE_H
