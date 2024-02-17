#include <stdio.h>
#include <stdlib.h>

#include "pre_processor.h"
#include "global_definitions.h"
#include "compilation.h"
#include "identification.h"

int compile(char * fileName){

    char PreProcessedFileName[MAX_LINE_LENGTH];   /* Buffer to store the output file name of the pre-processed file */

    HashTable *instructionsHash = create_table(HT_CAPACITY);


    /* Pre-process the file */
    preProcessFile(fileName);

    /* Process the pre-processed file */
    sprintf(PreProcessedFileName, "%s.am", removeFileExtension(fileName));


    processFile(PreProcessedFileName);


    /* Create the instruction table */
    createInstructionTable(instructionsHash);
    print_table(instructionsHash);
    free_table(instructionsHash);


    return EXIT_SUCCESS;
}

void processFile(char *inputFileName) {

    char line[MAX_LINE_LENGTH];             /* Buffer to store each line from the file */
    FILE *inputFile = NULL;                 /* File pointer for the input file */
    CommandType commandType;                /* Type of the command in the line */

    /* Create the instruction table */
    HashTable *instructionsHash = create_table(HT_CAPACITY);

    createInstructionTable(instructionsHash);

    /* Open the input file */
    inputFile = openFile(inputFileName, "r");

    /* Read lines from the input file */
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        /* Identify the command type */
        commandType = identifyCommandType(line, instructionsHash);
        switch (commandType) {
            case EMPTY:
                printf("empty line\n");
                break;
            case COMMENT:
                printf("comment line\n");
                break;
            case DATA_DIRECTIVE:
                printf("data directive line\n");
                break;
            case STRING_DIRECTIVE:
                printf("string directive line\n");
                break;
            case ENTRY_DIRECTIVE:
                printf("entry directive line\n");
                break;
            case EXTERN_DIRECTIVE:
                printf("extern directive line\n");
                break;
            case UNDEFINED_DIRECTIVE:
                printf("undifined directive line\n");
                break;
            case INSTRUCTION:
                printf("Instruction\n");
                break;
            case UNDEFINED:
                printf("undefined line\n");
                break;
        }
    }

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

    ht_insert(mp, "mov", "0");
    ht_insert(mp, "cmp", "1");
    ht_insert(mp, "add", "2");
    ht_insert(mp, "sub", "3");
    ht_insert(mp, "not", "4");
    ht_insert(mp, "clr", "5");
    ht_insert(mp, "lea", "6");
    ht_insert(mp, "inc", "7");
    ht_insert(mp, "dec", "8");
    ht_insert(mp, "jmp", "9");
    ht_insert(mp, "bne", "10");
    ht_insert(mp, "red", "11");
    ht_insert(mp, "prn", "12");
    ht_insert(mp, "jsr", "13");
    ht_insert(mp, "rts", "14");
    ht_insert(mp, "hlt", "15");
}

