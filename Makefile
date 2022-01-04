CC = gcc
CFLAGS = -pedantic -ansi -Wall
OBJECTS = main.o ManipulationsFichier.o ByteUtils.o LabelLib.o MipsSimulateur.o compiler.o
BIN = main
RM = rm -f

all: $(OBJECTS)
	@$(CC) $(OBJECTS) -o $(BIN) $(CFLAGS)

main.o: main.c ManipulationsFichier.h MipsSimulateur.h
	@$(CC) -c $< -o $@ $(CFLAGS)

ManipulationsFichier.o: ManipulationsFichier.c ManipulationsFichier.h compiler.h
	@$(CC) -c $< -o $@ $(CFLAGS)

ByteUtils.o: ByteUtils.c ByteUtils.h
	@$(CC) -c $< -o $@ $(CFLAGS)

MipsSimulateur.o: MipsSimulateur.c MipsSimulateur.h
	@$(CC) -c $< -o $@ $(CFLAGS)

LabelLib.o: LabelLib.c LabelLib.h
	@$(CC) -c $< -o $@ $(CFLAGS)

compiler.o: compiler.c compiler.h ByteUtils.h operation_code_defines.h
	@$(CC) -c $< -o $@ $(CFLAGS)

clean:
	@$(RM) *.o $(BIN)





test: test.o compiler.o ByteUtils.o
	@$(CC) $^ -o $@ $(CFLAGS)
test.o: test.c compiler.h
	@$(CC) -c $< -o $@ $(CFLAGS)