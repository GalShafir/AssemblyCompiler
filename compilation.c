#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pre_processor.h"
#include "global_definitions.h"
#include "compilation.h"
#include "identification.h"
#include "error_handling.h"
#include "analysis.h"

int compile(char * fileName){

    FILE *inputFile = NULL;                       /* File pointer for the input file */
    char PreProcessedFileName[MAX_LINE_LENGTH];   /* Buffer to store the output file name of the pre-processed file */
    char manipulatedFileName[MAX_LINE_LENGTH];    /* Buffer to store the input file name */

    strcpy(manipulatedFileName, fileName);

    /**
    HashTable *instructionsHash        = create_table(HT_CAPACITY);
    HashTable *symbolsLabelsValuesHash = create_table(HT_CAPACITY);
    HashTable *symbolsLabelsMemoryHash = create_table(HT_CAPACITY);
    */

    /* Add the ".as" extension to the input file name */
    sprintf(manipulatedFileName, "%s.as", manipulatedFileName);

    /* Check the file even exists */
    inputFile = openFile(manipulatedFileName, "r");
    if (inputFile == NULL) {
        return 1;
    }
    else {
        fclose(inputFile);
    }

    /* Pre-process the file */
    preProcessFile(manipulatedFileName);

    /* Process the pre-processed file */
    sprintf(PreProcessedFileName, "%s.am", removeFileExtension(manipulatedFileName));

    printf("Processing file %s...\n", PreProcessedFileName);

    processFile(PreProcessedFileName);



    return EXIT_SUCCESS;
}

void processFile(char *inputFileName) {

    char line[MAX_LINE_LENGTH];             /* Buffer to store each line from the file */
    FILE *inputFile = NULL;                 /* File pointer for the input file */
    CommandType commandType;                /* Type of the command in the line */
    int lineNumber = 0;                    /* Counter for the line number */
    int directiveOrder = 0;               /* Counter for the directive order */
    bool foundError = False;              /* Flag to indicate if an error was found */
    int currentMemoryAddress = STARTING_MEMORY_LOCATION;       /* Counter for the current memory address */
    
    HashTable *instructionsHash = create_table(HT_CAPACITY); /* Create the instruction table */
    HashTable *symbolsLabelsValuesHash = create_table(HT_CAPACITY); /* Create the symbols-labels values table */
    HashTable *entriesExternsHash = create_table(HT_CAPACITY); /* Create the entries-externs table */
    HashTable *registersHash = create_table(HT_CAPACITY); /* Create the registers table */

    createInstructionTable(instructionsHash);
    createRegistersTable(registersHash);

    /* Open the input file */
    inputFile = openFile(inputFileName, "r");
    if (inputFile == NULL) {
        free_table(instructionsHash);
        free_table(symbolsLabelsValuesHash);
        return;
    }

    printf("------------------------------------------- Directive errors -------------------------------------------\n");

    /* Read lines from the input file - first iteration for error checking */
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        
        lineNumber++;
        
        /* Identify the command type */
        commandType = identifyCommandType(line, instructionsHash);

        /* Check for errors */
        check_errors(commandType, line, lineNumber, inputFileName, symbolsLabelsValuesHash, &directiveOrder, entriesExternsHash, &foundError);
    }

    /* Move the file pointer to the beginning of the file */

    rewind(inputFile);

    /* Reset the line number */
    lineNumber = 0;  

    /* Read lines from the input file - second iteration for checking externs and entries (because entry can be declared before the actual) */
    printf("------------------------------------------- Entries / Exters errors -------------------------------------------\n");
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        
        lineNumber++;
        
        /* Identify the command type */
        commandType = identifyCommandType(line, instructionsHash);

        /* Check for errors */
        check_entries_externs_errors(commandType, line, lineNumber, inputFileName, entriesExternsHash, symbolsLabelsValuesHash, &foundError);
    }

    /* Move the file pointer to the beginning of the file */

    rewind(inputFile);

    /* Reset the line number */
    lineNumber = 0;  

    printf("------------------------------------------- Instructions errors -------------------------------------------\n");

    /* Read lines from the input file - third iteration for checking instructions errors after we have the symbols labels hash */
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        
        lineNumber++;
        
        /* Identify the command type */
        commandType = identifyCommandType(line, instructionsHash);

        /* Check for errors */
        check_instruction_errors(commandType, line, lineNumber, inputFileName, entriesExternsHash, symbolsLabelsValuesHash, &foundError);
    }

    if (foundError == True) {
        free_table(instructionsHash);
        free_table(symbolsLabelsValuesHash);
        free_table(entriesExternsHash);
        fclose(inputFile);
        printf("Errors found in file %s. Compilation aborted.\n", inputFileName);
        return;
    }

    /* Move the file pointer to the beginning of the file */

    rewind(inputFile);

    /* Reset the line number */
    lineNumber = 0; 
    directiveOrder = 0;
    printf("------------------------------------------- Memory Calculations -------------------------------------------\n");

    /* Read lines from the input file - Get the address memory for each instruction line - now we know there are no syntax errors */
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        
        lineNumber++;
        
        /* Identify the command type */
        commandType = identifyCommandType(line, instructionsHash);

        /* Check for errors */
        calculate_memory_addresses_for_instructions(commandType, line, symbolsLabelsValuesHash, &directiveOrder, entriesExternsHash,&currentMemoryAddress);
    }

    /* Move the file pointer to the beginning of the file */

    rewind(inputFile);

    /* Reset the line number */
    lineNumber = 0; 
    directiveOrder = 0;
    printf("------------------------------------------- Memory Calculations -------------------------------------------\n");

    /* Read lines from the input file - Get the address memory for each directive line - now we know there are no syntax errors */
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        
        lineNumber++;
        
        /* Identify the command type */
        commandType = identifyCommandType(line, instructionsHash);

        /* Check for errors */
        calculate_memory_addresses_for_directives(commandType, line, symbolsLabelsValuesHash, &directiveOrder, entriesExternsHash,&currentMemoryAddress);
    }

    /* Close the input file */
    fclose(inputFile);
    
    printf("------------------------------------------- Binary File Creation -------------------------------------------\n");
    build_binary_file(inputFileName, symbolsLabelsValuesHash, entriesExternsHash, instructionsHash, registersHash);



    printf("------------------------------------------- Encoded file creation -------------------------------------------\n");
    build_encoded_file(inputFileName, symbolsLabelsValuesHash, currentMemoryAddress);


    print_table(symbolsLabelsValuesHash);
    print_directives_by_order(symbolsLabelsValuesHash);
    print_table(entriesExternsHash);



    free_table(instructionsHash);
    free_table(symbolsLabelsValuesHash);
    free_table(entriesExternsHash);
    free_table(registersHash);


}

