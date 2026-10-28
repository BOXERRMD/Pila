//
// Created by chronos on 10/28/26.
//

#include "value.h"
#include "../MAKEPILE/pile.h"
#include <stdlib.h>

static void valueRetain(const Value* value) {
    /* Add 1 in value reference */
    if (value == NULL) return;

    if (value->type == TYPE_STRING && value->data.string != NULL) {
        value->data.string->references++;
    }
    else if (value->type == TYPE_PILE && value->data.pile != NULL) {
        value->data.pile->references++;
    }
}

static void valueRelease(Value* value) {
    /* Remove 1 in value reference */
    if (value == NULL) return;

    if (value->type != TYPE_STRING && value->type != TYPE_PILE) return;

    if (value->type == TYPE_STRING && value->data.string != NULL) {
        value->data.string->references--;
        if (value->data.string->references == 0) {
            freeValue(value);
        }
    }
    else if (value->type == TYPE_PILE && value->data.pile != NULL) {
        value->data.pile->references--;
        pileFree(&(value->data.pile));
    }
}

void freeValue(Value* value) {
    /* Free a value in a cell
     * Only pile and string are malloced !
     */
    if (value == NULL) return;

    switch (value->type) {

        case TYPE_STRING:
            valueRelease(value);
            break;

        case TYPE_PILE:
            valueRelease(value);
            break;

        case TYPE_INTEGER:
            break;
        case TYPE_FLOAT:
            break;
        case TYPE_BOOLEAN:
            break;
        case TYPE_NULL:
            break;
    }


    value->type = TYPE_NULL;
}

void valueInit(Value* value) {
    /* Init a value */
    if (value == NULL) return;

    value->type = TYPE_NULL;
    value->data.string = NULL;
}

int valueCopy(Value* destination, const Value* source) {
    /* Copy a value and add 1 to references if is a string or a pile */
    if (destination == NULL || source == NULL) {
        return 1;
    }

    *destination = *source;

    if (destination->type == TYPE_STRING &&
        destination->data.string != NULL) {
        valueRetain(destination);
    }
    else if (destination->type == TYPE_PILE &&
        destination->data.pile != NULL) {
        valueRetain(destination);
    }

    return 0;
}

