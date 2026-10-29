#include "../src/MAKEPILE/pile.h"
#include "../src/MAKEVALUE/value.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void fail(const char* message, int code) {
    fprintf(stderr, "FAIL: %s\n", message);
    exit(code);
}

static void expect(bool condition, const char* message, int code) {
    if (!condition) {
        fail(message, code);
    }
}

static void expect_size(tPile pile, size_t expected, int code) {
    if (pileSize(pile) != expected) {
        fprintf(stderr, "FAIL: expected pile size %zu, got %zu\n",
                expected, pileSize(pile));
        exit(code);
    }
}

static char* duplicate_string(const char* source) {
    size_t length;
    char* copy;

    if (source == NULL) {
        return NULL;
    }

    length = strlen(source) + 1;
    copy = malloc(length);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, source, length);
    return copy;
}

static Value make_null(void) {
    Value value;

    valueInit(&value);
    return value;
}

static Value make_integer(long long int integer) {
    Value value;

    valueInit(&value);
    value.type = TYPE_INTEGER;
    value.data.integer = integer;
    return value;
}

static Value make_float(double floating) {
    Value value;

    valueInit(&value);
    value.type = TYPE_FLOAT;
    value.data.floating = floating;
    return value;
}

static Value make_boolean(bool boolean) {
    Value value;

    valueInit(&value);
    value.type = TYPE_BOOLEAN;
    value.data.boolean = boolean;
    return value;
}

static Value make_string(const char* text) {
    Value value;

    valueInit(&value);
    value.data.string = malloc(sizeof(*value.data.string));
    if (value.data.string == NULL) {
        return value;
    }

    value.data.string->data = duplicate_string(text);
    if (value.data.string->data == NULL) {
        free(value.data.string);
        value.data.string = NULL;
        return value;
    }

    value.data.string->references = 1;
    value.type = TYPE_STRING;
    return value;
}

static Value make_pile_value(tPile pile) {
    Value value;

    valueInit(&value);
    value.type = TYPE_PILE;
    value.data.pile = pile;
    return value;
}

static void release_pile(tPile* pile) {
    if (pile != NULL && *pile != NULL) {
        pileRelease(pile);
    }
}

static void test_new_pile(void) {
    tPile pile = pileCreate();
    Value value;

    expect(pile != NULL, "pileCreate returned NULL", 1);
    expect(pile->references == 1, "new pile must have one reference", 2);
    expect_size(pile, 0, 3);

    value = make_null();
    expect(pileTop(pile, &value) == PILE_EMPTY_ERROR,
           "pileTop must reject an empty pile", 4);
    expect(pilePop(pile, &value) == PILE_EMPTY_ERROR,
           "pilePop must reject an empty pile", 5);
    expect_size(pile, 0, 6);

    freeValue(&value);
    release_pile(&pile);
    expect(pile == NULL, "pileRelease must destroy the last owner", 7);
}

static void test_scalar_lifo(void) {
    tPile pile = pileCreate();
    Value input;
    Value output;

    expect(pile != NULL, "pileCreate returned NULL", 10);

    input = make_integer(44);
    expect(pileAdd(pile, input) == PILE_OK,
           "pileAdd failed for an integer", 11);
    freeValue(&input);

    input = make_float(3.25);
    expect(pileAdd(pile, input) == PILE_OK,
           "pileAdd failed for a float", 12);
    freeValue(&input);

    input = make_boolean(true);
    expect(pileAdd(pile, input) == PILE_OK,
           "pileAdd failed for a boolean", 13);
    freeValue(&input);

    expect_size(pile, 3, 14);

    output = make_null();
    expect(pileTop(pile, &output) == PILE_OK,
           "pileTop failed for a scalar value", 15);
    expect(output.type == TYPE_BOOLEAN && output.data.boolean,
           "pileTop returned the wrong scalar value", 16);
    freeValue(&output);
    expect_size(pile, 3, 17);

    output = make_null();
    expect(pilePop(pile, &output) == PILE_OK,
           "pilePop failed for a boolean", 18);
    expect(output.type == TYPE_BOOLEAN && output.data.boolean,
           "pilePop returned the wrong boolean", 19);
    freeValue(&output);

    output = make_null();
    expect(pilePop(pile, &output) == PILE_OK,
           "pilePop failed for a float", 20);
    expect(output.type == TYPE_FLOAT && output.data.floating == 3.25,
           "pilePop returned the wrong float", 21);
    freeValue(&output);

    output = make_null();
    expect(pilePop(pile, &output) == PILE_OK,
           "pilePop failed for an integer", 22);
    expect(output.type == TYPE_INTEGER && output.data.integer == 44,
           "pilePop returned the wrong integer", 23);
    freeValue(&output);

    expect_size(pile, 0, 24);
    release_pile(&pile);
}

