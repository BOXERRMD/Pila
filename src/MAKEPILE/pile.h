//
// Created by chronos on 10/26/26.
//

#ifndef PILA_PILE_H
#define PILA_PILE_H

#include <stddef.h>
#include "../MAKEVALUE/value.h"

// SYMBOLIC CONST for errors in pile module
#define PILE_OK 0
#define PILE_MALLOC_ERROR 1
#define PILE_CELL_MALLOC_ERROR 2
#define PILE_EMPTY_ERROR 3
#define PILE_SIZE_ERROR 4 // used if pile->size != number of cells in the pile
#define PILE_NULL_ERROR 5
#define PILE_BUFF_NULL_ERROR 6


typedef struct sPile {
    /* Top cell; NULL means that the pile contains no cells. */
    struct sCell* top;
    /* Number of cells currently linked from top. */
    size_t size;
    /* Number of owners of this pile object. */
    size_t references;
} *tPile;

/* Allocates and initializes an empty pile with one owner. */
tPile pileCreate(void);

/*
 * Destroys pile only when its reference count is zero.
 *
 * If pile is NULL, *pile is NULL, or the pile still has owners, this
 * function does nothing. It releases every cell when destruction occurs.
 */
void pileFree(tPile* pile);

/*
 * Releases one ownership reference to a pile.
 *
 * The pile is destroyed when the reference count reaches zero. Passing NULL
 * is allowed and ignored.
 */
void pileRelease(tPile* pile);

/* Adds one ownership reference to pile. Passing NULL is ignored. */
void pileRetain(tPile pile);

/*
 * Copies value onto the top of pile.
 *
 * Dynamic values are copied through valueCopy and therefore retain their
 * referenced string or nested pile. The caller retains ownership of value.
 * Returns PILE_OK, PILE_NULL_ERROR, PILE_CELL_MALLOC_ERROR, or
 * VALUE_NULL_ERROR.
 */
int pileAdd(tPile pile, Value value);

/*
 * Removes the top cell from pile.
 *
 * If buff is not NULL, the removed value is copied into buff; otherwise the
 * value is discarded. The pile owns no value after the cell is removed.
 * Returns PILE_OK, PILE_NULL_ERROR, or PILE_EMPTY_ERROR.
 */
int pilePop(tPile pile, Value* buff);

/*
 * Copies the top value into buff without removing it.
 *
 * Dynamic values are retained through valueCopy. Returns PILE_OK,
 * PILE_NULL_ERROR, PILE_EMPTY_ERROR, or PILE_BUFF_NULL_ERROR.
 */
int pileTop(tPile pile, Value* buff);

/* Returns the stored cell count, or zero when pile is NULL. */
size_t pileSize(tPile pile);

/* Releases every cell while preserving the pile object itself. */
void pileClear(tPile pile);

#endif //PILA_PILE_H
