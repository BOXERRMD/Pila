//
// Created by chronos on 10/26/26.
//

#include "pile.h"
#include "../const.h"
#include <stdlib.h>

struct sPile {
    // "top" look the top of the pile (can be NULL if nothing is in the pile)
    struct sCell* top;
    // "size" is the size of the pile (init at 0)
    PILE_VALUE_SIZE size;
};

typedef struct sCell {
    PILE_VALUE_SIZE value;
    struct sCell* next;
    struct sCell* previous;
} *tCell;

void freeCell(tCell* cell) {
    free(*cell);
    *cell = NULL;
}

void freePile(tPile* pile) {
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
    /* Create a new pile */
    tPile new_pile = malloc(sizeof(struct sPile));
    if (new_pile == NULL) return NULL;

    new_pile->size = 0;
    new_pile->top = NULL;
    return new_pile;
}

int pileAdd(tPile pile, PILE_VALUE_SIZE value) {
    /* Add a value on the top of the pile */
    tCell new_cell = malloc(sizeof(struct sCell));
    if (new_cell == NULL) return 1;

    new_cell->value = value;
    new_cell->next = NULL; // nothing after her because it's the last on the pile

    tCell tmp_cell;
    if ((tmp_cell = pile->top) != NULL) {
        tmp_cell->next = new_cell;
        new_cell->previous = tmp_cell;
        pile->top = new_cell;
        pile->size += 1;
    }
    else {
        new_cell->previous = NULL;
        pile->top = new_cell;
        pile->top += 1;
    }

    return 0;
}

PILE_VALUE_SIZE pilePop(tPile pile, PILE_VALUE_SIZE def) {
    /* Pop the top of the pile.
     * "def" is returned if an error occurred or the pile are empty */
    if (pile->size != 0) {
        tCell cell = pile->top;
        PILE_VALUE_SIZE value = cell->value;
        pile->top = (cell->previous != NULL ? cell->previous : NULL);
        pile->size -= 1;
        freeCell(&cell);
        return value;
    }

    return def;
}

PILE_VALUE_SIZE pileTop(tPile pile, PILE_VALUE_SIZE def) {
    /* Return the top value.
     * "def" is returned if an error occurred or the pile are empty */

    if (pile->size != 0) {
        return pile->top->value;
    }
    return def;
}

void pileClear(tPile pile) {
    /* Clear all cell on the pile. */
    if (pile->size == 0) return;

    tCell cell = pile->top;
    while (cell != NULL) {
        tCell tmp_cell = cell->previous;
        freeCell(&cell);
        cell = tmp_cell;
    }
}
