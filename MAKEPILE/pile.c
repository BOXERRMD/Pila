//
// Created by chronos on 10/26/26.
//

#include "pile.h"
#include "../const.h"
#include <stdlib.h>
#include <stddef.h>

struct sPile {
    // "top" look the top of the pile (can be NULL if nothing is in the pile)
    struct sCell* top;
    // "size" is the size of the pile (init at 0)
    size_t size;
};

typedef struct sCell {
    pile_value_t value;
    struct sCell* previous;
} *tCell;

static void freeCell(tCell* cell) {
    /* Free a cell and set the pointer to NULL.
     * This function is PRIVATE and must be not called outside this file ! */
    free(*cell);
    *cell = NULL;
}

void pileFree(tPile* pile) {
    /* Free a pile and cell inside. Put the pointer of the pile and cells to NULL */
    if (pile == NULL || *pile == NULL) return;

    tPile tmp_pile = *pile;
    if (tmp_pile->top != NULL) {
        tCell cell = tmp_pile->top;
        while (cell != NULL) {
            const tCell tmp_cell = cell->previous;
            freeCell(&cell);
            cell = tmp_cell;
        }
    }
    free(tmp_pile);
    *pile = NULL;
}


tPile pileCreate() {
    /* Create a new pile
     * Return NULL if an error occurred */
    tPile new_pile = malloc(sizeof(struct sPile));
    if (new_pile == NULL) return NULL;

    new_pile->size = 0;
    new_pile->top = NULL;
    return new_pile;
}

int pileAdd(tPile pile, pile_value_t value) {
    /* Add a value on the top of the pile
     * Return 1 if the pile is NULL
     * Return 2 if a malloc error occurred.
     */
    if (pile == NULL) return 1;

    tCell new_cell = malloc(sizeof(struct sCell));
    if (new_cell == NULL) return 2;

    new_cell->value = value;

    tCell tmp_cell = pile->top;
    if (tmp_cell != NULL) {
        new_cell->previous = tmp_cell;
        pile->top = new_cell;
        pile->size += 1;
    }
    else {
        new_cell->previous = NULL;
        pile->top = new_cell;
        pile->size += 1;
    }

    return 0;
}

int pilePop(tPile pile, pile_value_t* buff) {
    /* Pop the top of the pile.
     * return 1 if the pile is empty */
    if (pile != NULL && pile->top != NULL && pile->size != 0) {
        tCell cell = pile->top;
        pile_value_t value = cell->value;
        pile->top = (cell->previous != NULL ? cell->previous : NULL);
        pile->size -= 1;
        freeCell(&cell);

        if (buff != NULL)
            *buff = value;
        return 0;
    }

    return 1;
}

int pileTop(tPile pile, pile_value_t* buff) {
    /* Return the top value.
     * Return 1 if the pile is empty */

    if (pile != NULL && pile->size != 0 && buff != NULL) {
        *buff = pile->top->value;
        return 0;
    }
    return 1;
}

size_t pileSize(tPile pile) {
    /* Return the pile size
     * If pile is NULL, the size is equal to 0 */
    if (pile == NULL) return 0;

    return pile->size;
}

void pileClear(tPile pile) {
    /* Clear all cell on the pile. */
    if (pile == NULL || pile->size == 0) return;

    tCell cell = pile->top;
    while (cell != NULL) {
        tCell tmp_cell = cell->previous;
        freeCell(&cell);
        cell = tmp_cell;
    }
    pile->top = NULL;
    pile->size = 0;
}
