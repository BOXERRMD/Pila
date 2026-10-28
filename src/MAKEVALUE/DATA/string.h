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

#endif //PILA_STRING_H