static void test_null_and_error_cases(void) {
    tPile pile = pileCreate();
    Value value = make_null();

    expect(pile != NULL, "pileCreate returned NULL", 30);
    expect(pileAdd(NULL, value) == PILE_NULL_ERROR,
           "pileAdd must reject a NULL pile", 31);
    expect(pilePop(NULL, &value) == PILE_NULL_ERROR,
           "pilePop must reject a NULL pile", 32);
    expect(pileTop(NULL, &value) == PILE_NULL_ERROR,
           "pileTop must reject a NULL pile", 33);
    expect(pileSize(NULL) == 0,
           "pileSize must return zero for a NULL pile", 34);

    expect(pileTop(pile, NULL) == PILE_EMPTY_ERROR,
           "empty pile must be checked before a NULL top buffer", 35);
    expect(pilePop(pile, NULL) == PILE_EMPTY_ERROR,
           "pop on an empty pile must fail", 36);

    value = make_integer(1);
    expect(pileAdd(pile, value) == PILE_OK,
           "pileAdd failed before NULL-buffer checks", 37);
    freeValue(&value);

    expect(pileTop(pile, NULL) == PILE_BUFF_NULL_ERROR,
           "pileTop must reject a NULL buffer", 38);
    expect(pilePop(pile, NULL) == PILE_OK,
           "pilePop must allow discarding the top value", 39);
    expect_size(pile, 0, 40);

    expect(valueCopy(NULL, &value) == VALUE_NULL_ERROR,
           "valueCopy must reject a NULL destination", 41);
    expect(valueCopy(&value, NULL) == VALUE_NULL_ERROR,
           "valueCopy must reject a NULL source", 42);

    freeValue(NULL);
    valueInit(NULL);
    pileClear(NULL);
    pileFree(NULL);
    release_pile(&pile);
    freeValue(&value);
}

static void test_clear_and_reuse(void) {
    tPile pile = pileCreate();
    Value value;

    expect(pile != NULL, "pileCreate returned NULL", 50);

    value = make_integer(1);
    expect(pileAdd(pile, value) == PILE_OK, "first add failed", 51);
    freeValue(&value);

    value = make_integer(2);
    expect(pileAdd(pile, value) == PILE_OK, "second add failed", 52);
    freeValue(&value);

    pileClear(pile);
    expect_size(pile, 0, 53);
    expect(pile->top == NULL, "pileClear must clear the top pointer", 54);

    value = make_integer(3);
    expect(pileAdd(pile, value) == PILE_OK,
           "pile cannot be reused after pileClear", 55);
    freeValue(&value);
    expect_size(pile, 1, 56);

    pileClear(pile);
    pileClear(pile);
    expect_size(pile, 0, 57);
    release_pile(&pile);
}

