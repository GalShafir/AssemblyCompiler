#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pre_processor.h"
#include "global_definitions.h"


FILE* openFile(const char * fileName, const char * accessMode) {
    
    FILE *file = NULL;

    /* Open the file for reading and check for errors */
    file = fopen(fileName, accessMode);
    if (file == NULL) {
        perror(FILE_OPEN_ERROR);
        return NULL;
    }

    return file;
}

int preProcessFile(char * fileName) {
    
    printf("Pre-processing file %s...\n", fileName);
    processMacros(fileName);
    return 0;
}

/**
 * Process Macros in the Input Assembly File and Write to a New File
 *
 * This function reads an assembly file, identifies macro definitions, and replaces
 * macro calls with their content. It writes the processed output to a new file with
 * the same name but a ".am" extension.
 *
 * @param inputFileName - Name of the input assembly file.
 */
void processMacros(const char *inputFileName) {

    char line[MAX_LINE_LENGTH];             /* Buffer to store each line from the file */
    char outputFileName[MAX_LINE_LENGTH];   /* Buffer to store the output file name */
    int macroCount = 0;                     /* Count of detected macros */
    int i;                                  /* Loop counter */
    size_t contentLen;                      /* Length of the macro content */
    Macro *macros = NULL;                   /* Array to store information about macros */
    FILE *inputFile = NULL;                 /* File pointer for the input file */
    FILE *outputFile = NULL;                /* File pointer for the output file */

    /* Open the input file */
    inputFile = openFile(inputFileName, "r");

    /* Allocate memory for the array of macros */
    macros = malloc(MAX_MACROS * sizeof(Macro));

    /* Check for memory allocation errors */
    if (macros == NULL) {
        perror(MEMORY_ALLOCATION_ERROR);
        exit(EXIT_FAILURE);
    }

    /* Read lines from the input file to detect and store macro definitions */
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        /* Check if the line contains a macro definition */
        if (strstr(line, "mcr") != NULL) {
            /* Extract the macro name */
            sscanf(line, "mcr %s", macros[macroCount].name);

            /* Initialize content with an empty string */
            macros[macroCount].content = malloc(1);

            /* Check for memory allocation errors */
            if (macros[macroCount].content == NULL) {
                perror(MEMORY_ALLOCATION_ERROR);
                exit(EXIT_FAILURE);
            }

            macros[macroCount].content[0] = '\0';

            /* Read the macro content until "endmcr" is encountered */
            macroCount++;
            while (fgets(line, sizeof(line), inputFile) != NULL) {
                /* Check for the end of the macro */
                if (strstr(line, "endmcr") != NULL) {
                    break;
                }

                /* Dynamically allocate memory and concatenate the macro content */
                contentLen = strlen(macros[macroCount - 1].content);
                macros[macroCount - 1].content = realloc(macros[macroCount - 1].content, contentLen + strlen(line) + 1);

                /* Check for memory allocation errors */
                if (macros[macroCount - 1].content == NULL) {
                    perror(MEMORY_ALLOCATION_ERROR);
                    exit(EXIT_FAILURE);
                }

                strcat(macros[macroCount - 1].content, line);
            }
        }
    }

    /* Close the input file */
    fclose(inputFile);

    /* Open a new file with the same name but a ".am" extension for writing */
    sprintf(outputFileName, "%s.am", removeFileExtension(inputFileName));
    outputFile = openFile(outputFileName, "w");

    /* Open the input file again for processing */
    inputFile = openFile(inputFileName, "r");

    /* Process the input file, replacing macro calls with macro content */
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        /* Check if the line contains a macro call */
        for (i = 0; i < macroCount; i++) {
            if (strstr(line, macros[i].name) != NULL) {
                /* Replace the macro call with the macro content */
                fputs(macros[i].content, outputFile);
                break;  /* Skip the original macro call line */
            }
        }

        /* Print the line as it is if it doesn't contain a macro call or is within a macro definition */
        if (strstr(line, "mcr") == NULL && strstr(line, "endmcr") == NULL) {
            fputs(line, outputFile);
        }
    }

    /* Close the input and output files */
    fclose(inputFile);
    fclose(outputFile);

    /* Free dynamically allocated memory for macros */
    for (i = 0; i < macroCount; i++) {
        free(macros[i].content);
    }

    free(macros);
}


/**
 * Remove File Extension from the Input File Name
 *
 * This function takes an input file name and removes the file extension (if exists).
 * It returns a new dynamically allocated string containing the modified file name.
 *
 * @param inputFileName - The input file name to process.
 * @return char* - A dynamically allocated string containing the file name without the extension.
 */
char *removeFileExtension(const char *inputFileName) {

    const char *dotPosition;
    size_t length;
    char *outputFileName;

    /* Find the last occurrence of '.' in the input file name */
    dotPosition = strrchr(inputFileName, '.');
    if (dotPosition != NULL) {
        /* Calculate the length of the file name without the extension */
        size_t length = dotPosition - inputFileName;

        /* Allocate memory for the new string */
        outputFileName = (char *)malloc(length + 1);

        /* Check for memory allocation errors */
        if (outputFileName == NULL) {
            perror(MEMORY_ALLOCATION_ERROR);
            exit(EXIT_FAILURE);
        }

        /* Copy the file name without the extension */
        strncpy(outputFileName, inputFileName, length);

        /* Null-terminate the string */
        outputFileName[length] = '\0';

        return outputFileName;
    } else {
        /* No dot found, return a copy of the entire input file name */
        length = strlen(inputFileName);

        /* Allocate memory for the new string */
        outputFileName = (char *)malloc(length + 1);

        /* Check for memory allocation errors */
        if (outputFileName == NULL) {
            perror(MEMORY_ALLOCATION_ERROR);
            exit(EXIT_FAILURE);
        }

        /* Copy the entire input file name */
        strcpy(outputFileName, inputFileName);

        return outputFileName;
    }
}


