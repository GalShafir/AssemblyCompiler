#include "utility_functions.h"

int compile(char * fileName);
void processFile(char *inputFileName);
void createInstructionTable(HashTable* mp);
void print_directives_by_order(HashTable *table);