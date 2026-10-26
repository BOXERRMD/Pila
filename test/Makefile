CC=gcc
CFLAGS=-std=c11 -Wall -Werror -pedantic

OBJ_DIR=../OBJ/

_: test_$(TARGET).o
	mkdir -p $(TARGET)
	if [ ! -f $(OBJ_DIR)$(TARGET).o ]; then echo "$(TARGET).o not found !"; exit 1; fi
	$(CC) $(CFLAGS) OBJ/test_$(TARGET).o $(OBJ_DIR)$(TARGET).o -o $(TARGET)/test_$(TARGET)
	./$(TARGET)/test_$(TARGET)

OBJ:
	mkdir -p OBJ/

test_pile.o: test_pile.c
	make OBJ
	$(CC) $(CFLAGS) test_pile.c -c -o OBJ/test_pile.o

clean:
	# clear all directory in test folder
	for file in *; do if [ -d "$$file" ]; then rm -rf "$$file"; fi; done
