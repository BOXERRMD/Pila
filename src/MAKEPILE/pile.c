//
// Created by chronos on 10/26/26.
//

#include "pile.h"
#include <stdlib.h>
#include <stddef.h>
#include "../MAKEVALUE/value.h"

typedef struct sCell {
    Value value;
    struct sCell* previous;
} *tCell;

static tCell cellCreate() {
    /*
     * Allocates an empty cell and initializes its Value to TYPE_NULL.
     *
     * Returns NULL when allocation fails. This helper is private to this
     * translation unit.
     */
    tCell cell = malloc(sizeof(struct sCell));
    if (cell == NULL) return NULL;

    Value value;
    valueInit(&value);
    cell->value = value;
    cell->previous = NULL;

    return cell;
}

static void freeCell(tCell* cell) {
    /*
     * Releases the cell's Value, frees the cell, and clears its pointer.
     *
     * This helper is private to this translation unit. Passing NULL or a
     * pointer to NULL is allowed and ignored.
     */
    if (cell == NULL || *cell == NULL) return;

    tCell tmp = *cell;

    freeValue(&(tmp->value));
    free(tmp);
    *cell = NULL;
}

void pileFree(tPile* pile) {
    /*
     * Destroys an unreferenced pile and all of its cells.
     *
     * A pile is destroyed only when its reference count is zero. Passing
     * NULL, a pointer to NULL, or a still-referenced pile is a no-op.
     */
    if (pile == NULL || *pile == NULL) return;

    tPile tmp_pile = *pile;

    if (tmp_pile->references != 0) return;

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

void pileRelease(tPile* pile) {
    /*
     * Drops one pile ownership reference.
     *
     * When the count reaches zero, pileFree performs the actual destruction.
     * Passing NULL or a pointer to NULL is allowed and ignored.
     */
    if (pile == NULL || *pile == NULL) return;

    tPile tmp = *pile;

    if (tmp->references == 0) {
        pileFree(pile);
        return;
    }

    tmp->references--;

    pileFree(pile); // free only if references = 0

}

void pileRetain(tPile pile) {
    /* Adds one ownership reference to a live pile object. */
    if (pile == NULL) return;

    pile->references++;
}

tPile pileCreate() {
    /*
     * Allocates an empty pile with one initial ownership reference.
     *
     * Returns NULL when allocation fails.
     */
    tPile new_pile = malloc(sizeof(struct sPile));
    if (new_pile == NULL) return NULL;

    new_pile->size = 0;
    new_pile->top = NULL;
    new_pile->references = 1;
    return new_pile;
}

int pileAdd(tPile pile, Value value) {
    /*
     * Allocates a cell, copies value into it, and links it at the top.
     *
     * The caller retains ownership of value. Dynamic data is shared through
     * reference counting. Returns PILE_NULL_ERROR for a NULL pile,
     * PILE_CELL_MALLOC_ERROR when the cell cannot be allocated, and
     * VALUE_NULL_ERROR when valueCopy rejects its arguments.
     */
    if (pile == NULL) return PILE_NULL_ERROR;

    tCell new_cell = cellCreate();
    if (new_cell == NULL) return PILE_CELL_MALLOC_ERROR;

    if (valueCopy(&(new_cell->value), &value) != 0) {
        freeCell(&new_cell);
        return VALUE_NULL_ERROR;
    }

    new_cell->previous = pile->top;
    pile->top = new_cell;
    pile->size += 1;

    return PILE_OK;
}

int pilePop(tPile pile, Value* buff) {
    /*
     * Unlinks and releases the top cell.
     *
     * When buff is non-NULL, the removed value is copied into it. When buff
     * is NULL, the removed value is released and discarded. Returns
     * PILE_NULL_ERROR for a NULL pile, PILE_EMPTY_ERROR for an empty pile,
     * and PILE_OK otherwise.
     */
    if (pile == NULL) return PILE_NULL_ERROR;
    if (pile->top == NULL || pile->size == 0) return PILE_EMPTY_ERROR;

    tCell cell = pile->top;
    Value value = cell->value;
    pile->top = (cell->previous != NULL ? cell->previous : NULL);
    pile->size -= 1;

    if (buff != NULL) {
        valueCopy(buff, &value);
        valueInit(&value);
    }

    freeCell(&cell);

    return PILE_OK;
}

int pileTop(tPile pile, Value* buff) {
    /*
     * Copies the top value into buff without changing pile size.
     *
     * Dynamic data is retained through valueCopy. Returns PILE_NULL_ERROR
     * for a NULL pile, PILE_EMPTY_ERROR for an empty pile, and
     * PILE_BUFF_NULL_ERROR for a NULL destination.
     */

    if (pile == NULL) return PILE_NULL_ERROR;
    if (pile->top == NULL || pile->size == 0) return PILE_EMPTY_ERROR;
    if (buff == NULL) return PILE_BUFF_NULL_ERROR;

    valueCopy(buff, &(pile->top->value));
    return PILE_OK;
}

size_t pileSize(tPile pile) {
    /* Returns the number of cells, or zero when pile is NULL. */
    if (pile == NULL) return 0;

    return pile->size;
}

void pileClear(tPile pile) {
    /*
     * Releases every cell and resets top and size.
     *
     * The pile object remains allocated and can be reused. Passing NULL or
     * an already empty pile is allowed and ignored.
     */
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
