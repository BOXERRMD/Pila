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

static void release_pile(tPile* pile) {
    if (pile != NULL && *pile != NULL) {
        pileRelease(pile);
    }
}

static void test_new_pile(void) {
    tPile pile = pileCreate();
    Value value = makeValueNull();

    expect(pile != NULL, "pileCreate returned NULL", 1);
    expect(pile->references == 1, "new pile must have one reference", 2);
    expect_size(pile, 0, 3);

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
    Value output;

    expect(pile != NULL, "pileCreate returned NULL", 10);
    expect(pileAdd(pile, makeValueInteger(44)) == PILE_OK,
           "pileAdd failed for an integer", 11);
    expect(pileAdd(pile, makeValueFloat(3.25)) == PILE_OK,
           "pileAdd failed for a float", 12);
    expect(pileAdd(pile, makeValueBoolean(true)) == PILE_OK,
           "pileAdd failed for a boolean", 13);
    expect_size(pile, 3, 14);

    output = makeValueNull();
    expect(pileTop(pile, &output) == PILE_OK,
           "pileTop failed for a scalar", 15);
    expect(output.type == TYPE_BOOLEAN && output.data.boolean,
           "pileTop returned the wrong boolean", 16);
    freeValue(&output);
    expect_size(pile, 3, 17);

    output = makeValueNull();
    expect(pilePop(pile, &output) == PILE_OK,
           "pilePop failed for a boolean", 18);
    expect(output.type == TYPE_BOOLEAN && output.data.boolean,
           "pilePop returned the wrong boolean", 19);
    freeValue(&output);

    output = makeValueNull();
    expect(pilePop(pile, &output) == PILE_OK,
           "pilePop failed for a float", 20);
    expect(output.type == TYPE_FLOAT && output.data.floating == 3.25,
           "pilePop returned the wrong float", 21);
    freeValue(&output);

    output = makeValueNull();
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
    Value value = makeValueNull();

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

    expect(pileAdd(pile, makeValueInteger(1)) == PILE_OK,
           "pileAdd failed before NULL-buffer checks", 37);
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
    freeValue(&value);
    release_pile(&pile);
}

static void test_clear_and_reuse(void) {
    tPile pile = pileCreate();

    expect(pile != NULL, "pileCreate returned NULL", 50);
    expect(pileAdd(pile, makeValueInteger(1)) == PILE_OK,
           "first add failed", 51);
    expect(pileAdd(pile, makeValueInteger(2)) == PILE_OK,
           "second add failed", 52);

    pileClear(pile);
    expect_size(pile, 0, 53);
    expect(pile->top == NULL, "pileClear must clear the top pointer", 54);

    expect(pileAdd(pile, makeValueInteger(3)) == PILE_OK,
           "pile cannot be reused after pileClear", 55);
    expect_size(pile, 1, 56);

    pileClear(pile);
    pileClear(pile);
    expect_size(pile, 0, 57);
    release_pile(&pile);
}

static void test_string_creation(void) {
    const char* inputs[] = {"", "a", "pila", "chaine avec espaces"};
    size_t input_count = sizeof(inputs) / sizeof(inputs[0]);

    for (size_t i = 0; i < input_count; ++i) {
        tString string = stringCreate(inputs[i]);

        expect(string != NULL, "stringCreate failed", 60);
        expect(getStringReferences(string) == 1,
               "new string must have one reference", 61);
        expect(getStringData(string) != NULL || inputs[i][0] == '\0',
               "string data is unexpectedly NULL", 62);
        expect(strcmp(getStringData(string), inputs[i]) == 0,
               "stringCreate returned incorrect data", 63);

        stringRelease(&string);
        expect(string == NULL,
               "stringRelease must clear the last string pointer", 64);
    }

    expect(stringCreate(NULL) == NULL,
           "stringCreate must reject NULL input", 65);
}

static void test_string_reference_counting(void) {
    tPile pile = pileCreate();
    Value source = makeValueString("pila");
    Value top;
    Value popped;
    tString string = source.data.string;

    expect(pile != NULL, "pileCreate returned NULL", 70);
    expect(source.type == TYPE_STRING && string != NULL,
           "makeValueString failed", 71);
    expect(strcmp(getStringData(string), "pila") == 0,
           "makeValueString returned incorrect data", 72);
    expect(getStringReferences(string) == 1,
           "new Value string must have one reference", 73);

    expect(pileAdd(pile, source) == PILE_OK,
           "pileAdd failed for a string", 74);
    expect(getStringReferences(string) == 2,
           "pileAdd must retain the string", 75);
    freeValue(&source);
    expect(getStringReferences(string) == 1,
           "source release must preserve the pile string", 76);

    top = makeValueNull();
    expect(pileTop(pile, &top) == PILE_OK,
           "pileTop failed for a string", 77);
    expect(strcmp(getStringData(top.data.string), "pila") == 0,
           "pileTop returned incorrect string data", 78);
    expect(getStringReferences(string) == 2,
           "pileTop must retain the string", 79);
    freeValue(&top);
    expect(getStringReferences(string) == 1,
           "freeing pileTop result must release the string", 80);

    popped = makeValueNull();
    expect(pilePop(pile, &popped) == PILE_OK,
           "pilePop failed for a string", 81);
    expect(strcmp(getStringData(popped.data.string), "pila") == 0,
           "pilePop returned incorrect string data", 82);
    freeValue(&popped);
    expect_size(pile, 0, 83);

    release_pile(&pile);
}

