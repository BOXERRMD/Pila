CC=gcc
CFLAGS=-std=c11 -Wall -Werror -pedantic
EXE=pila

BUILD_DIR=BUILD/
OBJ_DIR=OBJ/

SRC=src/
MAKEPILE=$(SRC)MAKEPILE/
MAKEVALUE=$(SRC)MAKEVALUE/
MAKEVALUE_DATA=$(MAKEVALUE)/DATA/

TEST_DIR=test

OBJ=main.o pile.o value.o string.o

$(EXE): $(OBJ)
	if [ -d "$(BUILD_DIR)" ]; then rm -rf "$(BUILD_DIR)"; fi
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OBJ_DIR)*.o -o $(BUILD_DIR)$(EXE)
	if [ $$? -eq 0 ]; then echo "Build successfully done !"; else "Build failed..."; fi

OBJ:
	mkdir -p $(OBJ_DIR)

main.o: main.c
	make OBJ
	$(CC) $(CFLAGS) main.c -c -o $(OBJ_DIR)main.o

pile.o: $(MAKEPILE)
	make OBJ
	$(CC) $(CFLAGS) $(MAKEPILE)pile.c -c -o $(OBJ_DIR)pile.o

value.o: $(MAKEVALUE)
	make OBJ
	$(CC) $(CFLAGS) $(MAKEVALUE)value.c -c -o $(OBJ_DIR)value.o

string.o: $(MAKEVALUE_DATA)
	make OBJ
	$(CC) $(CFLAGS) $(MAKEVALUE_DATA)string.c -c -o $(OBJ_DIR)string.o

test_pile: pile.o test
	make -C $(TEST_DIR) TARGET=pile

clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)
	make -C test/ clean

clean_test:
	make -C test/ clean