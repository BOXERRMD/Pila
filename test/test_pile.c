//
// Created by chronos on 10/26/26.
//

#include "../MAKEPILE/pile.h"
#include "../const.h"
#include <stdio.h>
#include <stdlib.h>

#define VALUE1 44
#define VALUE2 78665467

int main(int argc, char** argv) {

    tPile pile = pileCreate();
    if (pile == NULL) {
        fprintf(stderr, "ERROR : pile creation failed !\n");
        exit(1);
    }
    pile_value_t value;
    if (pileTop(pile, &value) == 0) {
        fprintf(stderr, "ERROR : Top on a empty pile must return a code != 0\n");
        exit(2);
    }
    int return_code = pileAdd(pile, VALUE1);
    if (return_code != 0) {
        fprintf(stderr, "ERROR : adding the number %d in the pile. Code : %d\n", VALUE1, return_code);
        exit(3);
    }
    pile_value_t top_value;
    if (pileTop(pile, &top_value) != 0 || top_value != VALUE1) {
        fprintf(stderr, "ERROR : the top pile value is not equal to 44. Value : %lld\n", top_value);
        exit(4);
    }

    return_code = pileAdd(pile, VALUE2);
    if (return_code != 0) {
        fprintf(stderr, "ERROR : adding the number %d in the pile. Code : %d\n", VALUE2, return_code);
        exit(5);
    }

    pile_value_t pop_value;
    if (pilePop(pile, &pop_value) != 0) {
        fprintf(stderr, "ERROR : pop a value from a \"empty list\" after add a value on the pile.\n");
        exit(6);
    }

    printf("SUCCESS : pile test passed successfully :)\n");

    return 0;
}
