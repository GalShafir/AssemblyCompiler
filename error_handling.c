#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "global_definitions.h"
#include "utility_functions.h"
#include "identification.h"
#include "error_handling.h"

void check_errors(CommandType commandType, char *line, int lineNumber, char * fileName, HashTable *symbolsLabelsValuesHash, int * directiveOrder, HashTable *entriesExternsHash, bool * foundError) {
    switch (commandType) {
        case EMPTY:
            break;
        case COMMENT:
            break;
        case DATA_DIRECTIVE:
            check_data_directive_error(line, lineNumber, fileName, symbolsLabelsValuesHash, directiveOrder, foundError);
            break;
        case STRING_DIRECTIVE:
            check_string_directive_error(line, lineNumber, fileName, symbolsLabelsValuesHash, directiveOrder, foundError);
            break;
        case ENTRY_DIRECTIVE:
            break;
        case EXTERN_DIRECTIVE:
            break;
        case UNDEFINED_DIRECTIVE:
            break;
        case INSTRUCTION:
            temp_instruction_addition(line, lineNumber, fileName, entriesExternsHash, symbolsLabelsValuesHash, foundError);
            break;
        case CONSTANT:
            check_constant_error(line, lineNumber, fileName, symbolsLabelsValuesHash, foundError);
            break;
        case UNDEFINED:
            print_error("undefined instruction\n", line, lineNumber, fileName);
            *foundError = True;
            break;
    }
}

void check_entries_externs_errors(CommandType commandType, char *line, int lineNumber, char * fileName, HashTable *entriesExternsHash, HashTable *symbolsLabelsValuesHash, bool * foundError) {

    switch (commandType) {
        case ENTRY_DIRECTIVE:
            check_entry_directive_error(line, lineNumber, fileName, entriesExternsHash, symbolsLabelsValuesHash, foundError);
            break;
        case EXTERN_DIRECTIVE:
            check_extern_directive_error(line, lineNumber, fileName, entriesExternsHash, foundError);
            break;
        default:
            break;
    }
}

