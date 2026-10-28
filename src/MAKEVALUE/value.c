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
            free(value->data.string);
            value->data.string = NULL;
            break;

        case TYPE_PILE:
            pileFree(&(value->data.pile));
            value->data.pile = NULL;
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
