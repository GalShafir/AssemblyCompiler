#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pre_processor.h"
#include "global_definitions.h"
#include "compilation.h"
#include "identification.h"
#include "error_handling.h"

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
    
    HashTable *instructionsHash = create_table(HT_CAPACITY); /* Create the instruction table */
    HashTable *symbolsLabelsValuesHash = create_table(HT_CAPACITY); /* Create the symbols-labels values table */
    HashTable *entriesExternsHash = create_table(HT_CAPACITY); /* Create the entries-externs table */

    createInstructionTable(instructionsHash);

    /* Open the input file */
    inputFile = openFile(inputFileName, "r");
    if (inputFile == NULL) {
        free_table(instructionsHash);
        free_table(symbolsLabelsValuesHash);
        return;
    }

    /* Read lines from the input file - first iteration for error checking */
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        
        lineNumber++;
        
        /* Identify the command type */
        commandType = identifyCommandType(line, instructionsHash);

        /* Check for errors */
        check_errors(commandType, line, lineNumber, inputFileName, symbolsLabelsValuesHash, &directiveOrder, entriesExternsHash);
    }

    /* Move the file pointer to the beginning of the file */

    rewind(inputFile);

    /* Reset the line number */
    lineNumber = 0;  

    /* Read lines from the input file - second iteration for checking externs and entries (because entry can be declared before the actual) */
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        
        lineNumber++;
        
        /* Identify the command type */
        commandType = identifyCommandType(line, instructionsHash);

        /* Check for errors */
        check_entries_externs_errors(commandType, line, lineNumber, inputFileName, entriesExternsHash, symbolsLabelsValuesHash);
    }

    /* Move the file pointer to the beginning of the file */

    rewind(inputFile);

    /* Reset the line number */
    lineNumber = 0;  

    /* Read lines from the input file - third iteration for checking instructions errors after we have the symbols labels hash */
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        
        lineNumber++;
        
        /* Identify the command type */
        commandType = identifyCommandType(line, instructionsHash);

        /* Check for errors */
        check_instruction_errors(commandType, line, lineNumber, inputFileName, entriesExternsHash, symbolsLabelsValuesHash);
    }


    print_table(symbolsLabelsValuesHash);

    print_directives_by_order(symbolsLabelsValuesHash);

    print_table(entriesExternsHash);

    free_table(instructionsHash);
    free_table(symbolsLabelsValuesHash);
    free_table(entriesExternsHash);


    /* Close the input file */
    fclose(inputFile);



}

/** Create the Instruction Table
 *
 * This function creates the instruction table and stores it in a hash map.
 *
 * @param mp - Pointer to the hash map to store the instruction table.
 */

void createInstructionTable(HashTable* mp){

    ht_insert(mp, "mov", "0",  "instruction", "no address", "no size", "no order");
    ht_insert(mp, "cmp", "1",  "instruction", "no address", "no size", "no order");
    ht_insert(mp, "add", "2",  "instruction", "no address", "no size", "no order");
    ht_insert(mp, "sub", "3",  "instruction", "no address", "no size", "no order");
    ht_insert(mp, "not", "4",  "instruction", "no address", "no size", "no order");
    ht_insert(mp, "clr", "5",  "instruction", "no address", "no size", "no order");
    ht_insert(mp, "lea", "6",  "instruction", "no address", "no size", "no order");
    ht_insert(mp, "inc", "7",  "instruction", "no address", "no size", "no order");
    ht_insert(mp, "dec", "8",  "instruction", "no address", "no size", "no order");
    ht_insert(mp, "jmp", "9",  "instruction", "no address", "no size", "no order");
    ht_insert(mp, "bne", "10", "instruction", "no address", "no size", "no order");
    ht_insert(mp, "red", "11", "instruction", "no address", "no size", "no order");
    ht_insert(mp, "prn", "12", "instruction", "no address", "no size", "no order");
    ht_insert(mp, "jsr", "13", "instruction", "no address", "no size", "no order");
    ht_insert(mp, "rts", "14", "instruction", "no address", "no size", "no order");
    ht_insert(mp, "hlt", "15", "instruction", "no address", "no size", "no order");
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

