//
// Created by chronos on 10/28/26.
//

#include "value.h"
#include "../MAKEPILE/pile.h"
#include <stdlib.h>

void freeValue(Value* value) {
    /* Free a value in a cell
     * Only pile and string are malloced !
     */
    if (value == NULL) return;

    switch (value->type) {

        case TYPE_STRING:
            stringRelease(&(value->data.string));
            break;

        case TYPE_PILE:
            pileRelease(&(value->data.pile));
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

    valueInit(value);
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
        stringRetain(destination->data.string);
    }
    else if (destination->type == TYPE_PILE &&
        destination->data.pile != NULL) {
        pileRetain(destination->data.pile);
    }

    return 0;
}

