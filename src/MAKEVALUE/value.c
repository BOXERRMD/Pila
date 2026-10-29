//
// Created by chronos on 10/28/26.
//

#include "value.h"
#include "../MAKEPILE/pile.h"
#include <stdlib.h>

void freeValue(Value* value) {
    /*
     * Releases resources referenced by value and resets value to TYPE_NULL.
     *
     * The Value object is normally embedded in another object, such as a
     * pile cell, so this function must never free the Value pointer itself.
     * Only TYPE_STRING and TYPE_PILE can own reference-counted resources.
     *
     * A NULL pointer is accepted and ignored.
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
    /*
     * Initializes value to the neutral TYPE_NULL state.
     *
     * This function does not release an existing string or pile reference;
     * callers must release an already-owned value before reinitializing it.
     * A NULL pointer is accepted and ignored.
     */
    if (value == NULL) return;

    value->type = TYPE_NULL;
    value->data.string = NULL;
}

int valueCopy(Value* destination, const Value* source) {
    /*
     * Copies source into destination and retains dynamic resources.
     *
     * Integer, floating-point, boolean, and null values are copied directly.
     * String and pile values share their referenced object and increment its
     * reference count. The previous contents of destination are overwritten
     * without being released, so destination must be empty or already
     * released by the caller.
     *
     * Returns 0 on success, or VALUE_NULL_ERROR if either pointer is NULL.
     */
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
