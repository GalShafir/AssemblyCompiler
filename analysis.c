#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "global_definitions.h"
#include "utility_functions.h"
#include "identification.h"
#include "error_handling.h"
#include "analysis.h"



void calculate_memory_addresses_for_instructions(CommandType commandType, char *line, HashTable *symbolsLabelsValuesHash, int * directiveOrder, HashTable *entriesExternsHash, int * currentMemoryAddress) {
    switch (commandType) {
        case DATA_DIRECTIVE:
            break;
        case STRING_DIRECTIVE:
            break;
        case INSTRUCTION:
            calculate_instruction_memory_address(line, symbolsLabelsValuesHash, entriesExternsHash, currentMemoryAddress);
            break;
        default:
            break;

    }
}

void calculate_memory_addresses_for_directives(CommandType commandType, char *line, HashTable *symbolsLabelsValuesHash, int * directiveOrder, HashTable *entriesExternsHash, int * currentMemoryAddress) {
    switch (commandType) {
        case DATA_DIRECTIVE:
            calculate_data_directive_memory_address(line, symbolsLabelsValuesHash, directiveOrder, currentMemoryAddress);
            break;
        case STRING_DIRECTIVE:
            calculate_string_directive_memory_address(line, symbolsLabelsValuesHash, directiveOrder, currentMemoryAddress);
            break;
        case INSTRUCTION:
            break;
        default:
            break;

    }
}


void build_binary_file(char * inputFileName, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash, HashTable *instructionsHash){

    char line[MAX_LINE_LENGTH];             /* Buffer to store each line from the file */
    char outputFileName[MAX_LINE_LENGTH];   /* Buffer to store the output file name */

    FILE *inputFile = NULL;                 /* File pointer for the input file */
    FILE *outputFile = NULL;                /* File pointer for the output file */

    CommandType commandType;                /* Type of the command in the line */


    /* Open the input file */
    inputFile = openFile(inputFileName, "r");

    /* Open a new file with the same name but a ".am" extension for writing */
    sprintf(outputFileName, "%s.temp", removeFileExtension(inputFileName));
    outputFile = openFile(outputFileName, "w");

    while (fgets(line, sizeof(line), inputFile) != NULL) {
        
        /* Identify the command type */
        commandType = identifyCommandType(line, instructionsHash);
        switch (commandType) {
            case DATA_DIRECTIVE:
                analyze_data_directive(line, symbolsLabelsValuesHash, entriesExternsHash, instructionsHash, outputFile);
                break;
            case STRING_DIRECTIVE:
                analyze_string_directive(line, symbolsLabelsValuesHash, entriesExternsHash, instructionsHash, outputFile);
                break;
            case INSTRUCTION:
                break;
            default:
                break;

    }


    }



}

void decimalToBinary(int decimal, int numBits, char *binary) {
    int i; /* Loop counter */
    if (binary == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }


    /* Handle negative numbers using two's complement */
    if (decimal < 0) {
        decimal = (1 << numBits) + decimal; /* Convert to positive value in two's complement */
    }


    /* Convert decimal to binary */
    for (i = numBits - 1; i >= 0; i--) {
        binary[i] = (decimal & 1) ? '1' : '0'; /* Extract the least significant bit */
        decimal >>= 1; /* Shift right to get next bit */
    }
    binary[numBits] = '\0'; /* Null-terminate the string */

}


