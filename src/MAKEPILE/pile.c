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
    /* Create a new cell and init it */
    tCell cell = malloc(sizeof(struct sCell));
    if (cell == NULL) return NULL;

    Value value;
    valueInit(&value);
    cell->value = value;
    cell->previous = NULL;

    return cell;
}

static void freeCell(tCell* cell) {
    /* Free a cell and set the pointer to NULL.
     * This function is PRIVATE and must be not called outside this file ! */
    if (cell == NULL || *cell == NULL) return;

    tCell tmp = *cell;

    freeValue(&(tmp->value));
    free(tmp);
    *cell = NULL;
}

void pileFree(tPile* pile) {
    /* Free a pile and cell inside. Put the pointer of the pile and cells to NULL */
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
    /* Release a pile reference.
     * If references = 0, free the pile.
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
    /* Add 1 to references */
    if (pile == NULL) return;

    pile->references++;
}

tPile pileCreate() {
    /* Create a new pile
     * Return NULL if an error occurred */
    tPile new_pile = malloc(sizeof(struct sPile));
    if (new_pile == NULL) return NULL;

    new_pile->size = 0;
    new_pile->top = NULL;
    new_pile->references = 1;
    return new_pile;
}

int pileAdd(tPile pile, Value value) {
    /* Add a value on the top of the pile
     * Return PILE_NULL_ERROR if the pile is NULL
     * Return PILE_CELL_MALLOC_ERROR if a malloc error occurred on the new cell.
     * Return VALUE_NULL_ERROR if a pointed value is NULL
     */
    if (pile == NULL) return PILE_NULL_ERROR;

    tCell new_cell = cellCreate();
    if (new_cell == NULL) return PILE_CELL_MALLOC_ERROR;

    if (valueCopy(&(new_cell->value), &value) != 0) {
        return VALUE_NULL_ERROR;
    }

    new_cell->previous = pile->top;
    pile->top = new_cell;
    pile->size += 1;

    return PILE_OK;
}

int pilePop(tPile pile, Value* buff) {
    /* Pop the top of the pile.
     * "buff" can be NULL. If it is, it remove the top value if the pile.
     * return PILE_EMPTY_ERROR if the pile is empty
     * return PILE_NULL_ERROR if the pile is NULL
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
    /* Return the top value.
     * return PILE_EMPTY_ERROR if the pile is empty
     * return PILE_NULL_ERROR if the pile is NULL
     * return PILE_BUFF_NULL_ERROR if the buffer is NULL
     */

    if (pile == NULL) return PILE_NULL_ERROR;
    if (pile->top == NULL || pile->size == 0) return PILE_EMPTY_ERROR;
    if (buff == NULL) return PILE_BUFF_NULL_ERROR;

    valueCopy(buff, &(pile->top->value));
    return PILE_OK;
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
