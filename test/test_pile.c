//
// Created by chronos on 10/26/26.
//

#include "../MAKEPILE/pile.h"
#include <stdio.h>

int main(int argc, char** argv) {

    tPile pile = pileCreate();
    printf("%lld", pileTop(pile, 5));
    return 0;
}