void calculate_data_directive_memory_address(char * line, HashTable *symbolsLabelsValuesHash, int * directiveOrder, int * currentMemoryAddress){

    char **splitedLine;                    /* Array to store the splited line */
    int numberOfElements = 0;              /* Reset the elemnts number - for the string spliter counter */
    int i = 0;                             /* Loop counter */
    char * directiveOrderString = NULL;    /* String to store the directive order */
    int memorySize;                        /* The size of the memory to be allocated */
    char * memorySizeString = NULL;        /* String to store the memory size */
    int memoryAddress;                     /* The memory address of the directive */
    char * memoryAddressString = NULL;     /* String to store the memory address */

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


    removeSubstring(line, ".data");


    if(hasLabel(line)){

        splitedLine = splitString(line, ":", &numberOfElements);
        printStringArray(splitedLine, numberOfElements);

        strcpy(labelName, splitedLine[0]);
        strcpy(value, splitedLine[1]);
    }

    numberOfElements = 0;

    strcat(labelName, ":");
    removeSubstring(line, labelName);
    removeSubstring(labelName, ":");

    removeWhiteSpaces(line);

    splitedLine = splitString(line, ",", &numberOfElements);
    printStringArray(splitedLine, numberOfElements);

    for (i = 0; i < numberOfElements; i++) {

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

    removeLastCharacter(variableValue);


    
    if(hasLabel(originalLine)){
        directiveOrderString = intToString(*directiveOrder);
        memorySize = numberOfElements;
        memorySizeString = intToString(memorySize);
        memoryAddress = *currentMemoryAddress;
        *currentMemoryAddress += memorySize;
        memoryAddressString = intToString(memoryAddress);
        ht_insert(symbolsLabelsValuesHash, labelName, variableValue, "dataDirective", memoryAddressString, memorySizeString, directiveOrderString);
        (*directiveOrder)++;
        free(directiveOrderString);
        free(memorySizeString);
        free(memoryAddressString);
    }
    

    freeStringArray(splitedLine, numberOfElements);
    free(originalLine);

    return;


}


void calculate_string_directive_memory_address(char * line, HashTable *symbolsLabelsValuesHash, int * directiveOrder, int * currentMemoryAddress){

    char **splitedLine;                    /* Array to store the splited line */
    int numberOfElements = 0;              /* Reset the elemnts number - for the string spliter counter */
    char * directiveOrderString = NULL;    /* String to store the directive order */
    int memorySize;                        /* The size of the memory to be allocated */
    char * memorySizeString = NULL;        /* String to store the memory size */
    int memoryAddress;                     /* The memory address of the directive */
    char * memoryAddressString = NULL;     /* String to store the memory address */

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

    removeSubstring(line, ".string");

    if(hasLabel(line)){

        splitedLine = splitString(line, ":", &numberOfElements);
        printStringArray(splitedLine, numberOfElements);



        strcpy(labelName, splitedLine[0]);
        strcpy(value, splitedLine[1]);
    }

    numberOfElements = 0;

    strcat(labelName, ":");
    removeSubstring(line, labelName);
    removeSubstring(labelName, ":");

    removeWhiteSpaces(line);

    if(hasLabel(originalLine)){

        removeLeadingSpaces(value);
        directiveOrderString = intToString(*directiveOrder);
        /* Calculate the string size - both of the quotes + /0 character */
        memorySize = countCharacters(value) - 1;
        memorySizeString = intToString(memorySize);
        memoryAddress = *currentMemoryAddress;
        *currentMemoryAddress += memorySize;
        memoryAddressString = intToString(memoryAddress);
        ht_insert(symbolsLabelsValuesHash, labelName, value, "stringDirective", memoryAddressString, memorySizeString, directiveOrderString);
        (*directiveOrder)++;
        free(directiveOrderString);
        free(memorySizeString);
        free(memoryAddressString);

    }

    freeStringArray(splitedLine, numberOfElements);
    free(originalLine);

    return;

}


void calculate_instruction_memory_address(char * line, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash, int * currentMemoryAddress){

    char **splitedLine;                    /* Array to store the splited line */
    int numberOfElements = 0;              /* Reset the elemnts number - for the string spliter counter */
    int memorySize = 1;                    /* The size of the memory to be allocated */
    char * memorySizeString = NULL;        /* String to store the memory size */
    int memoryAddress;                     /* The memory address of the directive */
    char * memoryAddressString = NULL;     /* String to store the memory address */

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
        printStringArray(splitedLine, numberOfElements);

        strcpy(labelName, splitedLine[0]);

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

    removeSubstring(line, instruction);

    if (strcmp(instruction, "mov") == 0 || strcmp(instruction, "cmp") == 0 || strcmp(instruction, "add") == 0 ||
        strcmp(instruction, "sub") == 0 || strcmp(instruction, "lea") == 0) {


            removeLeadingSpaces(line);
            numberOfElements = 0;
            splitedLine = splitString(line, ",", &numberOfElements);

            operand1AddressingMode = analyzeAddressingMode(splitedLine[0], symbolsLabelsValuesHash, entriesExternsHash);
            operand2AddressingMode = analyzeAddressingMode(splitedLine[1], symbolsLabelsValuesHash, entriesExternsHash);

            freeStringArray(splitedLine, numberOfElements);

            if(operand1AddressingMode == IMMEDIATE){
                memorySize += 1;
            }

            else if(operand1AddressingMode == DIRECT){
                memorySize += 1;
            }

            else if(operand1AddressingMode == INDEX){
                memorySize += 2;
            }

            else if(operand1AddressingMode == REGISTER){
                memorySize += 1;
            }

            if(operand2AddressingMode == IMMEDIATE){
                memorySize += 1;
            }

            else if(operand2AddressingMode == DIRECT){
                memorySize += 1;
            }

            else if(operand2AddressingMode == INDEX){
                memorySize += 2;
            }

            else if(operand2AddressingMode == REGISTER){
                memorySize += 1;
            }

            /* If both operands are registers, then the memory size should be decreased by 1 */
            if(operand1AddressingMode == REGISTER && operand2AddressingMode == REGISTER){
                memorySize -= 1;
            }



    }

    else if (strcmp(instruction, "not") == 0 || strcmp(instruction, "clr") == 0 || strcmp(instruction, "inc") == 0 ||
               strcmp(instruction, "dec") == 0 || strcmp(instruction, "jmp") == 0 || strcmp(instruction, "bne") == 0 ||
               strcmp(instruction, "red") == 0 || strcmp(instruction, "prn") == 0 || strcmp(instruction, "jsr") == 0) {


        removeLeadingSpaces(line);
        numberOfElements = 0;
        splitedLine = splitString(line, " ", &numberOfElements);
        printStringArray(splitedLine, numberOfElements);
        operand1AddressingMode = analyzeAddressingMode(splitedLine[0], symbolsLabelsValuesHash, entriesExternsHash);

        freeStringArray(splitedLine, numberOfElements);

        if(operand1AddressingMode == IMMEDIATE){
            memorySize += 1;
        }

        else if(operand1AddressingMode == DIRECT){
            memorySize += 1;
        }

        else if(operand1AddressingMode == INDEX){
            memorySize += 2;
        }

        /* Must be REGISTER mode*/
        else{
            memorySize += 1;
        }


    }

    memorySizeString = intToString(memorySize);
    memoryAddress = *currentMemoryAddress;
    *currentMemoryAddress += memorySize;
    memoryAddressString = intToString(memoryAddress);

    printf("originalLine: %s\n", originalLine);
    printf("operand1AddressingMode: %d\n", operand1AddressingMode);
    printf("operand2AddressingMode: %d\n", operand2AddressingMode);
    printf("Memory Address: %s\n", memoryAddressString);
    printf("Memory Size: %s\n", memorySizeString);

    if(hasLabel(originalLine)){
        ht_insert(symbolsLabelsValuesHash, labelName, "NULL", "instruction", memoryAddressString, memorySizeString, "-1");
    }

    free(memorySizeString);
    free(memoryAddressString);
    free(originalLine);

    return;

}

AddressingMode analyzeAddressingMode(char *operand, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash) {


    cleanCommand(operand);
    printf("operand: %s\n", operand);

    /* If the operand starts with a '#' suspect immediate addressing mode */
    if (operand[0] == '#') {
        return IMMEDIATE;
    /* If there is a use of existing data directive or string directive return direct addressing mode*/
    } else if ((ht_search(symbolsLabelsValuesHash, operand) != NULL) || (ht_search(entriesExternsHash, operand) != NULL )) {
        return DIRECT;

    /* If the opernad contains a '[' and a ']' and a valid integer in between suspect index addressing mode */
    } else if (strchr(operand, '[') != NULL && strchr(operand, ']') != NULL) {
            return INDEX;

    /* If the operand is a register return register addressing mode */
    } else if (strcmp(operand, "r0") == 0 || strcmp(operand, "r1") == 0 || strcmp(operand, "r2") == 0 || strcmp(operand, "r3") == 0 || strcmp(operand, "r4") == 0 || strcmp(operand, "r5") == 0 || strcmp(operand, "r6") == 0 || strcmp(operand, "r7") == 0) {
        return REGISTER;

    } else {
        return UNDEFINED_ADDRESSING;
    }
}

void analyze_data_directive(char * line, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash, HashTable *instructionsHash, FILE *outputFile){

    char **splitedLine;                    /* Array to store the splited line */
    int numberOfElements = 0;              /* Reset the elemnts number - for the string spliter counter */
    int i = 0;                             /* Loop counter */
    char * binary = NULL;                  /* String to store the binary representation of the decimal number */

    char labelName[MAX_LABEL_LENGTH];
    char value[MAX_LINE_LENGTH];

    /* Skip leading whitespaces */
    while (*line && (*line == ' ' || *line == '\t')) {
        line++;
    }

    removeSubstring(line, ".data");


    if(hasLabel(line)){

        splitedLine = splitString(line, ":", &numberOfElements);
        printStringArray(splitedLine, numberOfElements);

        strcpy(labelName, splitedLine[0]);
        freeStringArray(splitedLine, numberOfElements);

        strcpy(value, ht_search(symbolsLabelsValuesHash, labelName));
    }

    numberOfElements = 0;

    splitedLine = splitString(value, ",", &numberOfElements);
    printStringArray(splitedLine, numberOfElements);

    for (i = 0; i < numberOfElements; i++) {

        /* Write line to the output file */
        binary = (char *)malloc(14 + 1); /* Allocate memory for the binary string */
        decimalToBinary(stringToInt(splitedLine[i]), 14, binary);
        fprintf(outputFile, "%04d ", stringToInt(ht_get_memory_address(symbolsLabelsValuesHash, labelName)) + i);
        fputs(binary, outputFile);
        fputs("\n", outputFile);
        printf("binary represtation of %d is %s\n", stringToInt(splitedLine[i]), binary);
        free(binary);

    }

    freeStringArray(splitedLine, numberOfElements);

    return;

}

void analyze_string_directive(char * line, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash, HashTable *instructionsHash, FILE *outputFile){

    char **splitedLine;                    /* Array to store the splited line */
    int numberOfElements = 0;              /* Reset the elemnts number - for the string spliter counter */
    int i;                                 /* Loop counter */
    int counter = 0;                       /* Counter to count the number of characters in the string */
    char * binary = NULL;                  /* String to store the binary representation of the decimal number */

    char labelName[MAX_LABEL_LENGTH];
    char value[MAX_LINE_LENGTH];

    /* Skip leading whitespaces */
    while (*line && (*line == ' ' || *line == '\t')) {
        line++;
    }

    removeSubstring(line, ".data");


    if(hasLabel(line)){

        splitedLine = splitString(line, ":", &numberOfElements);
        printStringArray(splitedLine, numberOfElements);

        strcpy(labelName, splitedLine[0]);
        freeStringArray(splitedLine, numberOfElements);

        strcpy(value, ht_search(symbolsLabelsValuesHash, labelName));
    }

    for (i = 1; value[i+1] != '\0'; i++) {

        /* Write line to the output file */
        binary = (char *)malloc(14 + 1); /* Allocate memory for the binary string */
        printf("assci represtation of %c is %d\n", value[i], (int)(value[i]));
        decimalToBinary((int)(value[i]), 14, binary);
        fprintf(outputFile, "%04d ", stringToInt(ht_get_memory_address(symbolsLabelsValuesHash, labelName)) + (i - 1));
        fputs(binary, outputFile);
        fputs("\n", outputFile);
        printf("binary represtation of %d is %s\n", (int)(value[i]), binary);
        counter++;
        free(binary);


    }

    /* add the null terminator */
    fprintf(outputFile, "%04d ", stringToInt(ht_get_memory_address(symbolsLabelsValuesHash, labelName)) + counter);
    fputs("00000000000000", outputFile);
    fputs("\n", outputFile);

    return;

}





