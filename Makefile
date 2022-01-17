CC = gcc
CFLAGS = -pedantic -ansi -Wall
OBJECTS = main.o module_io.o module_byteutils.o module_emulateur.o module_compilateur.o module_ram.o
BIN = main
RM = rm -f

all: $(OBJECTS)
	@$(CC) $(OBJECTS) -o $(BIN) $(CFLAGS)

main.o: main.c module_io.h module_emulateur.h
	@$(CC) -c $< -o $@ $(CFLAGS)

module_compilateur.o: module_compilateur.c  module_byteutils.h operation_code_defines.h mips_structures.h
	@$(CC) -c $< -o $@ $(CFLAGS)

module_emulateur.o: module_emulateur.c module_emulateur.h mips_structures.h module_ram.h
	@$(CC) -c $< -o $@ $(CFLAGS)

module_ram.o: module_ram.c module_ram.h  mips_structures.h
	@$(CC) -c $< -o $@ $(CFLAGS)

module_io.o: module_io.c module_io.h mips_structures.h
	@$(CC) -c $< -o $@ $(CFLAGS)

module_byteutils.o: module_byteutils.c module_byteutils.h mips_structures.h
	@$(CC) -c $< -o $@ $(CFLAGS)


clean:
	@$(RM) *.o $(BIN)



test: test.o ByteUtils.o
	@$(CC) $^ -o $@ $(CFLAGS)
test.o: test.c ByteUtils.h
	@$(CC) -c $< -o $@ $(CFLAGS)