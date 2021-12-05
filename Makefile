CC = gcc
CFLAGS = -pedantic -ansi -Wall
OBJECTS = main.o textToBinary.o ManipulationsFichier.o
BIN = main
RM = rm -f

all: $(OBJECTS)
	@$(CC) $(OBJECTS) -o $(BIN) $(CFLAGS)

main.o: main.c 
	@$(CC) -c $< -o $@ $(CFLAGS)



test: test.o textToBinary.o
	@$(CC) $^ -o $@ $(CFLAGS)
test.o: test.c textToBinary.h operation_code_defines.h
	@$(CC) -c $< -o $@ $(CFLAGS)


textToBinary.o: textToBinary.c textToBinary.h operation_code_defines.h 
	@$(CC) -c $< -o $@ $(CFLAGS)

ManipulationsFichier.o: ManipulationsFichier.c ManipulationsFichier.h textToBinary.h 
	@$(CC) -c $< -o $@ $(CFLAGS)

clean:
	@$(RM) $(OBJECTS) $(BIN)