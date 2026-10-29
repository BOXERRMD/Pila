//
// Created by chronos on 10/28/26.
//

#ifndef PILA_VALUE_H
#define PILA_VALUE_H

#include <stdbool.h>
#include "./DATA/string.h"

/* Returned when a Value pointer required by an operation is NULL. */
#define VALUE_NULL_ERROR 1

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
 * This is a shallow copy of scalar data and a reference-counted copy of
 * dynamic data. The destination is overwritten as-is; its previous
 * resources are not released by this function. The caller must therefore
 * provide an initialized, empty destination or release it beforehand.
 *
 * Returns 0 on success and VALUE_NULL_ERROR when destination or source is
 * NULL.
 */
int valueCopy(Value* destination, const Value* source);

#endif //PILA_VALUE_H
