//
// Created by chronos on 10/28/26.
//

#ifndef PILA_VALUE_H
#define PILA_VALUE_H

#include <stdbool.h>

#include "./DATA/string.h"

#define VALUE_NULL_ERROR 1

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
        tString string;
        tPile pile;
    } data;

} Value;

void freeValue(Value* value);
void valueInit(Value* value);
int valueCopy(Value* destination, const Value* source);

#endif //PILA_VALUE_H