/** Create the Instruction Table
 *
 * This function creates the instruction table and stores it in a hash map.
 *
 * @param mp - Pointer to the hash map to store the instruction table.
 */

void createInstructionTable(HashTable* mp){

    ht_insert(mp, "mov", "0000",  "instruction", "no address", "no size", "no order");
    ht_insert(mp, "cmp", "0001",  "instruction", "no address", "no size", "no order");
    ht_insert(mp, "add", "0010",  "instruction", "no address", "no size", "no order");
    ht_insert(mp, "sub", "0011",  "instruction", "no address", "no size", "no order");
    ht_insert(mp, "not", "0100",  "instruction", "no address", "no size", "no order");
    ht_insert(mp, "clr", "0101",  "instruction", "no address", "no size", "no order");
    ht_insert(mp, "lea", "0110",  "instruction", "no address", "no size", "no order");
    ht_insert(mp, "inc", "0111",  "instruction", "no address", "no size", "no order");
    ht_insert(mp, "dec", "1000",  "instruction", "no address", "no size", "no order");
    ht_insert(mp, "jmp", "1001",  "instruction", "no address", "no size", "no order");
    ht_insert(mp, "bne", "1010", "instruction", "no address", "no size", "no order");
    ht_insert(mp, "red", "1011", "instruction", "no address", "no size", "no order");
    ht_insert(mp, "prn", "1100", "instruction", "no address", "no size", "no order");
    ht_insert(mp, "jsr", "1101", "instruction", "no address", "no size", "no order");
    ht_insert(mp, "rts", "1110", "instruction", "no address", "no size", "no order");
    ht_insert(mp, "hlt", "1111", "instruction", "no address", "no size", "no order");
}

void createRegistersTable(HashTable* mp){

    ht_insert(mp, "r0", "000",  "register", "no address", "no size", "no order");
    ht_insert(mp, "r1", "001",  "register", "no address", "no size", "no order");
    ht_insert(mp, "r2", "010",  "register", "no address", "no size", "no order");
    ht_insert(mp, "r3", "011",  "register", "no address", "no size", "no order");
    ht_insert(mp, "r4", "100",  "register", "no address", "no size", "no order");
    ht_insert(mp, "r5", "101",  "register", "no address", "no size", "no order");
    ht_insert(mp, "r6", "110",  "register", "no address", "no size", "no order");
    ht_insert(mp, "r7", "111",  "register", "no address", "no size", "no order");
}

void print_directives_by_order(HashTable *table)
{
    int i, j, order;
    printf("\nDirectives Hash Table\n-------------------\n");
    for (j = 0; j < table -> size; j++){

        for (i = 0; i < table -> size; i++)
        {
            if ((table -> items[i]) && (table -> items[i] -> order != NULL)){
                order = stringToInt(table -> items[i] -> order);
                if (order == j && (strcmp(table -> items[i] -> type, "dataDirective") == 0 || strcmp(table -> items[i] -> type, "stringDirective") == 0))
                {
                    printf("Index:%d, Key:%s, Value:%s, Type:%s, Address:%s, Memory Size:%s, Order:%s\n", i, table -> items[i] -> key, table -> items[i] -> value, table -> items[i] -> type, table -> items[i] -> address, table -> items[i] -> memorySize, table -> items[i] -> order);
                }
            }
        }
    }

    printf("-------------------\n\n");
}

