CC = gcc
CFLAGS = -pedantic -ansi -Wall #-D MATRICE_CREUSE_DEBUG
OBJECTS = main.o
BIN = main
RM = rm -f

all: $(OBJECTS)
	@$(CC) $(OBJECTS) -o $(BIN) $(CFLAGS)
main.o: main.c 
	@$(CC) -c $< -o $@ $(CFLAGS)
clean:
	@$(RM) $(OBJECTS) $(BIN)