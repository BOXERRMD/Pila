//
// Created by chronos on 10/28/26.
//

#ifndef PILA_VALUE_H
#define PILA_VALUE_H

#include <stdbool.h>

#include "../MAKEPILE/pile.h"

typedef enum {
    TYPE_NULL,
    TYPE_INTEGER,
    TYPE_FLOAT,
    TYPE_BOOLEAN,
    TYPE_STRING,
    TYPE_PILE
} eValueType;

typedef struct sValue {
    eValueType type;

    union {
        long long int integer;
        double floating;
        bool boolean;
        char* string;
        tPile pile;
    } data;

} Value;

void freeValue(Value* value);
void valueInit(Value* value);

#endif //PILA_VALUE_H
