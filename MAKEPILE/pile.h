//
// Created by chronos on 10/26/26.
//

#ifndef PILA_PILE_H
#define PILA_PILE_H

#include <stddef.h>
#include "../const.h"

typedef struct sPile* tPile;

tPile pileCreate();
void pileFree(tPile* pile);
int pileAdd(tPile pile, pile_value_t value);
int pilePop(tPile pile, pile_value_t* buff);
int pileTop(tPile pile, pile_value_t* buff);
size_t pileSize(tPile pile);
void pileClear(tPile pile);

#endif //PILA_PILE_H
