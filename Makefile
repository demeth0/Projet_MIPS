CC = gcc
CFLAGS = -pedantic -ansi -Wall
OBJECTS = main.o ManipulationsFichier.o ByteUtils.o module_emulateur.o module_compilateur.o module_ram.o
BIN = main
RM = rm -f

all: $(OBJECTS)
	@$(CC) $(OBJECTS) -o $(BIN) $(CFLAGS)

main.o: main.c ManipulationsFichier.h module_emulateur.h
	@$(CC) -c $< -o $@ $(CFLAGS)

module_compilateur.o: module_compilateur.c  ByteUtils.h operation_code_defines.h mips_structures.h
	@$(CC) -c $< -o $@ $(CFLAGS)

module_emulateur.o: module_emulateur.c module_emulateur.h mips_structures.h module_ram.h
	@$(CC) -c $< -o $@ $(CFLAGS)

module_ram.o: module_ram.c module_ram.h  mips_structures.h
	@$(CC) -c $< -o $@ $(CFLAGS)

ManipulationsFichier.o: ManipulationsFichier.c ManipulationsFichier.h mips_structures.h
	@$(CC) -c $< -o $@ $(CFLAGS)

ByteUtils.o: ByteUtils.c ByteUtils.h mips_structures.h
	@$(CC) -c $< -o $@ $(CFLAGS)


clean:
	@$(RM) *.o $(BIN)



test: test.o ByteUtils.o
	@$(CC) $^ -o $@ $(CFLAGS)
test.o: test.c ByteUtils.h
	@$(CC) -c $< -o $@ $(CFLAGS)