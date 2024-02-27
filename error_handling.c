#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "global_definitions.h"
#include "utility_functions.h"
#include "identification.h"
#include "error_handling.h"

void check_errors(CommandType commandType, char *line, int lineNumber, char * fileName, HashTable *symbolsLabelsValuesHash) {
    switch (commandType) {
        case EMPTY:
            printf("empty line\n");
            break;
        case COMMENT:
            printf("comment line\n");
            break;
        case DATA_DIRECTIVE:
            printf("data directive line\n");
            check_data_directive_error(line, lineNumber, fileName, symbolsLabelsValuesHash);
            break;
        case STRING_DIRECTIVE:
            printf("string directive line\n");
            check_string_directive_error(line, lineNumber, fileName, symbolsLabelsValuesHash);
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
        case CONSTANT:
            printf("Constant\n");
            check_constant_error(line, lineNumber, fileName, symbolsLabelsValuesHash);
            break;
        case UNDEFINED:
            printf("undefined line\n");
            break;
    }
}

/**
 * Print an error message to the console.
 * @param error - The error message to be printed.
 * @param line - The line where the error occurred.
 * @param lineNumber - The line number where the error occurred.
 * @param fileName - The name of the file where the error occurred.
 */
void print_error(char *error, const char *line, int lineNumber, char *fileName) {

    /* Red color*/
    fprintf(stderr, "\033[1;31m"); 
    fprintf(stderr, "ERROR: ");

    /* Reset the color*/
    fprintf(stderr, "\033[0m"); 
    fprintf(stderr, "%s", error);

    /* Yellow color*/
    fprintf(stderr, "\033[0;33m");
    fprintf(stderr, "In File: ");

    /* Reset the color*/
    fprintf(stderr, "\033[0m"); 
    fprintf(stderr, "%s", fileName);

    /* Purple */
    fprintf(stderr, "\033[1;35m");
    fprintf(stderr, "\nOn Line %d: ", lineNumber);

    /* Reset the color*/
    fprintf(stderr, "\033[0m"); 
    fprintf(stderr, "%s\n\n", line);


}




/**
 * hasSomethingAfterSection - Check if a string contains additional characters after a section.
 * @param input - The input string to be checked.
 * @param section - The section to be checked for additional characters.
 * @return 1 if the string contains additional characters after the section, 0 otherwise.
 */

bool hasSomethingAfterSection(const char *input, const char *section) {
    /* Find the position of the section in the string */
    const char *sectionPos = strstr(input, section);

    if (sectionPos != NULL) {
        /* Calculate the length of the substring after the section */
        size_t sectionLength = strlen(section);
        size_t remainingLength = strlen(sectionPos + sectionLength);

        /* Check if there are additional characters after the section */
        if(remainingLength > 0) {
            return True;
        }
    }

    /* Section not found in the string */
    return False;
}

/**
 * isValidInteger - Check if a given string represents a valid integer.
 * @param str - The string to be checked.
 * @return True if valid, False otherwise.
 */
bool isValidInteger(const char *str) {
    if (str == NULL || *str == '\0') {
        /* Empty string is not a valid integer */
        return False;
    }

    /* Skip leading whitespaces */
    while (isspace(*str)) {
        str++;
    }

    /* Check for optional sign */
    if (*str == '+' || *str == '-') {
        str++;
    }

    /* Check if the remaining characters are digits */
    while (isdigit(*str)) {
        str++;
    }

    /* Check if the string has been fully processed (no trailing characters) */
    if(*str == '\0'){
        return True;
    } else {
        return False;
    }
}


/**
 * checkCommas - Check for syntax errors in the directive line.
 * @param line - The input string to be checked.
 * @param originalLine - The original input string.
 * @return True if the line is valid, False otherwise.
 */

bool checkCommas(const char* line, const char* originalLine, int lineNumber, char * fileName) {
    int consecutiveCommas = 0;

    /* Check if the line begins with a comma */
    if (*line == ',') {
        print_error("Directive begins with a comma.\n", originalLine, lineNumber, fileName);
        return False;
    }

    /* Iterate through the characters in the line */
    while (*line) {
        /* Check for consecutive commas */
        if (*line == ',') {
            consecutiveCommas++;

            /* Check if there are more than one consecutive commas */
            if (consecutiveCommas > 1) {
                print_error("More than one consecutive comma found.\n", originalLine, lineNumber, fileName);
                return False;
            }
        } else {
            consecutiveCommas = 0;
        }

        /* Move to the next character */
        line++;
    }

    /* Check if the line ends with a comma */
    if (*(line - 1) == ',') {
        print_error("Directive ends with a comma.\n", originalLine, lineNumber, fileName);
        return False;
    }

    return True;
}

