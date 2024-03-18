#include "utility_functions.h"

/**
 * @brief Compiles the given file.
 * 
 * @param fileName The name of the file to compile.
 * @return int 0 if the compilation was successful, 1 otherwise.
 */

int compile(char * fileName);

/**
 * @brief Processes the given file.
 * 
 * @param inputFileName The name of the file to process.
 */

void processFile(char *inputFileName);

/** createInstructionTable
 * @brief Creates the instruction table.
 * 
 * @param mp The hash table to create.
 */

void createInstructionTable(HashTable* mp);

/** createRegistersTable
 * @brief Creates the registers table.
 * 
 * @param mp The hash table to create.
 */

void createRegistersTable(HashTable* mp);

/** print_directives_by_order
 * @brief Prints the directives by order.
 * 
 * @param table The table to print.
 */

void print_directives_by_order(HashTable *table);