static void test_string_reference_counting(void) {
    tPile pile = pileCreate();
    Value source;
    Value top;
    Value popped;
    tString string;

    expect(pile != NULL, "pileCreate returned NULL", 60);

    source = make_string("pila");
    expect(source.type == TYPE_STRING && source.data.string != NULL,
           "string fixture allocation failed", 61);
    string = source.data.string;
    expect(string->references == 1,
           "new string must have one reference", 62);

    expect(pileAdd(pile, source) == PILE_OK,
           "pileAdd failed for a string", 63);
    expect(string->references == 2,
           "pileAdd must retain the string", 64);
    freeValue(&source);
    expect(string->references == 1,
           "releasing the source must keep the pile string alive", 65);

    top = make_null();
    expect(pileTop(pile, &top) == PILE_OK,
           "pileTop failed for a string", 66);
    expect(top.type == TYPE_STRING &&
           strcmp(top.data.string->data, "pila") == 0,
           "pileTop returned the wrong string", 67);
    expect(string->references == 2,
           "pileTop must retain the string", 68);
    freeValue(&top);
    expect(string->references == 1,
           "freeing pileTop result must release one reference", 69);

    popped = make_null();
    expect(pilePop(pile, &popped) == PILE_OK,
           "pilePop failed for a string", 70);
    expect(popped.type == TYPE_STRING &&
           strcmp(popped.data.string->data, "pila") == 0,
           "pilePop returned the wrong string", 71);
    expect(string->references == 1,
           "pilePop must transfer one live string reference", 72);
    freeValue(&popped);
    expect(popped.type == TYPE_NULL,
           "freeValue must reset the popped string", 73);

    expect_size(pile, 0, 74);
    release_pile(&pile);
}

static void test_string_clear_releases_references(void) {
    tPile pile = pileCreate();
    Value first;
    Value second;
    tString string;

    expect(pile != NULL, "pileCreate returned NULL", 80);

    first = make_string("shared");
    expect(first.type == TYPE_STRING, "first string fixture failed", 81);
    string = first.data.string;

    expect(valueCopy(&second, &first) == 0,
           "valueCopy failed for a string", 82);
    expect(string->references == 2,
           "valueCopy must retain a string", 83);

    expect(pileAdd(pile, first) == PILE_OK, "first string add failed", 84);
    expect(pileAdd(pile, second) == PILE_OK, "second string add failed", 85);
    expect(string->references == 4,
           "two pile copies must retain the shared string", 86);

    freeValue(&first);
    freeValue(&second);
    expect(string->references == 2,
           "releasing source values must preserve pile references", 87);

    pileClear(pile);
    expect_size(pile, 0, 88);
    release_pile(&pile);
}

static void test_nested_pile_reference_counting(void) {
    tPile outer = pileCreate();
    tPile nested = pileCreate();
    Value nested_value;
    Value output;

    expect(outer != NULL && nested != NULL,
           "nested pile creation failed", 90);

    nested_value = make_integer(123);
    expect(pileAdd(nested, nested_value) == PILE_OK,
           "cannot add value to nested pile", 91);
    freeValue(&nested_value);

    nested_value = make_pile_value(nested);
    expect(pileAdd(outer, nested_value) == PILE_OK,
           "cannot add nested pile value", 92);
    expect(nested->references == 2,
           "adding a nested pile must retain it", 93);

    freeValue(&nested_value);
    expect(nested->references == 1,
           "releasing nested pile source must keep pile alive", 94);

    output = make_null();
    expect(pilePop(outer, &output) == PILE_OK,
           "cannot pop nested pile value", 95);
    expect(output.type == TYPE_PILE && output.data.pile == nested,
           "popped nested pile is incorrect", 96);
    expect(nested->references == 1,
           "popping nested pile must preserve its ownership", 97);

    freeValue(&output);
    nested = NULL;
    release_pile(&outer);
}

static void test_pile_reference_counting(void) {
    tPile pile = pileCreate();
    tPile alias;

    expect(pile != NULL, "pileCreate returned NULL", 100);
    alias = pile;

    pileRetain(alias);
    expect(pile->references == 2,
           "pileRetain must increment references", 101);

    pileRelease(&alias);
    expect(alias != NULL && pile->references == 1,
           "pileRelease must keep a shared pile alive", 102);

    pileRelease(&pile);
    expect(pile == NULL, "last pileRelease must destroy the pile", 103);
}

int main(void) {
    test_new_pile();
    test_scalar_lifo();
    test_null_and_error_cases();
    test_clear_and_reuse();
    test_string_reference_counting();
    test_string_clear_releases_references();
    test_nested_pile_reference_counting();
    test_pile_reference_counting();

    printf("SUCCESS: all pile and Value tests passed.\n");
    return 0;
}
