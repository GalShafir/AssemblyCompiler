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
    
    HashTable *instructionsHash = create_table(HT_CAPACITY); /* Create the instruction table */
    HashTable *symbolsLabelsValuesHash = create_table(HT_CAPACITY); /* Create the symbols-labels values table */

    createInstructionTable(instructionsHash);

    /* Open the input file */
    inputFile = openFile(inputFileName, "r");
    if (inputFile == NULL) {
        free_table(instructionsHash);
        free_table(symbolsLabelsValuesHash);
        return;
    }

    /* Read lines from the input file */
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        
        lineNumber++;
        
        /* Identify the command type */
        commandType = identifyCommandType(line, instructionsHash);

        /* Check for errors */
        check_errors(commandType, line, lineNumber, inputFileName, symbolsLabelsValuesHash);
    }

    print_table(symbolsLabelsValuesHash);

    free_table(instructionsHash);
    free_table(symbolsLabelsValuesHash);

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

    ht_insert(mp, "mov", "0", "instruction", "no address", "no size");
    ht_insert(mp, "cmp", "1", "instruction", "no address", "no size");
    ht_insert(mp, "add", "2", "instruction", "no address", "no size");
    ht_insert(mp, "sub", "3", "instruction", "no address", "no size");
    ht_insert(mp, "not", "4", "instruction", "no address", "no size");
    ht_insert(mp, "clr", "5", "instruction", "no address", "no size");
    ht_insert(mp, "lea", "6", "instruction", "no address", "no size");
    ht_insert(mp, "inc", "7", "instruction", "no address", "no size");
    ht_insert(mp, "dec", "8", "instruction", "no address", "no size");
    ht_insert(mp, "jmp", "9", "instruction", "no address", "no size");
    ht_insert(mp, "bne", "10", "instruction", "no address", "no size");
    ht_insert(mp, "red", "11", "instruction", "no address", "no size");
    ht_insert(mp, "prn", "12", "instruction", "no address", "no size");
    ht_insert(mp, "jsr", "13", "instruction", "no address", "no size");
    ht_insert(mp, "rts", "14", "instruction", "no address", "no size");
    ht_insert(mp, "hlt", "15", "instruction", "no address", "no size");
}

