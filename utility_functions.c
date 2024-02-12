#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global_definitions.h"
#include "utility_functions.h"


/**
 * Open a File
 *
 * This function opens a file with the specified name and access mode.
 *
 * @param fileName - The name of the file to open.
 * @param accessMode - The access mode for opening the file.
 * @return A file pointer or NULL if the file cannot be opened.
 */
 
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

/**
 * Remove Whitespace Characters from the Input String
 *
 * This function removes whitespace characters from the input string.
 *
 * @param inputString - The input string to be modified.
 */

void removeWhiteSpaces(char *inputString) {
    int i, j = 0;
    int length = strlen(inputString);

    /* Iterate through the characters of the string */
    for (i = 0; i < length; i++) {
        /* If the current character is not a whitespace, copy it to the new position */
        if (inputString[i] != ' ' && inputString[i] != '\t' && inputString[i] != '\n' && inputString[i] != '\r') {
            inputString[j] = inputString[i];
            j++;
        }
    }

    /* Null-terminate the modified string */
    inputString[j] = '\0';
}



/**
 * Split inputString into an array of strings based on the specified separators.
 *
 * @param inputString - The input string to be split.
 * @param separators - The characters used as separators.
 * @param count - Pointer to an integer to store the number of strings in the result.
 * @return An array of strings or NULL if memory allocation fails.
 */
char **splitString(const char *inputString, const char *separators, int *count) {
    
    int length = strlen(inputString);
    char buffer[MAX_LINE_LENGTH + 2];
    int i = 0, j, k;

    /* Marks the starting index of a word */
    int startIndex; 
    int stringIndex = 0;
    int toAllocate; 

    char *stringCopy;
    char **resultStrings;


    /* Allocate memory for the copy of the input string */
    stringCopy = (char *)malloc((length + 1) * sizeof(char));
    if (stringCopy == NULL) {
        /* Failed to allocate memory for the copy */
        perror(MEMORY_ALLOCATION_ERROR);
        exit(EXIT_FAILURE);
    }

    strncpy(stringCopy, inputString, length + 1);

    /* Ignore the newline character in the last word */
    if (length > 0 && stringCopy[length - 1] == '\n') {
        stringCopy[length - 1] = '\0';
        length--;
    }

    /* Count the number of strings */
    *count = 0;
    while (i < length) {
        /* Skip the separators */
        while (i < length && strchr(separators, stringCopy[i]) != NULL) {
            i++;
        }
        /* Mark the starting index of a word */
        startIndex = i;
        while (i < length && strchr(separators, stringCopy[i]) == NULL) {
            i++;
        }
        /* If a word was found, increment the count */
        if (i > startIndex) {
            (*count)++;
        }
    }

    /* Allocate memory for the array of strings */
    resultStrings = (char **)malloc((*count) * sizeof(char *));
    if (resultStrings == NULL) {
        /* Failed to allocate memory for the array */
        free(stringCopy);
        return NULL;
    }

    /* Split the input string into strings */
    i = 0;
    while (i < length) {
        /* Skip the separators */
        while (i < length && strchr(separators, stringCopy[i]) != NULL) {
            i++;
        }
        /* Mark the starting index of a word */
        j = 0;
        while (i < length && strchr(separators, stringCopy[i]) == NULL) {

            /* Copy the word into the buffer */
            buffer[j] = stringCopy[i];
            i++;
            j++;

        }
        /* If a word was found, copy it into the result array */
        if (j > 0) {

            buffer[j] = '\0';

            /* Allocate memory for the string */
            toAllocate = (strlen(buffer) + 1) * sizeof(char);
            resultStrings[stringIndex] = (char *)malloc(toAllocate);

            if (resultStrings[stringIndex] == NULL) {
                /* Failed to allocate memory for a string */
                free(stringCopy);
                for (k = 0; k < stringIndex; k++) {
                    free(resultStrings[k]);
                }
                free(resultStrings);
                return NULL;
            }
            /* Copy the string into the result array */
            strcpy(resultStrings[stringIndex], buffer);
            
            stringIndex++;
        }
    }

    /* Free the copy of the input string */
    free(stringCopy);

    /* Return the array of strings */
    return resultStrings;
}

/**
 * Free the memory allocated for the array of strings.
 *
 * @param splittedStrings - The array of strings to be freed.
 * @param count - The number of strings in the array.
 */
void freeStringArray(char **splittedStrings, int count) {
    int i = 0;
    for (i = 0; i < count; i++) {
        free(splittedStrings[i]);
    }
    free(splittedStrings);
}

/**
 * Print the array of strings.
 *
 * @param strings - The array of strings to be printed.
 * @param count - The number of strings in the array.
 */
void printStringArray(char **strings, int count) {
    int i = 0;
    printf("String Array:\n");

    for (i = 0; i < count; i++) {
        printf("%s\n", strings[i]);
    }
}
