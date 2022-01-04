CC = gcc
CFLAGS = -pedantic -ansi -Wall
OBJECTS = main.o ManipulationsFichier.o ByteUtils.o LabelLib.o module_emulateur.o module_compilateur.o
BIN = main
RM = rm -f

all: $(OBJECTS)
	@$(CC) $(OBJECTS) -o $(BIN) $(CFLAGS)

main.o: main.c ManipulationsFichier.h module_emulateur.h
	@$(CC) -c $< -o $@ $(CFLAGS)

ManipulationsFichier.o: ManipulationsFichier.c ManipulationsFichier.h module_compilateur.h
	@$(CC) -c $< -o $@ $(CFLAGS)

ByteUtils.o: ByteUtils.c ByteUtils.h
	@$(CC) -c $< -o $@ $(CFLAGS)

module_emulateur.o: module_emulateur.c module_emulateur.h
	@$(CC) -c $< -o $@ $(CFLAGS)

LabelLib.o: LabelLib.c LabelLib.h
	@$(CC) -c $< -o $@ $(CFLAGS)

module_compilateur.o: module_compilateur.c module_compilateur.h ByteUtils.h operation_code_defines.h
	@$(CC) -c $< -o $@ $(CFLAGS)

clean:
	@$(RM) *.o $(BIN)





test: test.o module_compilateur.o ByteUtils.o
	@$(CC) $^ -o $@ $(CFLAGS)
test.o: test.c module_compilateur.h
	@$(CC) -c $< -o $@ $(CFLAGS)