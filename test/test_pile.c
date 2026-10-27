#include "../MAKEPILE/pile.h"
#include <stdio.h>
#include <stdlib.h>

#define VALUE_BOTTOM 44LL
#define VALUE_MIDDLE 78665467LL
#define VALUE_TOP -123456789LL

static void fail(const char* message, int code) {
    fprintf(stderr, "FAIL: %s\n", message);
    exit(code);
}

static void expect(int condition, const char* message, int code) {
    if (!condition) fail(message, code);
}

static void expect_size(tPile pile, size_t expected, int code) {
    if (pileSize(pile) != expected) {
        fprintf(stderr, "FAIL: expected pile size %zu, got %zu\n", expected, pileSize(pile));
        exit(code);
    }
}

static void test_new_pile(void) {
    tPile pile = pileCreate();
    expect(pile != NULL, "pileCreate returned NULL", 1);
    expect_size(pile, 0, 2);

    pile_value_t value = 0;
    expect(pileTop(pile, &value) != 0,
           "pileTop must fail on an empty pile", 3);
    expect(pilePop(pile, &value) != 0,
           "pilePop must fail on an empty pile", 4);
    expect_size(pile, 0, 5);

    pileFree(&pile);
    expect(pile == NULL, "pileFree must set the pile pointer to NULL", 6);
}

static void test_lifo_order(void) {
    tPile pile = pileCreate();
    expect(pile != NULL, "pileCreate returned NULL", 10);

    expect(pileAdd(pile, VALUE_BOTTOM) == 0,
           "pileAdd failed for the first value", 11);
    expect_size(pile, 1, 12);

    expect(pileAdd(pile, VALUE_MIDDLE) == 0,
           "pileAdd failed for the second value", 13);
    expect_size(pile, 2, 14);

    expect(pileAdd(pile, VALUE_TOP) == 0,
           "pileAdd failed for the third value", 15);
    expect_size(pile, 3, 16);

    pile_value_t value = 0;
    expect(pileTop(pile, &value) == 0 && value == VALUE_TOP,
           "pileTop did not return the newest value", 17);
    expect_size(pile, 3, 18);

    expect(pilePop(pile, &value) == 0 && value == VALUE_TOP,
           "pilePop returned the wrong top value", 19);
    expect_size(pile, 2, 20);

    expect(pilePop(pile, &value) == 0 && value == VALUE_MIDDLE,
           "pilePop did not preserve LIFO order", 21);
    expect_size(pile, 1, 22);

    expect(pilePop(pile, &value) == 0 && value == VALUE_BOTTOM,
           "pilePop returned the wrong bottom value", 23);
    expect_size(pile, 0, 24);

    expect(pilePop(pile, &value) != 0,
           "pilePop must fail after the pile becomes empty", 25);

    pileFree(&pile);
}

static void test_clear_and_reuse(void) {
    tPile pile = pileCreate();
    expect(pile != NULL, "pileCreate returned NULL", 30);

    expect(pileAdd(pile, VALUE_BOTTOM) == 0,
           "pileAdd failed before pileClear", 31);
    expect(pileAdd(pile, VALUE_MIDDLE) == 0,
           "pileAdd failed before pileClear", 32);
    expect_size(pile, 2, 33);

    pileClear(pile);
    expect_size(pile, 0, 34);

    pile_value_t value = 0;
    expect(pileTop(pile, &value) != 0,
           "pileTop must fail after pileClear", 35);
    expect(pilePop(pile, &value) != 0,
           "pilePop must fail after pileClear", 36);

    pileClear(pile);
    expect_size(pile, 0, 37);

    expect(pileAdd(pile, VALUE_TOP) == 0,
           "pile cannot be reused after pileClear", 38);
    expect(pileTop(pile, &value) == 0 && value == VALUE_TOP,
           "reused pile returned the wrong value", 39);
    expect_size(pile, 1, 40);

    pileFree(&pile);
}

static void test_null_free_operations(void) {
    tPile pile = NULL;

    pileFree(NULL);
    pileFree(&pile);
    expect(pile == NULL, "pileFree changed a NULL pile unexpectedly", 50);

    pileClear(NULL);
}

int main(void) {
    test_new_pile();
    test_lifo_order();
    test_clear_and_reuse();
    test_null_free_operations();

    printf("SUCCESS: all pile tests passed.\n");
    return 0;
}