static void test_value_copy_and_replacement(void) {
    Value source = makeValueString("source");
    Value destination = makeValueInteger(42);
    tString string = source.data.string;

    expect(source.type == TYPE_STRING && string != NULL,
           "source string creation failed", 90);
    expect(valueCopy(&destination, &source) == 0,
           "valueCopy failed", 91);
    expect(destination.type == TYPE_STRING &&
           strcmp(getStringData(destination.data.string), "source") == 0,
           "valueCopy returned incorrect string", 92);
    expect(getStringReferences(string) == 2,
           "valueCopy must retain the source string", 93);

    freeValue(&source);
    expect(getStringReferences(destination.data.string) == 1,
           "destination must retain the copied string", 94);
    freeValue(&destination);
}

static void test_nested_pile_reference_counting(void) {
    tPile outer = pileCreate();
    Value nested_value = makeValuePile();
    tPile nested = nested_value.data.pile;
    Value output;

    expect(outer != NULL && nested != NULL,
           "nested pile creation failed", 100);
    expect(nested->references == 1,
           "new nested pile must have one reference", 101);

    expect(pileAdd(nested, makeValueInteger(123)) == PILE_OK,
           "cannot add value to nested pile", 102);
    expect(pileAdd(outer, nested_value) == PILE_OK,
           "cannot add nested pile value", 103);
    expect(nested->references == 2,
           "adding a nested pile must retain it", 104);

    freeValue(&nested_value);
    expect(nested->references == 1,
           "releasing nested pile source must keep it alive", 105);

    output = makeValueNull();
    expect(pilePop(outer, &output) == PILE_OK,
           "cannot pop nested pile value", 106);
    expect(output.type == TYPE_PILE && output.data.pile == nested,
           "popped nested pile is incorrect", 107);
    freeValue(&output);

    release_pile(&outer);
}

static void test_pile_reference_counting(void) {
    tPile pile = pileCreate();
    tPile alias;

    expect(pile != NULL, "pileCreate returned NULL", 110);
    alias = pile;

    pileRetain(alias);
    expect(pile->references == 2,
           "pileRetain must increment references", 111);

    pileRelease(&alias);
    expect(alias == pile && pile->references == 1,
           "pileRelease must decrement references without destroying the pile",
           112);

    pileRelease(&pile);
    expect(pile == NULL, "last pileRelease must destroy the pile", 113);
}

static void test_value_constructors(void) {
    Value integer = makeValueInteger(42);
    Value floating = makeValueFloat(2.5);
    Value boolean = makeValueBoolean(true);
    Value null_value = makeValueNull();
    Value empty_string = makeValueString(NULL);

    expect(integer.type == TYPE_INTEGER && integer.data.integer == 42,
           "integer constructor failed", 120);
    expect(floating.type == TYPE_FLOAT && floating.data.floating == 2.5,
           "float constructor failed", 121);
    expect(boolean.type == TYPE_BOOLEAN && boolean.data.boolean,
           "boolean constructor failed", 122);
    expect(null_value.type == TYPE_NULL,
           "null constructor failed", 123);
    expect(empty_string.type == TYPE_STRING &&
           empty_string.data.string != NULL &&
           strcmp(getStringData(empty_string.data.string), "") == 0,
           "NULL string must produce an empty string", 124);

    freeValue(&integer);
    freeValue(&floating);
    freeValue(&boolean);
    freeValue(&null_value);
    freeValue(&empty_string);
}

int main(void) {
    test_new_pile();
    test_scalar_lifo();
    test_null_and_error_cases();
    test_clear_and_reuse();
    test_string_creation();
    test_string_reference_counting();
    test_value_copy_and_replacement();
    test_nested_pile_reference_counting();
    test_pile_reference_counting();
    test_value_constructors();

    printf("SUCCESS: all pile, string, and Value tests passed.\n");
    return 0;
}
