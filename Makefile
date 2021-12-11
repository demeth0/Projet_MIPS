CC = gcc
CFLAGS = -pedantic -ansi -Wall
OBJECTS = main.o InstructionCompiler.o ManipulationsFichier.o ByteUtils.o LabelLib.o MipsSimulateur.o
BIN = main
RM = rm -f

all: $(OBJECTS)
	@$(CC) $(OBJECTS) -o $(BIN) $(CFLAGS)

main.o: main.c 
	@$(CC) -c $< -o $@ $(CFLAGS)

InstructionCompiler.o: InstructionCompiler.c InstructionCompiler.h operation_code_defines.h ByteUtils.h
	@$(CC) -c $< -o $@ $(CFLAGS)

ManipulationsFichier.o: ManipulationsFichier.c ManipulationsFichier.h InstructionCompiler.h 
	@$(CC) -c $< -o $@ $(CFLAGS)

ByteUtils.o: ByteUtils.c ByteUtils.h
	@$(CC) -c $< -o $@ $(CFLAGS)

MipsSimulateur.o: MipsSimulateur.c MipsSimulateur.h
	@$(CC) -c $< -o $@ $(CFLAGS)

LabelLib.o: LabelLib.c LabelLib.h
	@$(CC) -c $< -o $@ $(CFLAGS)

clean:
	@$(RM) *.o $(BIN)





test: test.o InstructionCompiler.o ManipulationsFichier.o ByteUtils.o LabelLib.o MipsSimulateur.o
	@$(CC) $^ -o $@ $(CFLAGS)
test.o: test.c InstructionCompiler.h operation_code_defines.h ManipulationsFichier.h register_defines.h ByteUtils.h
	@$(CC) -c $< -o $@ $(CFLAGS)