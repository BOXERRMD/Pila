//
// Created by chronos on 10/28/26.
//

#include "string.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct sChar {
    char character;
    struct sChar* next;
};

struct sString {

    size_t references;
    tChar data;
    size_t size;

};

size_t getStringReferences(tString string) {
    /*
     * Returns the current ownership-reference count.
     *
     * A NULL String pointer represents no object and returns zero.
     */
    if (string == NULL) return 0;

    return string->references;
}

struct sChar* getStringData(tString string) {
    /*
     * Returns the first borrowed character node in the string list.
     *
     * A NULL String pointer or an empty string returns NULL. The returned
     * node remains owned by the String object.
     */
    if (string == NULL) return NULL;

    return string->data;
}

static tChar characterCreate(char character) {
    /*
     * Allocates one character node.
     *
     * Returns NULL when allocation fails. This helper is private to this
     * translation unit.
     */
    tChar char_ = malloc(sizeof(struct sChar));
    if (char_ == NULL) return NULL;

    char_->character = character;
    char_->next = NULL;
    return char_;
}

static void characterFree(tChar* char_) {
    /* Frees one character node and clears the caller's pointer. */
    if (char_ == NULL || *char_ == NULL) return;
    free(*char_);
    *char_ = NULL;
}

static void characterFreeAll(tChar* char_) {
    /*
     * Frees every node in a character list and clears the list pointer.
     *
     * This helper is private to this translation unit.
     */
    if (char_ == NULL || *char_ == NULL) return;

    tChar tmp = *char_;
    tChar next_char = tmp;
    while (tmp != NULL) {
        next_char = tmp->next;
        characterFree(&tmp);
        tmp = next_char;
    }
    *char_ = NULL;
}

static void stringFree(tString* string) {
    /*
     * Destroys a string whose reference count is already zero.
     *
     * This helper is private to this translation unit. It frees the complete
     * character list and the String object, and does nothing for NULL
     * pointers or objects that still have outstanding references.
     */
    if (string == NULL || *string == NULL) return;

    tString tmp = *string;

    if (tmp->references != 0) return;

    characterFreeAll(&(tmp->data));
    free(tmp);

    *string = NULL;
}

void stringRelease(tString* string) {
    /*
     * Drops one ownership reference and destroys the String at zero.
     *
     * A zero reference count is treated as an invalid release state and is
     * left unchanged. Passing NULL is allowed and ignored.
     */
    if (string == NULL || *string == NULL) {
        return;
    }

    tString current = *string;

    if (current->references == 0) {
        /* A zero count indicates an invalid or already released object. */
        return;
    }

    current->references--;

    stringFree(string); // free only if references = 0
}

void stringRetain(tString string) {
    /* Adds one ownership reference to a live String object. */
    if (string == NULL) return;

    string->references++;
}

tString stringCreate(const char* data) {
    /*
     * Creates a reference-counted String from a NUL-terminated C string.
     *
     * Characters are inserted into a singly linked list in source order.
     * Returns NULL when data is NULL or when any allocation fails. The
     * returned object starts with one ownership reference.
     */
    if (data == NULL) return NULL;

    tString new_string = malloc(sizeof(struct sString));
    if (new_string == NULL) return NULL;

    new_string->references = 1;
    new_string->size = 0;
    new_string->data = NULL;

    size_t i = strlen(data)+1;
    tChar next_char = NULL;
    while (i-1 > 0) {
        const tChar char_ = characterCreate(data[i-1]);
        if (char_ == NULL) {
            characterFreeAll(&next_char);
            stringRelease(&new_string); // free the new string because reference = 1
            return NULL;
        }
        char_->next = next_char;
        next_char = char_;
        i--;
        new_string->size++;
    }
    new_string->data = next_char;
    return new_string;
}
