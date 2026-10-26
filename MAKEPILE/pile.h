//
// Created by chronos on 10/26/26.
//

#ifndef PILA_PILE_H
#define PILA_PILE_H

#include "../const.h"

typedef struct sPile* tPile;

tPile pileCreate();
int pileAdd(tPile pile, PILE_VALUE_SIZE value);
PILE_VALUE_SIZE pilePop(tPile pile, PILE_VALUE_SIZE def);
PILE_VALUE_SIZE pileTop(tPile pile, PILE_VALUE_SIZE def);
void pileClear(tPile pile);

#endif //PILA_PILE_H