/**
 * checkQuotes - Check if the string starts and ends with double-quotes.
 * @param line - The input string to be checked.
 * @param originalLine - The original input string.
 * @return True if the string starts and ends with double-quotes, False otherwise.
 */

bool checkQuotes(const char* line, const char* originalLine, int lineNumber, char * fileName) {
    /* Check if the line starts with a double-quote */
    if (*line == '"') {
        line++;

        /* Iterate through the characters in the line */
        while (*line) {
            line++;

            /* Check if the line ends with a double-quote */
            if (*line == '"') {
                return True;  /* String starts and ends with double-quotes */
            }
        }
    }

    /* If we reach here, it means the string is missing opening or closing double-quotes */
    print_error("String must start and end with double-quotes. Pay attention it must be regular quotes (\")\n", originalLine, lineNumber, fileName);
    return False;
}

/**
 * checkLabelLength - Check if a label has more than 31 characters.
 * @param label - The label to be checked.
 * @return True if the label is valid, False otherwise.
 */

bool checkLabelLength(const char* label) {
    size_t length = strlen(label);

    if (length > MAX_LABEL_LENGTH) {
        return False;
    }

    return True;
}


void check_constant_error(char *line, int lineNumber, char * fileName, HashTable *symbolsLabelsValuesHash) {

    char **constantSplitedLine;                    /* Array to store the constat splited line */
    int constantNumberOfElements = 0;              /* Reset the elemnts number - for the constat string spliter counter */

    /* copy line to side variable */
    char* originalLine = malloc(strlen(line) + 1);

    if (originalLine == NULL) {
        perror(MEMORY_ALLOCATION_ERROR);
        exit(EXIT_FAILURE);
    }

    strcpy(originalLine, line);

    if (hasSomethingAfterSection(line, ".define") == False) {
        print_error(".define didn't get any definition (the part after the define is empty)\n", originalLine, lineNumber, fileName);
        return;
    }

    if (hasLabel(line)) {
        print_error(".define instruction cannot have a lablel before it\n", originalLine, lineNumber, fileName);
        return;
    }

    /* print line */
    printf("Line before removal: %s\n", line);
    removeSubstring(line, ".define");
    printf("Line after removal: %s\n", line);

    removeWhiteSpaces(line);
    constantSplitedLine = splitString(line, "=", &constantNumberOfElements);
    printStringArray(constantSplitedLine, constantNumberOfElements);
    if(constantNumberOfElements != 2){
        print_error("Invalid constant definition\n", originalLine, lineNumber, fileName);
        return;
    }

    if(ht_search(symbolsLabelsValuesHash, constantSplitedLine[0]) != NULL){
        print_error("Constant is already defined\n", originalLine, lineNumber, fileName);
        return;
    }

    if(isValidInteger(constantSplitedLine[1]) == False){
        print_error("Invalid constant definition - number is not a valid integer\n", originalLine, lineNumber, fileName);
        return;
    }

    ht_insert(symbolsLabelsValuesHash, constantSplitedLine[0], constantSplitedLine[1]);

    /* Free the memory allocated for the splitedLines */
    freeStringArray(constantSplitedLine, constantNumberOfElements);
    free(originalLine);

    return;
}

/**
 * check_data_directive_error - Check for syntax errors in the .data directive.
 * @param line - The input string to be checked.
 * @param symbolsLabelsValuesHash - The hash table to store the symbols-labels values.
 * @return True if the line is valid, False otherwise.
 */

