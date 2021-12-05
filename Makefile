CC = gcc
CFLAGS = -pedantic -ansi -Wall
OBJECTS = main.o textToBinary.o ManipulationsFichier.o ByteUtils.o
BIN = main
RM = rm -f

all: $(OBJECTS)
	@$(CC) $(OBJECTS) -o $(BIN) $(CFLAGS)

main.o: main.c 
	@$(CC) -c $< -o $@ $(CFLAGS)



test: test.o textToBinary.o ManipulationsFichier.o
	@$(CC) $^ -o $@ $(CFLAGS)

test.o: test.c textToBinary.h operation_code_defines.h ManipulationsFichier.h register_defines.h ByteUtils.h
	@$(CC) -c $< -o $@ $(CFLAGS)

textToBinary.o: textToBinary.c textToBinary.h operation_code_defines.h ByteUtils.h
	@$(CC) -c $< -o $@ $(CFLAGS)

ManipulationsFichier.o: ManipulationsFichier.c ManipulationsFichier.h textToBinary.h 
	@$(CC) -c $< -o $@ $(CFLAGS)

ByteUtils.o: ByteUtils.c ByteUtils.h
	@$(CC) -c $< -o $@ $(CFLAGS)

clean:
	@$(RM) $(OBJECTS) $(BIN)