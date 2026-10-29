//
// Created by chronos on 10/26/26.
//

#ifndef PILA_PILE_H
#define PILA_PILE_H

#include <stddef.h>
#include "../MAKEVALUE/value.h"

// SYMBOLIC CONST for errors in pile module
#define PILE_OK 0
#define PILE_MALLOC_ERROR 1
#define PILE_CELL_MALLOC_ERROR 2
#define PILE_EMPTY_ERROR 3
#define PILE_SIZE_ERROR 4 // used if pile->size != number of cells in the pile
#define PILE_NULL_ERROR 5
#define PILE_BUFF_NULL_ERROR 6


typedef struct sPile {
    // "top" look the top of the pile (can be NULL if nothing is in the pile)
    struct sCell* top;
    // "size" is the size of the pile (init at 0)
    size_t size;
    size_t references;
} *tPile;

tPile pileCreate(void);
void pileFree(tPile* pile);
void pileRelease(tPile* pile);
void pileRetain(tPile pile);
int pileAdd(tPile pile, Value value);
int pilePop(tPile pile, Value* buff);
int pileTop(tPile pile, Value* buff);
size_t pileSize(tPile pile);
void pileClear(tPile pile);

#endif //PILA_PILE_H
