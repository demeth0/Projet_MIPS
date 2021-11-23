CC = gcc
CFLAGS = -pedantic -ansi -Wall #-D MATRICE_CREUSE_DEBUG
OBJECTS = main.o textToBinary.o
BIN = main
RM = rm -f

all: $(OBJECTS)
	@$(CC) $(OBJECTS) -o $(BIN) $(CFLAGS)
main.o: main.c 
	@$(CC) -c $< -o $@ $(CFLAGS)

textToBinary.o: textToBinary.c textToBinary.h operation_code_defines.h 
	@$(CC) -c $< -o $@ $(CFLAGS)
clean:
	@$(RM) $(OBJECTS) $(BIN)