void check_data_directive_error(char * line, int lineNumber, char * fileName,  HashTable *symbolsLabelsValuesHash){

    char **splitedLine;                    /* Array to store the splited line */
    int numberOfElements = 0;              /* Reset the elemnts number - for the string spliter counter */
    int i = 0;                             /* Loop counter */


    /* copy line to side variable */
    char* originalLine = malloc(strlen(line) + 1);

    char labelName[MAX_LABEL_LENGTH];
    char value[MAX_LINE_LENGTH];
    char variableValue[MAX_LINE_LENGTH] = "";

    if (originalLine == NULL) {
        perror(MEMORY_ALLOCATION_ERROR);
        exit(EXIT_FAILURE);
    }

    strcpy(originalLine, line);

    /* Skip leading whitespaces */
    while (*line && (*line == ' ' || *line == '\t')) {
        line++;
    }

    if (hasSomethingAfterSection(line, ".data") == False) {
        print_error(".data directive didn't get any arguments\n", line, lineNumber, fileName);
        return;
    }

    /* print line */
    printf("Line before removal: %s\n", line);
    removeSubstring(line, ".data");
    printf("Line after removal: %s\n", line);


    if(hasLabel(line)){

        splitedLine = splitString(line, ":", &numberOfElements);
        printStringArray(splitedLine, numberOfElements);

        if(ht_search(symbolsLabelsValuesHash, splitedLine[0]) != NULL){
            print_error("Directive is already defined\n", originalLine, lineNumber, fileName);
            return;
        }

        strcpy(labelName, splitedLine[0]);

        if(checkLabelLength(labelName) == False){
            print_error("Label name is too long\n", originalLine, lineNumber, fileName);
            return;
        }

        strcpy(value, splitedLine[1]);
    }

    numberOfElements = 0;

    strcat(labelName, ":");
    removeSubstring(line, labelName);
    removeSubstring(labelName, ":");

    removeWhiteSpaces(line);
    if(checkCommas(line, originalLine, lineNumber, fileName) == False){
        return;
    }

    splitedLine = splitString(line, ",", &numberOfElements);
    printStringArray(splitedLine, numberOfElements);

    for (i = 0; i < numberOfElements; i++) {

        if(isValidInteger(splitedLine[i]) == False && ht_search(symbolsLabelsValuesHash, splitedLine[i]) == NULL){
            print_error("Invalid data definition - One of the numbers is not a valid integer or a defined variable\n", originalLine, lineNumber, fileName);
            return;
        }

        else{
            /* Add the string to value */
            if(ht_search(symbolsLabelsValuesHash, splitedLine[i]) != NULL){
                strcat(variableValue, ht_search(symbolsLabelsValuesHash, splitedLine[i]));
                strcat(variableValue, ",");
            }
            else{
                strcat(variableValue, splitedLine[i]);
                strcat(variableValue, ",");
            }

        }

    }
    removeLastCharacter(variableValue);
    printf("\noriginalLine: %s\n", originalLine);
    printf("value: %s\n", variableValue);
    printf("labelName: %s\n\n", labelName);

    
    if(hasLabel(originalLine)){
        ht_insert(symbolsLabelsValuesHash, labelName, variableValue);
    }
    

    freeStringArray(splitedLine, numberOfElements);
    free(originalLine);

    return;


}


/**
 * check_string_directive_error - Check for syntax errors in the .string directive.
 * @param line - The input string to be checked.
 * @param symbolsLabelsValuesHash - The hash table to store the symbols-labels values.
 * @return True if the line is valid, False otherwise.
 */

void check_string_directive_error(char * line, int lineNumber, char * fileName, HashTable *symbolsLabelsValuesHash){

    char **splitedLine;                    /* Array to store the splited line */
    int numberOfElements = 0;              /* Reset the elemnts number - for the string spliter counter */

    /* copy line to side variable */
    char* originalLine = malloc(strlen(line) + 1);

    char labelName[MAX_LABEL_LENGTH];
    char value[MAX_LINE_LENGTH];

    if (originalLine == NULL) {
        perror(MEMORY_ALLOCATION_ERROR);
        exit(EXIT_FAILURE);
    }

    strcpy(originalLine, line);

    /* Skip leading whitespaces */
    while (*line && (*line == ' ' || *line == '\t')) {
        line++;
    }

    if (hasSomethingAfterSection(line, ".string") == False) {
        print_error(".string directive didn't get any arguments\n", line, lineNumber, fileName);
        return;
    }

    /* print line */
    printf("Line before removal: %s\n", line);
    removeSubstring(line, ".string");
    printf("Line after removal: %s\n", line);


    if(hasLabel(line)){

        splitedLine = splitString(line, ":", &numberOfElements);
        printStringArray(splitedLine, numberOfElements);

        if(ht_search(symbolsLabelsValuesHash, splitedLine[0]) != NULL){
            print_error("Directive is already defined\n", originalLine, lineNumber, fileName);
            return;
        }

        strcpy(labelName, splitedLine[0]);

        if(checkLabelLength(labelName) == False){
            print_error("Label name is too long\n", originalLine, lineNumber, fileName);
            return;
        }
        
        strcpy(value, splitedLine[1]);
    }

    numberOfElements = 0;

    strcat(labelName, ":");
    removeSubstring(line, labelName);
    removeSubstring(labelName, ":");

    removeWhiteSpaces(line);
    if(checkQuotes(line, originalLine, lineNumber, fileName) == False){
        return;
    }

    splitedLine = splitString(line, ",", &numberOfElements);
    printStringArray(splitedLine, numberOfElements);

    if(hasLabel(originalLine)){
        removeLeadingSpaces(value);
        ht_insert(symbolsLabelsValuesHash, labelName, value);
    }

    freeStringArray(splitedLine, numberOfElements);
    free(originalLine);

    return;


}