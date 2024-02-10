# =========================================================

# $@ --> Represents the target of the rule
# $^ --> Represents all the dependencies of the rule

CC = gcc
CFLAGS = -Wall -ansi -g -pedantic
OBJECT_LIST = assembler.o compilation.o pre_processor.o
GLOBAL_HELPER = global_definitions.h


all: assembler

assembler: $(OBJECT_LIST) $(GLOBAL_HELPER) 
	$(CC) -g $(OBJECT_LIST) $(CFLAGS) -o $@

assembler.o: assembler.c assembler.h $(GLOBAL_HELPER)
	$(CC) $(CFLAGS) -c assembler.c -o $@

compilation.o: compilation.c compilation.h $(GLOBAL_HELPER)
	$(CC) $(CFLAGS) -c compilation.c -o $@

pre_processor.o: pre_processor.c pre_processor.h $(GLOBAL_HELPER)
	$(CC) $(CFLAGS) -c pre_processor.c -o $@

clean:
	rm -f assembler *.o *.am *.ob *.ent *.ext 