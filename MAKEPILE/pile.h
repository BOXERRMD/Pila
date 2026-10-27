//
// Created by chronos on 10/26/26.
//

#ifndef PILA_PILE_H
#define PILA_PILE_H

#include <stddef.h>
#include "../const.h"

// SYMBOLIC CONST for errors in pile module
#define PILE_OK 0
#define PILE_MALLOC_ERROR 1
#define PILE_CELL_MALLOC_ERROR 2
#define PILE_EMPTY_ERROR 3
#define PILE_SIZE_ERROR 4 // used if pile->size != number of cells in the pile
#define PILE_NULL_ERROR 5
#define PILE_BUFF_NULL_ERROR 6


typedef struct sPile* tPile;

tPile pileCreate();
void pileFree(tPile* pile);
int pileAdd(tPile pile, pile_value_t value);
int pilePop(tPile pile, pile_value_t* buff);
int pileTop(tPile pile, pile_value_t* buff);
size_t pileSize(tPile pile);
void pileClear(tPile pile);

#endif //PILA_PILE_H
