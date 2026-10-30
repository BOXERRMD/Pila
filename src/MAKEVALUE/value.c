//
// Created by chronos on 10/28/26.
//

#include "value.h"
#include "../MAKEPILE/pile.h"
#include <stdlib.h>
#include <string.h>

void freeValue(Value* value) {
    /*
     * Releases resources referenced by value and resets value to TYPE_NULL.
     *
     * The Value object is normally embedded in another object, such as a
     * pile cell, so this function never frees the Value pointer itself.
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
     * Initializes value to the neutral TYPE_NULL state and clears its
     * dynamic-data pointer.
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
     * reference count. The previous contents of destination are released
     * before being overwritten.
     *
     * Returns 0 on success, or VALUE_NULL_ERROR if either pointer is NULL.
     */
    if (destination == NULL || source == NULL) {
        return VALUE_NULL_ERROR;
    }

    freeValue(destination);

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

Value makeValueInteger(long long int data) {
    /* Builds a scalar integer Value without dynamic allocation. */
    Value value;
    value.type = TYPE_INTEGER;
    value.data.integer = data;
    return value;
}

Value makeValueBoolean(bool data) {
    /* Builds a scalar boolean Value without dynamic allocation. */
    Value value;
    value.type = TYPE_BOOLEAN;
    value.data.boolean = data;
    return value;
}

Value makeValueFloat(double data) {
    /* Builds a scalar floating-point Value without dynamic allocation. */
    Value value;
    value.type = TYPE_FLOAT;
    value.data.floating = data;
    return value;
}

Value makeValueNull(void) {
    /* Builds a Value representing the language-level null value. */
    Value value;
    value.type = TYPE_NULL;
    value.data.string = NULL;
    return value;
}

Value makeValueString(const char* data) {
    /*
     * Builds a string Value by copying data into a reference-counted String.
     *
     * The input must be a non-NULL NUL-terminated C string. On allocation
     * failure, the returned Value has TYPE_STRING and a NULL data pointer.
     */
    Value value;
    value.type = TYPE_STRING;

    const char* current_string = data != NULL ? data : ""; // avoid data = NULL

    const size_t data_size = strlen(current_string)+1;
    char* string = malloc(sizeof(char)*(data_size));

    if (string != NULL) {
        string[0] = '\0';
        strncpy(string, current_string, data_size);
        const tString string_structure = stringCreate(string);
        if (string_structure != NULL)
            value.data.string = string_structure;
        else value.data.string = NULL;
    }
    else {
        value.data.string = NULL;
    }
    return value;
}

Value makeValuePile(void) {
    /* Builds a pile Value owning the initial reference returned by pileCreate. */
    Value value;
    value.type = TYPE_PILE;

    tPile pile = pileCreate();
    value.data.pile = pile;     // assign the pile even if pile = NULL
    return value;
}