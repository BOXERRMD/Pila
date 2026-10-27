CC=gcc
CFLAGS=-std=c11 -Wall -Werror -pedantic
EXE=pila
BUILD_DIR=BUILD/
OBJ_DIR=OBJ/
TEST_DIR=test
OBJ=main.o pile.o

$(EXE): $(OBJ)
	if [ -d "$(BUILD_DIR)" ]; then rm -rf "$(BUILD_DIR)"; fi
	mkdir -p $(BUILD_DIR)
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(OBJ_DIR)*.o -o $(BUILD_DIR)$(EXE)
	if [ $$? -eq 0 ]; then echo "Build successfully done !"; else "Build failed..."; fi


main.o: main.c
	$(CC) $(CFLAGS) main.c -c -o $(OBJ_DIR)main.o

pile.o: MAKEPILE/pile.c
	$(CC) $(CFLAGS) MAKEPILE/pile.c -c -o $(OBJ_DIR)pile.o

test_pile: pile.o test
	make -C $(TEST_DIR) TARGET=pile

clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)
	make -C test/ clean

clean_test:
	make -C test/ clean