void check_instruction_errors(CommandType commandType, char *line, int lineNumber, char * fileName, HashTable *entriesExternsHash, HashTable *symbolsLabelsValuesHash, bool * foundError){
    
        switch (commandType) {
            case INSTRUCTION:
                check_instruction_error(line, lineNumber, fileName, entriesExternsHash, symbolsLabelsValuesHash, foundError);
                break;
            case UNDEFINED:
                print_error("undefined instruction\n", line, lineNumber, fileName);
                *foundError = True;
                break;
            default:
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

/**
 * checkLabelName - Check if a label is not one of the reserved words / instructions.
 * @param label - The label to be checked.
 * @return True if the label is valid, False otherwise.
 */
bool checkLabelName(const char* label) {

    /* Compare the label with each reserved word / instruction */
    if (strcmp(label, "mov")   == 0 || strcmp(label, "cmp")    == 0 || strcmp(label, "add")    == 0 ||
        strcmp(label, "sub")   == 0 || strcmp(label, "lea")    == 0 || strcmp(label, "not")    == 0 ||
        strcmp(label, "clr")   == 0 || strcmp(label, "inc")    == 0 || strcmp(label, "dec")    == 0 ||
        strcmp(label, "jmp")   == 0 || strcmp(label, "bne")    == 0 || strcmp(label, "red")    == 0 ||
        strcmp(label, "prn")   == 0 || strcmp(label, "jsr")    == 0 || strcmp(label, "rts")    == 0 ||
        strcmp(label, "hlt")   == 0 || strcmp(label, "data")   == 0 || strcmp(label, "string") == 0 ||
        strcmp(label, "entry") == 0 || strcmp(label, "extern") == 0 || strcmp(label, "define") == 0 ||
        strcmp(label, "r0")    == 0 || strcmp(label, "r1")     == 0 || strcmp(label, "r2")     == 0 ||
        strcmp(label, "r3")    == 0 || strcmp(label, "r4")     == 0 || strcmp(label, "r5")     == 0 ||
        strcmp(label, "r6")    == 0 || strcmp(label, "r7")     == 0) {

            return False; /* Label is a reserved word / instruction */

    } else {

            return True; /* Label is valid */
    }
}


void check_constant_error(char *line, int lineNumber, char * fileName, HashTable *symbolsLabelsValuesHash, bool * foundError) {

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
        *foundError = True;
        return;
    }

    if (hasLabel(line)) {
        print_error(".define instruction cannot have a lablel before it\n", originalLine, lineNumber, fileName);
        *foundError = True;
        return;
    }

    /* print line */
    removeSubstring(line, ".define");

    removeWhiteSpaces(line);
    constantSplitedLine = splitString(line, "=", &constantNumberOfElements);
    if(constantNumberOfElements != 2){
        print_error("Invalid constant definition\n", originalLine, lineNumber, fileName);
        *foundError = True;
        return;
    }

    if(ht_search(symbolsLabelsValuesHash, constantSplitedLine[0]) != NULL){
        print_error("Constant is already defined\n", originalLine, lineNumber, fileName);
        *foundError = True;
        return;
    }

    if(isValidInteger(constantSplitedLine[1]) == False){
        print_error("Invalid constant definition - number is not a valid integer\n", originalLine, lineNumber, fileName);
        *foundError = True;
        return;
    }

    ht_insert(symbolsLabelsValuesHash, constantSplitedLine[0], constantSplitedLine[1], "constant", "0", "0", "-1");

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

void check_data_directive_error(char * line, int lineNumber, char * fileName,  HashTable *symbolsLabelsValuesHash, int * directiveOrder, bool * foundError){

    char **splitedLine;                    /* Array to store the splited line */
    int numberOfElements = 0;              /* Reset the elemnts number - for the string spliter counter */
    int i = 0;                             /* Loop counter */
    char * directiveOrderString = NULL;    /* String to store the directive order */
    int memorySize;                        /* The size of the memory to be allocated */
    char * memorySizeString = NULL;        /* String to store the memory size */

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
        *foundError = True;
        return;
    }

    /* print line */
    removeSubstring(line, ".data");


    if(hasLabel(line)){

        splitedLine = splitString(line, ":", &numberOfElements);

        if(ht_search(symbolsLabelsValuesHash, splitedLine[0]) != NULL){
            print_error("Directive is already defined\n", originalLine, lineNumber, fileName);
            *foundError = True;
            return;
        }

        strcpy(labelName, splitedLine[0]);

        if(checkLabelLength(labelName) == False){
            print_error("Label name is too long\n", originalLine, lineNumber, fileName);
            *foundError = True;
            return;
        }

        if(checkLabelName(labelName) == False){
            print_error("Label name is a reserved word\n", originalLine, lineNumber, fileName);
            *foundError = True;
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

    for (i = 0; i < numberOfElements; i++) {

        if(isValidInteger(splitedLine[i]) == False && ht_search(symbolsLabelsValuesHash, splitedLine[i]) == NULL){
            print_error("Invalid data definition - One of the numbers is not a valid integer or a defined variable\n", originalLine, lineNumber, fileName);
            *foundError = True;
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

    
    if(hasLabel(originalLine)){
        directiveOrderString = intToString(*directiveOrder);
        memorySize = numberOfElements;
        memorySizeString = intToString(memorySize);
        ht_insert(symbolsLabelsValuesHash, labelName, variableValue, "dataDirective", "0", memorySizeString, directiveOrderString);
        (*directiveOrder)++;
        free(directiveOrderString);
        free(memorySizeString);
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

void check_string_directive_error(char * line, int lineNumber, char * fileName, HashTable *symbolsLabelsValuesHash, int * directiveOrder, bool * foundError){

    char **splitedLine;                    /* Array to store the splited line */
    int numberOfElements = 0;              /* Reset the elemnts number - for the string spliter counter */
    char * directiveOrderString = NULL;    /* String to store the directive order */
    int memorySize;                        /* The size of the memory to be allocated */
    char * memorySizeString = NULL;        /* String to store the memory size */


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
        *foundError = True;
        return;
    }

    /* print line */
    removeSubstring(line, ".string");


    if(hasLabel(line)){

        splitedLine = splitString(line, ":", &numberOfElements);

        if(ht_search(symbolsLabelsValuesHash, splitedLine[0]) != NULL){
            print_error("Directive is already defined\n", originalLine, lineNumber, fileName);
            *foundError = True;
            return;
        }

        strcpy(labelName, splitedLine[0]);

        if(checkLabelLength(labelName) == False){
            print_error("Label name is too long\n", originalLine, lineNumber, fileName);
            *foundError = True;
            return;
        }

        if(checkLabelName(labelName) == False){
            print_error("Label name is a reserved word\n", originalLine, lineNumber, fileName);
            *foundError = True;
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

    if(hasLabel(originalLine)){
        removeLeadingSpaces(value);
        directiveOrderString = intToString(*directiveOrder);
        /* Calculate the string size - both of the quotes */
        memorySize = countCharacters(value) - 2;

        memorySizeString = intToString(memorySize);
        ht_insert(symbolsLabelsValuesHash, labelName, value, "stringDirective", "0", memorySizeString, directiveOrderString);
        (*directiveOrder)++;
        free(directiveOrderString);
        free(memorySizeString);

    }

    freeStringArray(splitedLine, numberOfElements);
    free(originalLine);

    return;


}


void check_entry_directive_error(char * line, int lineNumber, char * fileName, HashTable *entriesExternsHash, HashTable *symbolsLabelsValuesHash, bool * foundError){

    char **splitedLine;                    /* Array to store the splited line */
    int numberOfElements = 0;              /* Reset the elemnts number - for the string spliter counter */

    /* copy line to side variable */
    char* originalLine = malloc(strlen(line) + 1);

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

    if (hasSomethingAfterSection(line, ".entry") == False) {
        print_error(".entry directive didn't get any arguments\n", originalLine, lineNumber, fileName);
        *foundError = True;
        return;
    }

    /* print line */
    removeSubstring(line, ".entry");

    if(hasLabel(line)){

        splitedLine = splitString(line, ":", &numberOfElements);

        /* A warning can be inserted here */

        strcpy(value, splitedLine[1]);
        freeStringArray(splitedLine, numberOfElements);

    }

    else {

        strcpy(value, line);
    }

    if(hasLabel(originalLine)){
        removeLeadingSpaces(value);
    }

    removeWhiteSpaces(value);

    if(ht_search(entriesExternsHash, value) != NULL && strcmp(ht_get_type(entriesExternsHash, value), "externDirective") == 0){
        print_error("Entry is already defined as extern directive\n", originalLine, lineNumber, fileName);
        *foundError = True;
        return;
    }

    if(ht_search(symbolsLabelsValuesHash, value) == NULL){
        print_error("The label the entry is pointing on is not defined in the file\n", originalLine, lineNumber, fileName);
        *foundError = True;
        return;
    }

    if(ht_search(entriesExternsHash, value) == NULL){
        ht_insert(entriesExternsHash, value, "0", "entryDirective", "0", "0", "0");
    }

    free(originalLine);

    return;
}

void check_extern_directive_error(char * line, int lineNumber, char * fileName, HashTable *entriesExternsHash, bool * foundError){

    char **splitedLine;                    /* Array to store the splited line */
    int numberOfElements = 0;              /* Reset the elemnts number - for the string spliter counter */

    /* copy line to side variable */
    char* originalLine = malloc(strlen(line) + 1);

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

    if (hasSomethingAfterSection(line, ".extern") == False) {
        print_error(".extern directive didn't get any arguments\n", originalLine, lineNumber, fileName);
        *foundError = True;
        return;
    }

    /* print line */
    removeSubstring(line, ".extern");

    if(hasLabel(line)){

        splitedLine = splitString(line, ":", &numberOfElements);

        /* A warning can be inserted here */

        strcpy(value, splitedLine[1]);
        freeStringArray(splitedLine, numberOfElements);

    }

    else{

        strcpy(value, line);
    }

    removeWhiteSpaces(value);

    if(hasLabel(originalLine)){
        removeLeadingSpaces(value);
    }

    if(ht_search(entriesExternsHash, value) != NULL && strcmp(ht_get_type(entriesExternsHash, value), "entryDirective") == 0){
        print_error("Extern is already defined as entry directive\n", originalLine, lineNumber, fileName);
        *foundError = True;
        return;
    }

    if(ht_search(entriesExternsHash, value) == NULL){
        ht_insert(entriesExternsHash, value, "0", "externDirective", "0", "0", "0");
    }

    free(originalLine);

    return;


}

void temp_instruction_addition(char * line, int lineNumber, char * fileName, HashTable *entriesExternsHash, HashTable *symbolsLabelsValuesHash, bool * foundError){
    char **splitedLine;                    /* Array to store the splited line */
    int numberOfElements = 0;              /* Reset the elemnts number - for the string spliter counter */

    /* copy line to side variable */
    char* originalLine = malloc(strlen(line) + 1);

    char labelName[MAX_LABEL_LENGTH];

    if (originalLine == NULL) {
        perror(MEMORY_ALLOCATION_ERROR);
        exit(EXIT_FAILURE);
    }

    strcpy(originalLine, line);

    /* Skip leading whitespaces */
    while (*line && (*line == ' ' || *line == '\t')) {
        line++;
    }

    if(hasLabel(line)){

        splitedLine = splitString(line, ":", &numberOfElements);

        if(ht_search(symbolsLabelsValuesHash, splitedLine[0]) != NULL){
            print_error("Label is already defined\n", originalLine, lineNumber, fileName);
            *foundError = True;
            freeStringArray(splitedLine, numberOfElements);
            return;
        }

        if(ht_search(entriesExternsHash, splitedLine[0]) != NULL){
            print_error("Label is already defined as entry or extern directive\n", originalLine, lineNumber, fileName);
            *foundError = True;
            freeStringArray(splitedLine, numberOfElements);
            return;
        }

        strcpy(labelName, splitedLine[0]);

        if(checkLabelLength(labelName) == False){
            print_error("Label name is too long\n", originalLine, lineNumber, fileName);
            *foundError = True;
            freeStringArray(splitedLine, numberOfElements);
            return;
        }

        if(checkLabelName(labelName) == False){
            print_error("Label name is a reserved word\n", originalLine, lineNumber, fileName);
            *foundError = True;
            freeStringArray(splitedLine, numberOfElements);
            return;
        }

        freeStringArray(splitedLine, numberOfElements);
        
    }

    strcat(labelName, ":");
    removeSubstring(line, labelName);
    removeSubstring(labelName, ":");

    if(hasLabel(originalLine)){
        ht_insert(symbolsLabelsValuesHash, labelName, "NULL", "instruction", "0", "0", "-1");
    }

    free(originalLine);

    return;

}

void check_instruction_error(char * line, int lineNumber, char * fileName, HashTable *entriesExternsHash, HashTable *symbolsLabelsValuesHash, bool * foundError){

    char **splitedLine;                    /* Array to store the splited line */
    int numberOfElements = 0;              /* Reset the elemnts number - for the string spliter counter */
    AddressingMode operand1AddressingMode;
    AddressingMode operand2AddressingMode;
    /* copy line to side variable */
    char* originalLine = malloc(strlen(line) + 1);

    char labelName[MAX_LABEL_LENGTH];
    char instruction[4];

    if (originalLine == NULL) {
        perror(MEMORY_ALLOCATION_ERROR);
        exit(EXIT_FAILURE);
    }

    strcpy(originalLine, line);

    /* Skip leading whitespaces */
    while (*line && (*line == ' ' || *line == '\t')) {
        line++;
    }

    if(hasLabel(line)){

        splitedLine = splitString(line, ":", &numberOfElements);

        if(ht_search(symbolsLabelsValuesHash, splitedLine[0]) != NULL && strcmp(ht_get_type(symbolsLabelsValuesHash, splitedLine[0]), "instruction") == 0){
            ht_delete(symbolsLabelsValuesHash, splitedLine[0]);
        }
        if(ht_search(symbolsLabelsValuesHash, splitedLine[0]) != NULL){
            print_error("Label is already defined\n", originalLine, lineNumber, fileName);
            *foundError = True;
            freeStringArray(splitedLine, numberOfElements);
            return;
        }

        if(ht_search(entriesExternsHash, splitedLine[0]) != NULL && strcmp(ht_get_type(entriesExternsHash, splitedLine[0]), "externDirective") == 0){
            print_error("Label is already defined as an extern directive\n", originalLine, lineNumber, fileName);
            *foundError = True;
            freeStringArray(splitedLine, numberOfElements);
            return;
        }

        strcpy(labelName, splitedLine[0]);

        if(checkLabelLength(labelName) == False){
            print_error("Label name is too long\n", originalLine, lineNumber, fileName);
            *foundError = True;
            freeStringArray(splitedLine, numberOfElements);
            return;
        }

        if(checkLabelName(labelName) == False){
            print_error("Label name is a reserved word\n", originalLine, lineNumber, fileName);
            *foundError = True;
            freeStringArray(splitedLine, numberOfElements);
            return;
        }

        freeStringArray(splitedLine, numberOfElements);
        
    }

    strcat(labelName, ":");
    removeSubstring(line, labelName);
    removeSubstring(labelName, ":");

    numberOfElements = 0;
    removeLeadingSpaces(line);

    splitedLine = splitString(line, " ", &numberOfElements);
    strcpy(instruction, splitedLine[0]);

    freeStringArray(splitedLine, numberOfElements);
    removeWhiteSpaces(instruction);

    if (number_of_operands_is_valid(instruction, line) == False) {
        print_error("Invalid number of operands\n", originalLine, lineNumber, fileName);
        *foundError = True;
        return;
    }

    if(checkInstructionCommas(line, originalLine, lineNumber, fileName) == False){
        print_error("Invalid commas\n", originalLine, lineNumber, fileName);
        *foundError = True;
        return;
    }

    if (strcmp(instruction, "mov") == 0 || strcmp(instruction, "cmp") == 0 || strcmp(instruction, "add") == 0 ||
        strcmp(instruction, "sub") == 0 || strcmp(instruction, "lea") == 0) {


            removeLeadingSpaces(line);
            numberOfElements = 0;
            splitedLine = splitString(line, ",", &numberOfElements);

            operand1AddressingMode = identifyAddressingMode(splitedLine[0], symbolsLabelsValuesHash, entriesExternsHash);
            operand2AddressingMode = identifyAddressingMode(splitedLine[1], symbolsLabelsValuesHash, entriesExternsHash);

            freeStringArray(splitedLine, numberOfElements);

            if(operand1AddressingMode == UNDEFINED_ADDRESSING){
                print_error("Unkown addressing mode for the source operand\n", originalLine, lineNumber, fileName);
                *foundError = True;
                return;
            }

            if(operand2AddressingMode == UNDEFINED_ADDRESSING){
                print_error("Unkown addressing mode for the destination operand\n", originalLine, lineNumber, fileName);
                *foundError = True;
                return;
            }

            if(operand1AddressingMode == UNDEFINED_CONSTANT){
                print_error("The constant that has been used for the source operand is undifined\n", originalLine, lineNumber, fileName);
                *foundError = True;
                return;
            }

            if(operand2AddressingMode == UNDEFINED_CONSTANT){
                print_error("The constant that has been used for the destination operand is undifined\n", originalLine, lineNumber, fileName);
                *foundError = True;
                return;
            }

            if(operand1AddressingMode == UNDEFINED_LABEL){
                print_error("Label does not exists for the source operand, also make sure it as a data or a string directive\n", originalLine, lineNumber, fileName);
                *foundError = True;
                return;
            }

            if(operand2AddressingMode == UNDEFINED_LABEL){
                print_error("Label does not exists for the destination operand, also make sure it as a data or a string directive\n", originalLine, lineNumber, fileName);
                *foundError = True;
                return;
            }

            if(operand1AddressingMode == INDEX_OVERFLOW){
                print_error("Index overflow for the source operand\n", originalLine, lineNumber, fileName);
                *foundError = True;
                return;
            }

            if(operand2AddressingMode == INDEX_OVERFLOW){
                print_error("Index overflow for the destination operand\n", originalLine, lineNumber, fileName);
                *foundError = True;
                return;
            }

            /* mov command*/
            if(strcmp(instruction, "mov") == 0){
                if(operand1AddressingMode != IMMEDIATE && operand1AddressingMode != DIRECT && operand1AddressingMode != INDEX && operand1AddressingMode != REGISTER){
                    print_error("Invalid addressing mode for the source operand\n", originalLine, lineNumber, fileName);
                    *foundError = True;
                    return;
                }

                if(operand2AddressingMode != DIRECT && operand2AddressingMode != INDEX && operand2AddressingMode != REGISTER){
                    print_error("Invalid addressing mode for the destination operand\n", originalLine, lineNumber, fileName);
                    *foundError = True;
                    return;
                }

            }

            /* cmp command*/
            else if(strcmp(instruction, "cmp") == 0){
                if(operand1AddressingMode != IMMEDIATE && operand1AddressingMode != DIRECT && operand1AddressingMode != INDEX && operand1AddressingMode != REGISTER){
                    print_error("Invalid addressing mode for the source operand\n", originalLine, lineNumber, fileName);
                    *foundError = True;
                    return;
                }

                if(operand2AddressingMode != IMMEDIATE && operand2AddressingMode != DIRECT && operand2AddressingMode != INDEX && operand2AddressingMode != REGISTER){
                    print_error("Invalid addressing mode for the destination operand\n", originalLine, lineNumber, fileName);
                    *foundError = True;
                    return;
                }

            }

            /* add command*/
            else if(strcmp(instruction, "add") == 0){
                if(operand1AddressingMode != IMMEDIATE && operand1AddressingMode != DIRECT && operand1AddressingMode != INDEX && operand1AddressingMode != REGISTER){
                    print_error("Invalid addressing mode for the source operand\n", originalLine, lineNumber, fileName);
                    *foundError = True;
                    return;
                }

                if(operand2AddressingMode != DIRECT && operand2AddressingMode != INDEX && operand2AddressingMode != REGISTER){
                    print_error("Invalid addressing mode for the destination operand\n", originalLine, lineNumber, fileName);
                    *foundError = True;
                    return;
                }

            }

            /* sub command*/
            else if(strcmp(instruction, "sub") == 0){
                if(operand1AddressingMode != IMMEDIATE && operand1AddressingMode != DIRECT && operand1AddressingMode != INDEX && operand1AddressingMode != REGISTER){
                    print_error("Invalid addressing mode for the source operand\n", originalLine, lineNumber, fileName);
                    *foundError = True;
                    return;
                }

                if(operand2AddressingMode != DIRECT && operand2AddressingMode != INDEX && operand2AddressingMode != REGISTER){
                    print_error("Invalid addressing mode for the destination operand\n", originalLine, lineNumber, fileName);
                    *foundError = True;
                    return;
                }

            }

            /* lea command*/
            else if(strcmp(instruction, "lea") == 0){
                if(operand1AddressingMode != DIRECT && operand1AddressingMode != INDEX){
                    print_error("Invalid addressing mode for the source operand\n", originalLine, lineNumber, fileName);
                    *foundError = True;
                    return;
                }

                if(operand2AddressingMode != DIRECT && operand2AddressingMode != INDEX && operand2AddressingMode != REGISTER){
                    print_error("Invalid addressing mode for the destination operand\n", originalLine, lineNumber, fileName);
                    *foundError = True;
                    return;
                }

            }

    }

    else if (strcmp(instruction, "not") == 0 || strcmp(instruction, "clr") == 0 || strcmp(instruction, "inc") == 0 ||
               strcmp(instruction, "dec") == 0 || strcmp(instruction, "jmp") == 0 || strcmp(instruction, "bne") == 0 ||
               strcmp(instruction, "red") == 0 || strcmp(instruction, "prn") == 0 || strcmp(instruction, "jsr") == 0) {


        removeLeadingSpaces(line);
        numberOfElements = 0;
        splitedLine = splitString(line, " ", &numberOfElements);
        operand1AddressingMode = identifyAddressingMode(splitedLine[0], symbolsLabelsValuesHash, entriesExternsHash);

        freeStringArray(splitedLine, numberOfElements);

        if(operand1AddressingMode == UNDEFINED_ADDRESSING){
            print_error("Unkown addressing mode for the destination operand\n", originalLine, lineNumber, fileName);
            *foundError = True;
            return;
        }

        if(operand1AddressingMode == UNDEFINED_CONSTANT){
            print_error("The constant that has been used for the destination operand is undifined\n", originalLine, lineNumber, fileName);
            *foundError = True;
            return;
        }

        if(operand1AddressingMode == UNDEFINED_LABEL){
            print_error("Label does not exists for the destination operand, also make sure it as a data or a string directive\n", originalLine, lineNumber, fileName);
            *foundError = True;
            return;
        }

        if(operand1AddressingMode == INDEX_OVERFLOW){
            print_error("Index overflow for the destination operand\n", originalLine, lineNumber, fileName);
            *foundError = True;
            return;
        }

        /* not command*/
        if(strcmp(instruction, "not") == 0){
            if(operand1AddressingMode != DIRECT && operand1AddressingMode != INDEX && operand1AddressingMode != REGISTER){
                print_error("Invalid addressing mode for the destination operand\n", originalLine, lineNumber, fileName);
                *foundError = True;
                return;
            }

        }

        /* clr command*/
        else if(strcmp(instruction, "clr") == 0){
            if(operand1AddressingMode != DIRECT && operand1AddressingMode != INDEX && operand1AddressingMode != REGISTER){
                print_error("Invalid addressing mode for the destination operand\n", originalLine, lineNumber, fileName);
                *foundError = True;
                return;
            }

        }

        /* inc command*/
        else if(strcmp(instruction, "inc") == 0){
            if(operand1AddressingMode != DIRECT && operand1AddressingMode != INDEX && operand1AddressingMode != REGISTER){
                print_error("Invalid addressing mode for the destination operand\n", originalLine, lineNumber, fileName);
                *foundError = True;
                return;
            }

        }

        /* dec command*/
        else if(strcmp(instruction, "dec") == 0){
            if(operand1AddressingMode != DIRECT && operand1AddressingMode != INDEX && operand1AddressingMode != REGISTER){
                print_error("Invalid addressing mode for the destination operand\n", originalLine, lineNumber, fileName);
                *foundError = True;
                return;
            }

        }

        /* jmp command*/
        else if(strcmp(instruction, "jmp") == 0){
            if(operand1AddressingMode != DIRECT && operand1AddressingMode != REGISTER){
                print_error("Invalid addressing mode for the destination operand\n", originalLine, lineNumber, fileName);
                *foundError = True;
                return;
            }

        }

        /* bne command*/
        else if(strcmp(instruction, "bne") == 0){
            if(operand1AddressingMode != DIRECT && operand1AddressingMode != REGISTER){
                print_error("Invalid addressing mode for the destination operand\n", originalLine, lineNumber, fileName);
                *foundError = True;
                return;
            }

        }

        /* red command*/
        else if(strcmp(instruction, "red") == 0){
            if(operand1AddressingMode != DIRECT && operand1AddressingMode != INDEX && operand1AddressingMode != REGISTER){
                print_error("Invalid addressing mode for the destination operand\n", originalLine, lineNumber, fileName);
                *foundError = True;
                return;
            }

        }

        /* prn command*/
        else if(strcmp(instruction, "prn") == 0){
            if(operand1AddressingMode != IMMEDIATE && operand1AddressingMode != DIRECT && operand1AddressingMode != INDEX && operand1AddressingMode != REGISTER){
                print_error("Invalid addressing mode for the destination operand\n", originalLine, lineNumber, fileName);
                *foundError = True;
                return;
            }

        }

        /* jsr command*/
        else if(strcmp(instruction, "jsr") == 0){
            if(operand1AddressingMode != DIRECT && operand1AddressingMode != REGISTER){
                print_error("Invalid addressing mode for the destination operand\n", originalLine, lineNumber, fileName);
                *foundError = True;
                return;
            }

        }

    }

    else if (strcmp(instruction, "rts") == 0 || strcmp(instruction, "hlt") == 0) {
        removeLeadingSpaces(line);
        if (hasSomethingAfterSection(line, instruction) == True) {
            print_error("Invalid number of operands\n", originalLine, lineNumber, fileName);
            *foundError = True;
            return;
        }
    }



    if(hasLabel(originalLine)){
        ht_insert(symbolsLabelsValuesHash, labelName, "NULL", "instruction", "0", "0", "-1");
    }

    free(originalLine);

    return;

}

/**
 * Check if the number of operands is valid for the given instruction.
 *
 * @param instructionName - The name of the instruction.
 * @param instructionContent - The content of the instruction.
 * @return True if the number of operands is valid, False otherwise.
 */
bool number_of_operands_is_valid(char *instructionName, char *line) {

    char **splitedLine;                    /* Array to store the split line */
    int numberOfElements = 0;              /* Reset the elements number - for the string splitter counter */

    if (strcmp(instructionName, "rts") == 0 || strcmp(instructionName, "hlt") == 0) {
        if (hasSomethingAfterSection(line, instructionName) == 0) {
            return True;
        }
    }

    removeSubstring(line, instructionName);

    if (strcmp(instructionName, "mov") == 0 || strcmp(instructionName, "cmp") == 0 || strcmp(instructionName, "add") == 0 ||
        strcmp(instructionName, "sub") == 0 || strcmp(instructionName, "lea") == 0) {

        splitedLine = splitString(line, ",", &numberOfElements);

        if (numberOfElements == 2) {
            freeStringArray(splitedLine, numberOfElements);
            return True;
        }
        else{
            freeStringArray(splitedLine, numberOfElements);
            return False;
        }

    } else if (strcmp(instructionName, "not") == 0 || strcmp(instructionName, "clr") == 0 || strcmp(instructionName, "inc") == 0 ||
               strcmp(instructionName, "dec") == 0 || strcmp(instructionName, "jmp") == 0 || strcmp(instructionName, "bne") == 0 ||
               strcmp(instructionName, "red") == 0 || strcmp(instructionName, "prn") == 0 || strcmp(instructionName, "jsr") == 0) {

        splitedLine = splitString(line, " ", &numberOfElements);

        if (numberOfElements == 1) {
            freeStringArray(splitedLine, numberOfElements);
            return True;
        }

        else{
            freeStringArray(splitedLine, numberOfElements);
            return False;
        }

    }

    return False;
        
}

/**
 * checkInstructionCommas - Check for syntax errors in the instruction line.
 * @param line - The input string to be checked.
 * @param originalLine - The original input string.
 * @return True if the line is valid, False otherwise.
 */

bool checkInstructionCommas(const char* line, const char* originalLine, int lineNumber, char * fileName) {
    int consecutiveCommas = 0;

    /* Check if the line begins with a comma */
    if (*line == ',') {
        print_error("Instruction begins with a comma.\n", originalLine, lineNumber, fileName);
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
        print_error("Instruction ends with a comma.\n", originalLine, lineNumber, fileName);
        return False;
    }

    return True;
}



