//
// Created by chronos on 10/26/26.
//

#include "../MAKEPILE/pile.h"
#include "../const.h"
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT 5
#define VALUE1 44
#define VALUE2 78665467

int main(int argc, char** argv) {

    tPile pile = pileCreate();
    if (pile == NULL) {
        fprintf(stderr, "ERROR : pile creation failed !\n");
        exit(1);
    }
    PILE_VALUE_SIZE default_value = pileTop(pile, DEFAULT);
    if (default_value != DEFAULT) {
        fprintf(stderr, "ERROR : default value not equal to %d. Value : %lld\n", DEFAULT, default_value);
        exit(2);
    }
    int return_code = pileAdd(pile, VALUE1);
    if (return_code != 0) {
        fprintf(stderr, "ERROR : adding the number %d in the pile. Code : %d\n", VALUE1, return_code);
        exit(3);
    }
    PILE_VALUE_SIZE top_value = pileTop(pile, 5);
    if (top_value != VALUE1) {
        fprintf(stderr, "ERROR : the top pile value is not equal to 44. Value : %lld\n", top_value);
        exit(4);
    }

    return_code = pileAdd(pile, VALUE2);
    if (return_code != 0) {
        fprintf(stderr, "ERROR : adding the number %d in the pile. Code : %d\n", VALUE2, return_code);
        exit(5);
    }

    PILE_VALUE_SIZE pop_value = pilePop(pile, DEFAULT);
    if (pop_value == DEFAULT) {
        fprintf(stderr, "ERROR : pop the top value return %d instead of %d\n", DEFAULT, VALUE2);
        exit(6);
    }

    pilePop(pile, DEFAULT);
    default_value = 0;
    default_value = pilePop(pile, DEFAULT);

    if (default_value != DEFAULT) {
        fprintf(stderr, "ERROR : pop to an empty pile. Waiting value : %d   Got Value : %lld\n", DEFAULT, default_value);
        exit(7);
    }

    pileAdd(pile, VALUE1);
    pileClear(pile);

    if (pileSize(pile) != 0 || pileTop(pile, DEFAULT) != DEFAULT) {
        fprintf(stderr, "ERROR : clear function doesn't clear the pile properly.\n");
        exit(8);
    }

    pileAdd(pile, VALUE1);
    pileAdd(pile, VALUE2);

    freePile(&pile);
    if (pile != NULL) {
        fprintf(stderr, "ERROR : free pile function doesn't free the pile or not set the pointer to NULL\n");
        exit(9);
    }

    printf("SUCCESS : pile test passed successfully :)\n");

    return 0;
}
