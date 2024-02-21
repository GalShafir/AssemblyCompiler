#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pre_processor.h"
#include "global_definitions.h"
#include "utility_functions.h"


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

void processMacros(char *inputFileName) {

    char line[MAX_LINE_LENGTH];             /* Buffer to store each line from the file */
    char outputFileName[MAX_LINE_LENGTH];   /* Buffer to store the output file name */
    int macroCount = 0;                     /* Count of detected macros */
    int i;                                  /* Loop counter */
    size_t contentLen;                      /* Length of the macro content */
    char ** splitedMacro;                   /* Array to store the splited macro */
    char ** splitedLine;                    /* Array to store the splited line */
    int numberOfElements = 0;               /* Number of elements in the splited line */
    bool holdWriting = False;               /* Flag to hold the writing of the line */
    bool isMacroCall = False;               /* Flag to check if the line contains a macro call */
    Macro *macros = NULL;                   /* Array to store information about macros */
    FILE *inputFile = NULL;                 /* File pointer for the input file */
    FILE *outputFile = NULL;                /* File pointer for the output file */


    /* Add the ".as" extension to the input file name */
    sprintf(inputFileName, "%s.as", inputFileName);

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
            splitedMacro = splitString(line, " ", &numberOfElements);

            /* copy the macro name to the macros structure */
            strcpy(macros[macroCount].name, splitedMacro[1]);

            /* Free the memory allocated for the splitedMacro */
            freeStringArray(splitedMacro, numberOfElements);

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

        /* Reset the elemnts number - for the string spliter counter */
        numberOfElements = 0;

        /* Check if the line contains a macro call */
        splitedLine = splitString(line, " ", &numberOfElements);

        /* Remove the white spaces from the first element */
        removeWhiteSpaces(splitedLine[0]);

        /* Check if the line contains a macro definition or endmcr */
        if (strcmp(splitedLine[0], "mcr") == 0) {
            holdWriting = True;
        }

        /* Check if the line contains a macro ending */
        else if (strcmp(splitedLine[0], "endmcr") == 0) {
            holdWriting = False;
        }

        for (i = 0; i < macroCount; i++) {
            /* Check if the line contains a macro call but is not a macro definition or endmcr */
            if ((strstr(line, macros[i].name) != NULL) && (strcmp(splitedLine[0], "mcr") != 0) && (strcmp(splitedLine[0], "endmcr") != 0)){
                /* Replace the macro call with the macro content */
                fputs(macros[i].content, outputFile);
                isMacroCall = True;
                break;  /* Skip the original macro call line */
            }
        }

        /* Print the line as it is if it doesn't contain a macro call or is within a macro definition or endmcr */
        if (holdWriting == False && strcmp(splitedLine[0], "endmcr") != 0 && isMacroCall == False) {
            fputs(line, outputFile);
        }

        /* Reset the isMacroCall flag */
        isMacroCall = False;

        /* Free the memory allocated for the splitedLine */
        freeStringArray(splitedLine, numberOfElements);
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
 * Print Detected Macros
 *
 * This function prints the names and contents of the detected macros.
 *
 * @param macros - Array of macros to print.
 * @param macroCount - Number of macros in the array.
 */

void printMacros(const Macro *macros, int macroCount) {
    int i;
    printf("Detected Macros:\n");
    for (i = 0; i < macroCount; i++) {
        printf("Macro Name: %s\n", macros[i].name);
        printf("Macro Content:\n%s\n", macros[i].content);
        printf("------------------------------\n");
    }
}



