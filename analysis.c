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


void build_binary_file(char * inputFileName, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash, HashTable *instructionsHash, HashTable *registersHash){

    char line[MAX_LINE_LENGTH];             /* Buffer to store each line from the file */
    char outputFileName[MAX_LINE_LENGTH];   /* Buffer to store the output file name */
    char entryFileName[MAX_LINE_LENGTH];    /* Buffer to store the entry file name */
    char externFileName[MAX_LINE_LENGTH];   /* Buffer to store the extern file name */

    FILE *inputFile = NULL;                 /* File pointer for the input file */
    FILE *outputFile = NULL;                /* File pointer for the output file */
    FILE *entryFile = NULL;                 /* File pointer for the entry file */
    FILE *externFile = NULL;                /* File pointer for the extern file */

    CommandType commandType;                /* Type of the command in the line */

    int currentMemoryAddress = STARTING_MEMORY_LOCATION;                     /* Line number in the input file */


    /* Open the input file */
    inputFile = openFile(inputFileName, "r");

    /* Open a new file with the same name but a ".am" extension for writing */
    sprintf(outputFileName, "%s.temp", removeFileExtension(inputFileName));
    outputFile = openFile(outputFileName, "w");

    sprintf(entryFileName, "%s.ent", removeFileExtension(inputFileName));
    entryFile = openFile(entryFileName, "w");

    sprintf(externFileName, "%s.ext", removeFileExtension(inputFileName));
    externFile = openFile(externFileName, "w");


    printf("---------------------- binary instruction ----------------------\n");

    while (fgets(line, sizeof(line), inputFile) != NULL) {
        
        /* Identify the command type */
        commandType = identifyCommandType(line, instructionsHash);
        switch (commandType) {
            case INSTRUCTION:
                analyze_instruction(line, symbolsLabelsValuesHash, entriesExternsHash, instructionsHash, registersHash, outputFile, entryFile, externFile, &currentMemoryAddress);
            default:
                break;
        }

    }

    /* Move the file pointer to the beginning of the file */

    rewind(inputFile);

    printf("---------------------- binary data directive ----------------------\n");

    while (fgets(line, sizeof(line), inputFile) != NULL) {
        
        /* Identify the command type */
        commandType = identifyCommandType(line, instructionsHash);
        switch (commandType) {
            case DATA_DIRECTIVE:
                analyze_data_directive(line, symbolsLabelsValuesHash, entriesExternsHash, instructionsHash, outputFile, entryFile, externFile);
                break;
            case STRING_DIRECTIVE:
                analyze_string_directive(line, symbolsLabelsValuesHash, entriesExternsHash, instructionsHash, outputFile, entryFile, externFile);
                break;
            case INSTRUCTION:
                break;
            default:
                break;

        }

    }

    fclose(inputFile);
    fclose(outputFile);
    fclose(entryFile);
    fclose(externFile);

    if(entryExists(entriesExternsHash) == False){
        /* Attempt to delete the file */
        if (remove(entryFileName) != 0) {
            perror("Error deleting file");
        }
    }

    if(externExists(entriesExternsHash) == False){
        /* Attempt to delete the file */
        if (remove(externFileName) != 0) {
            perror("Error deleting file");
        }
    }

}

void build_encoded_file(char * inputFileName, HashTable *symbolsLabelsValuesHash, int currentMemoryAddress){

    FILE *tempFile = NULL;                 /* File pointer for the input file */
    FILE *outputFile = NULL;                /* File pointer for the output file */

    char **splitedLine;                     /* Array to store the splited line */
    int numberOfElements = 0;               /* Reset the elemnts number - for the string spliter counter */

    char line[MAX_LINE_LENGTH];             /* Buffer to store each line from the file */
    char tempFileName[MAX_LINE_LENGTH];     /* Buffer to store the output file name */
    char outputFileName[MAX_LINE_LENGTH];   /* Buffer to store the output file name */

    char * encodedBinary = NULL;            /* String to store the encoded representation of the binary string */
    char * directiveMemorySizeString = NULL;         /* String to store the memory size */
    char * instructionMemorySizeString = NULL;         /* String to store the memory size */

    sprintf(tempFileName, "%s.temp", removeFileExtension(inputFileName));

    /* Open the input file */
    tempFile = openFile(tempFileName, "r");

    /* Open a new file with the same name but a ".ob" extension for writing */
    sprintf(outputFileName, "%s.ob", removeFileExtension(inputFileName));
    outputFile = openFile(outputFileName, "w");

    directiveMemorySizeString = intToString(get_directives_memory_size(symbolsLabelsValuesHash));
    instructionMemorySizeString = intToString(currentMemoryAddress - get_directives_memory_size(symbolsLabelsValuesHash) - STARTING_MEMORY_LOCATION);

    fputs("  ", outputFile);
    fputs(instructionMemorySizeString, outputFile);
    fputs(" ", outputFile);
    fputs(directiveMemorySizeString, outputFile);
    fputs("\n", outputFile);

    printf("instructionMemorySizeString: %s\n", instructionMemorySizeString);
    printf("directiveMemorySizeString: %s\n", directiveMemorySizeString);

    free(directiveMemorySizeString);
    free(instructionMemorySizeString);



    printf("---------------------- Decode file ----------------------\n");

    while (fgets(line, sizeof(line), tempFile) != NULL) {

        splitedLine = splitString(line, " ", &numberOfElements);
        printStringArray(splitedLine, numberOfElements);

        encodedBinary = encodeBinaryString(splitedLine[1]);

        fputs(splitedLine[0], outputFile);
        fputs(" ", outputFile);
        fputs(encodedBinary, outputFile);
        fputs("\n", outputFile);

        free(encodedBinary);
        freeStringArray(splitedLine, numberOfElements);

    }

    fclose(tempFile);
    fclose(outputFile);

    /* Attempt to delete the file */
    if (remove(tempFileName) != 0) {
        perror("Error deleting file");
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

void analyze_data_directive(char * line, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash, HashTable *instructionsHash, FILE *outputFile, FILE *entryFile, FILE *externFile){

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

        if(ht_search(entriesExternsHash, labelName) != NULL && strcmp(ht_get_type(entriesExternsHash, labelName), "entryDirective") == 0){
            fprintf(entryFile, "%s %04d\n", labelName, stringToInt(ht_get_memory_address(symbolsLabelsValuesHash, labelName)));
        }

        else if(ht_search(entriesExternsHash, labelName) != NULL && strcmp(ht_get_type(entriesExternsHash, labelName), "externDirective") == 0){
            fprintf(externFile, "%s %04d\n", labelName, stringToInt(ht_get_memory_address(symbolsLabelsValuesHash, labelName)));
        }
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

void analyze_string_directive(char * line, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash, HashTable *instructionsHash, FILE *outputFile, FILE *entryFile, FILE *externFile){

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

        if(ht_search(entriesExternsHash, labelName) != NULL && strcmp(ht_get_type(entriesExternsHash, labelName), "entryDirective") == 0){
            fprintf(entryFile, "%s %04d\n", labelName, stringToInt(ht_get_memory_address(symbolsLabelsValuesHash, labelName)));
        }

        else if(ht_search(entriesExternsHash, labelName) != NULL && strcmp(ht_get_type(entriesExternsHash, labelName), "externDirective") == 0){
            fprintf(externFile, "%s %04d\n", labelName, stringToInt(ht_get_memory_address(symbolsLabelsValuesHash, labelName)));
        }
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

void analyze_instruction(char * line, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash, HashTable *instructionsHash, HashTable *registersHash, FILE *outputFile, FILE *entryFile, FILE *externFile, int * currentMemoryAddress){

    char **splitedLine;                    /* Array to store the splited line */
    int numberOfElements = 0;              /* Reset the elemnts number - for the string spliter counter */

    int memorySize = 1;                    /* The size of the memory to be allocated */
    int memoryAddress;                     /* The memory address of the directive */
    int labelIndex;

    char * binary = NULL;                  /* String to store the binary representation of the decimal number */

    bool alsoRegister = False;

    AddressingMode operand1AddressingMode;
    AddressingMode operand2AddressingMode;

    /* Allocate memory for the binary string */

    char firstWordbinaryCode[15] = ""; 
    char secondWordbinaryCode[15] = ""; 
    char thirdWordbinaryCode[15] = ""; 
    char fourthWordbinaryCode[15] = ""; 
    char fifthWordbinaryCode[15] = ""; 

    bool hasSecondWord = False;
    bool hasThirdWord = False;
    bool hasFourthWord = False;
    bool hasFifthWord = False;
    
    char labelName[MAX_LABEL_LENGTH];
    char instruction[4];

    /* Skip leading whitespaces */
    while (*line && (*line == ' ' || *line == '\t')) {
        line++;
    }

    strcat(firstWordbinaryCode,  "0000");

    if(hasLabel(line)){

        splitedLine = splitString(line, ":", &numberOfElements);
        printStringArray(splitedLine, numberOfElements);

        strcpy(labelName, splitedLine[0]);

        if(ht_search(entriesExternsHash, labelName) != NULL && strcmp(ht_get_type(entriesExternsHash, labelName), "entryDirective") == 0){
            fprintf(entryFile, "%s %04d\n", labelName, *currentMemoryAddress);
        }

        else if(ht_search(entriesExternsHash, labelName) != NULL && strcmp(ht_get_type(entriesExternsHash, labelName), "externDirective") == 0){
            fprintf(externFile, "%s %04d\n", labelName, *currentMemoryAddress);
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

    removeSubstring(line, instruction);
    strcat(firstWordbinaryCode, ht_search(instructionsHash, instruction));


    if (strcmp(instruction, "mov") == 0 || strcmp(instruction, "cmp") == 0 || strcmp(instruction, "add") == 0 ||
        strcmp(instruction, "sub") == 0 || strcmp(instruction, "lea") == 0) {


            removeLeadingSpaces(line);
            numberOfElements = 0;
            splitedLine = splitString(line, ",", &numberOfElements);

            operand1AddressingMode = analyzeAddressingMode(splitedLine[0], symbolsLabelsValuesHash, entriesExternsHash);
            operand2AddressingMode = analyzeAddressingMode(splitedLine[1], symbolsLabelsValuesHash, entriesExternsHash);

            if(operand1AddressingMode == IMMEDIATE){
                memorySize += 1;
                strcat(firstWordbinaryCode, "00");

                hasSecondWord = True;
                binary = (char *)malloc(12 + 1); /* Allocate memory for the binary string */
                decimalToBinary(get_imidiate_data(splitedLine[0], symbolsLabelsValuesHash, entriesExternsHash), 12, binary);
                strcat(secondWordbinaryCode, binary);
                strcat(secondWordbinaryCode, "00");
                free(binary);
            }

            else if(operand1AddressingMode == DIRECT){
                memorySize += 1;
                strcat(firstWordbinaryCode, "01");

                hasSecondWord = True;
                binary = (char *)malloc(12 + 1); /* Allocate memory for the binary string */
                decimalToBinary(get_label_address(splitedLine[0], symbolsLabelsValuesHash, entriesExternsHash), 12, binary);
                strcat(secondWordbinaryCode, binary);
                if(ht_search(entriesExternsHash, splitedLine[0]) != NULL && strcmp(ht_get_type(entriesExternsHash, splitedLine[0]), "externDirective") == 0){
                    strcat(secondWordbinaryCode, "01");
                }
                else{
                    strcat(secondWordbinaryCode, "10");
                }

                free(binary);

                if(ht_search(entriesExternsHash, splitedLine[0]) != NULL && strcmp(ht_get_type(entriesExternsHash, splitedLine[0]), "entryDirective") == 0){
                    fprintf(entryFile, "%s %04d\n", splitedLine[0], (*currentMemoryAddress) + 1);
                }

                else if(ht_search(entriesExternsHash, splitedLine[0]) != NULL && strcmp(ht_get_type(entriesExternsHash, splitedLine[0]), "externDirective") == 0){
                    fprintf(externFile, "%s %04d\n", splitedLine[0], (*currentMemoryAddress) + 1);
                }


            }

            else if(operand1AddressingMode == INDEX){
                memorySize += 2;
                strcat(firstWordbinaryCode, "10");

                hasSecondWord = True;
                hasThirdWord = True;
                binary = (char *)malloc(12 + 1); /* Allocate memory for the binary string */
                decimalToBinary(get_indexed_label_address(splitedLine[0], symbolsLabelsValuesHash, entriesExternsHash, &labelIndex), 12, binary);
                strcat(secondWordbinaryCode, binary);
                free(binary);
                binary = (char *)malloc(12 + 1); /* Allocate memory for the binary string */
                decimalToBinary(labelIndex, 12, binary);
                strcat(thirdWordbinaryCode, binary);

                if(ht_search(entriesExternsHash, splitedLine[0]) != NULL && strcmp(ht_get_type(entriesExternsHash, splitedLine[0]), "externDirective") == 0){
                    strcat(secondWordbinaryCode, "01");
                    strcat(thirdWordbinaryCode, "00");
                }
                else{
                    strcat(secondWordbinaryCode, "10");
                    strcat(thirdWordbinaryCode, "00");
                }

                free(binary);

                if(ht_search(entriesExternsHash, splitedLine[0]) != NULL && strcmp(ht_get_type(entriesExternsHash, splitedLine[0]), "entryDirective") == 0){
                    fprintf(entryFile, "%s %04d\n", splitedLine[0], (*currentMemoryAddress) + 1);
                }

                else if(ht_search(entriesExternsHash, splitedLine[0]) != NULL && strcmp(ht_get_type(entriesExternsHash, splitedLine[0]), "externDirective") == 0){
                    fprintf(externFile, "%s %04d\n", splitedLine[0], (*currentMemoryAddress) + 1);
                }

            }

            else if(operand1AddressingMode == REGISTER){
                memorySize += 1;
                strcat(firstWordbinaryCode, "11");

                hasSecondWord = True;
                strcat(secondWordbinaryCode, "000000");
                strcat(secondWordbinaryCode, ht_search(registersHash, splitedLine[0]));                


            }

            if(operand2AddressingMode == IMMEDIATE){
                memorySize += 1;
                strcat(firstWordbinaryCode, "00");
                binary = (char *)malloc(12 + 1); /* Allocate memory for the binary string */
                decimalToBinary(get_imidiate_data(splitedLine[1], symbolsLabelsValuesHash, entriesExternsHash), 12, binary);

                if(operand1AddressingMode != INDEX){
                    hasThirdWord = True;
                    strcat(thirdWordbinaryCode, binary);
                    strcat(thirdWordbinaryCode, "00");
                }

                else{
                    hasFourthWord = True;
                    strcat(fourthWordbinaryCode, binary);
                    strcat(fourthWordbinaryCode, "00");
                }

                free(binary);

            }

            else if(operand2AddressingMode == DIRECT){
                memorySize += 1;
                strcat(firstWordbinaryCode, "01");
                binary = (char *)malloc(12 + 1); /* Allocate memory for the binary string */
                decimalToBinary(get_imidiate_data(splitedLine[1], symbolsLabelsValuesHash, entriesExternsHash), 12, binary);

                if(ht_search(entriesExternsHash, splitedLine[1]) != NULL && strcmp(ht_get_type(entriesExternsHash, splitedLine[1]), "entryDirective") == 0){
                    fprintf(entryFile, "%s %04d\n", splitedLine[1], (*currentMemoryAddress) + 1);
                }

                else if(ht_search(entriesExternsHash, splitedLine[1]) != NULL && strcmp(ht_get_type(entriesExternsHash, splitedLine[1]), "externDirective") == 0){
                    fprintf(externFile, "%s %04d\n", splitedLine[1], (*currentMemoryAddress) + 1);
                }

                if(operand1AddressingMode != INDEX){
                    hasThirdWord = True;
                    strcat(thirdWordbinaryCode, binary);
                    
                    if(ht_search(entriesExternsHash, splitedLine[1]) != NULL && strcmp(ht_get_type(entriesExternsHash, splitedLine[0]), "externDirective") == 0){
                        strcat(thirdWordbinaryCode, "01");
                    }
                    else{
                        strcat(thirdWordbinaryCode, "10");
                    }
                }

                else{
                    hasFourthWord = True;
                    strcat(fourthWordbinaryCode, binary);
                    if(ht_search(entriesExternsHash, splitedLine[1]) != NULL && strcmp(ht_get_type(entriesExternsHash, splitedLine[0]), "externDirective") == 0){
                        strcat(fourthWordbinaryCode, "01");
                    }
                    else{
                        strcat(fourthWordbinaryCode, "10");
                    }
                }
                free(binary);

            }

            else if(operand2AddressingMode == INDEX){
                memorySize += 2;
                strcat(firstWordbinaryCode, "10");
                binary = (char *)malloc(12 + 1); /* Allocate memory for the binary string */
                decimalToBinary(get_indexed_label_address(splitedLine[1], symbolsLabelsValuesHash, entriesExternsHash, &labelIndex), 12, binary);

                if(ht_search(entriesExternsHash, splitedLine[1]) != NULL && strcmp(ht_get_type(entriesExternsHash, splitedLine[1]), "entryDirective") == 0){
                    fprintf(entryFile, "%s %04d\n", splitedLine[1], (*currentMemoryAddress) + 1);
                }

                else if(ht_search(entriesExternsHash, splitedLine[1]) != NULL && strcmp(ht_get_type(entriesExternsHash, splitedLine[1]), "externDirective") == 0){
                    fprintf(externFile, "%s %04d\n", splitedLine[1], (*currentMemoryAddress) + 1);
                }

                if(operand1AddressingMode != INDEX){
                    hasThirdWord = True;
                    hasFourthWord = True;
                    strcat(thirdWordbinaryCode, binary);
                    free(binary);
                    binary = (char *)malloc(12 + 1); /* Allocate memory for the binary string */
                    decimalToBinary(labelIndex, 12, binary);
                    strcat(fourthWordbinaryCode, binary);
                    free(binary);
                    if(ht_search(entriesExternsHash, splitedLine[1]) != NULL && strcmp(ht_get_type(entriesExternsHash, splitedLine[0]), "externDirective") == 0){
                        strcat(thirdWordbinaryCode, "01");
                        strcat(fourthWordbinaryCode, "00");
                    }
                    else{
                        strcat(thirdWordbinaryCode, "10");
                        strcat(fourthWordbinaryCode, "00");
                    }
                }

                else{
                    hasFourthWord = True;
                    hasFifthWord = True;
                    strcat(fourthWordbinaryCode, binary);
                    free(binary);
                    binary = (char *)malloc(12 + 1); /* Allocate memory for the binary string */
                    decimalToBinary(labelIndex, 12, binary);
                    strcat(fifthWordbinaryCode, binary);
                    free(binary);
                    if(ht_search(entriesExternsHash, splitedLine[1]) != NULL && strcmp(ht_get_type(entriesExternsHash, splitedLine[0]), "externDirective") == 0){
                        strcat(fourthWordbinaryCode, "01");
                        strcat(fifthWordbinaryCode, "00");
                    }
                    else{
                        strcat(fourthWordbinaryCode, "10");
                        strcat(fifthWordbinaryCode, "00");
                    }
                }

            }

            else if(operand2AddressingMode == REGISTER){
                memorySize += 1;
                strcat(firstWordbinaryCode, "11");
                if(operand1AddressingMode == REGISTER){
                    alsoRegister = True;
                }

                if(alsoRegister == False){
                    if(operand1AddressingMode != INDEX){

                        hasThirdWord = True;
                        strcat(thirdWordbinaryCode, "000000");
                        strcat(thirdWordbinaryCode, ht_search(registersHash, splitedLine[1]));
                        strcat(thirdWordbinaryCode, "00");
                    }

                    else{
                        hasFourthWord = True;
                        strcat(fourthWordbinaryCode, "000000");
                        strcat(fourthWordbinaryCode, ht_search(registersHash, splitedLine[1]));
                        strcat(fourthWordbinaryCode, "00");
                    }
                }

            }

            /* If both operands are registers, then the memory size should be decreased by 1 */
            if(operand1AddressingMode == REGISTER && operand2AddressingMode == REGISTER){
                memorySize -= 1;
                hasThirdWord = False;
                strcat(secondWordbinaryCode, ht_search(registersHash, splitedLine[1]));
                strcat(secondWordbinaryCode, "00");
            }

            if(operand1AddressingMode == REGISTER && operand2AddressingMode != REGISTER){
                strcat(secondWordbinaryCode, "00000");
            }


            freeStringArray(splitedLine, numberOfElements);

    }

    else if (strcmp(instruction, "not") == 0 || strcmp(instruction, "clr") == 0 || strcmp(instruction, "inc") == 0 ||
               strcmp(instruction, "dec") == 0 || strcmp(instruction, "jmp") == 0 || strcmp(instruction, "bne") == 0 ||
               strcmp(instruction, "red") == 0 || strcmp(instruction, "prn") == 0 || strcmp(instruction, "jsr") == 0) {


        removeLeadingSpaces(line);
        numberOfElements = 0;
        splitedLine = splitString(line, " ", &numberOfElements);
        printStringArray(splitedLine, numberOfElements);
        operand1AddressingMode = analyzeAddressingMode(splitedLine[0], symbolsLabelsValuesHash, entriesExternsHash);

        strcat(firstWordbinaryCode, "00");

        if(operand1AddressingMode == IMMEDIATE){
            strcat(firstWordbinaryCode, "00");
            memorySize += 1;

            hasSecondWord = True;
            binary = (char *)malloc(12 + 1); /* Allocate memory for the binary string */
            decimalToBinary(get_imidiate_data(splitedLine[0], symbolsLabelsValuesHash, entriesExternsHash), 12, binary);
            strcat(secondWordbinaryCode, binary);
            strcat(secondWordbinaryCode, "00");
            free(binary);

        }

        else if(operand1AddressingMode == DIRECT){
            strcat(firstWordbinaryCode, "01");
            memorySize += 1;

            hasSecondWord = True;
            binary = (char *)malloc(12 + 1); /* Allocate memory for the binary string */
            decimalToBinary(get_label_address(splitedLine[0], symbolsLabelsValuesHash, entriesExternsHash), 12, binary);
            strcat(secondWordbinaryCode, binary);
            if(ht_search(entriesExternsHash, splitedLine[0]) != NULL && strcmp(ht_get_type(entriesExternsHash, splitedLine[0]), "externDirective") == 0){
                strcat(secondWordbinaryCode, "01");
            }
            else{
                strcat(secondWordbinaryCode, "10");
            }

            free(binary);

            if(ht_search(entriesExternsHash, splitedLine[0]) != NULL && strcmp(ht_get_type(entriesExternsHash, splitedLine[0]), "entryDirective") == 0){
                fprintf(entryFile, "%s %04d\n", splitedLine[0], (*currentMemoryAddress) + 1);
            }

            else if(ht_search(entriesExternsHash, splitedLine[0]) != NULL && strcmp(ht_get_type(entriesExternsHash, splitedLine[0]), "externDirective") == 0){
                fprintf(externFile, "%s %04d\n", splitedLine[0], (*currentMemoryAddress) + 1);
            }

        }

        else if(operand1AddressingMode == INDEX){
            strcat(firstWordbinaryCode, "10");
            memorySize += 2;
            hasSecondWord = True;
            hasThirdWord = True;
            binary = (char *)malloc(12 + 1); /* Allocate memory for the binary string */
            decimalToBinary(get_indexed_label_address(splitedLine[0], symbolsLabelsValuesHash, entriesExternsHash, &labelIndex), 12, binary);
            strcat(secondWordbinaryCode, binary);
            free(binary);
            binary = (char *)malloc(12 + 1); /* Allocate memory for the binary string */
            decimalToBinary(labelIndex, 12, binary);
            strcat(thirdWordbinaryCode, binary);
            free(binary);

            if(ht_search(entriesExternsHash, splitedLine[0]) != NULL && strcmp(ht_get_type(entriesExternsHash, splitedLine[0]), "entryDirective") == 0){
                fprintf(entryFile, "%s %04d\n", splitedLine[0], (*currentMemoryAddress) + 1);
            }

            else if(ht_search(entriesExternsHash, splitedLine[0]) != NULL && strcmp(ht_get_type(entriesExternsHash, splitedLine[0]), "externDirective") == 0){
                fprintf(externFile, "%s %04d\n", splitedLine[0], (*currentMemoryAddress) + 1);
            }

        }

        /* Must be REGISTER mode*/
        else{
            strcat(firstWordbinaryCode, "11");
            memorySize += 1;
            hasSecondWord = True;
            strcat(secondWordbinaryCode, "0000000000");
            strcat(secondWordbinaryCode, ht_search(registersHash, splitedLine[0]));
            strcat(secondWordbinaryCode, "00");
        }

        freeStringArray(splitedLine, numberOfElements);

    }

    else if (strcmp(instruction, "rts") == 0 || strcmp(instruction, "hlt") == 0) {
        strcat(firstWordbinaryCode, "0000");
    }

    strcat(firstWordbinaryCode, "00");

    memoryAddress = *currentMemoryAddress;

    printf("operand1AddressingMode: %d\n", operand1AddressingMode);
    printf("operand2AddressingMode: %d\n", operand2AddressingMode);
    printf("firstWordbinaryCode: %s\n", firstWordbinaryCode);
    printf("Memory Address: %d\n", memoryAddress);

    fprintf(outputFile, "%04d ", *currentMemoryAddress);
    fputs(firstWordbinaryCode, outputFile);
    fputs("\n", outputFile);

    if(hasSecondWord){
        printf("secondWordbinaryCode: %s\n", secondWordbinaryCode);
        fprintf(outputFile, "%04d ", *currentMemoryAddress + 1);
        fputs(secondWordbinaryCode, outputFile);
        fputs("\n", outputFile);
    }

    if(hasThirdWord){
        printf("thirdWordbinaryCode: %s\n", thirdWordbinaryCode);
        fprintf(outputFile, "%04d ", *currentMemoryAddress + 2);
        fputs(thirdWordbinaryCode, outputFile);
        fputs("\n", outputFile);
    }

    if(hasFourthWord){
        printf("fourthWordbinaryCode: %s\n", fourthWordbinaryCode);
        fprintf(outputFile, "%04d ", *currentMemoryAddress + 3);
        fputs(fourthWordbinaryCode, outputFile);
        fputs("\n", outputFile);
    }

    if(hasFifthWord){
        printf("fifthWordbinaryCode: %s\n", fifthWordbinaryCode);
        fprintf(outputFile, "%04d ", *currentMemoryAddress + 4);
        fputs(fifthWordbinaryCode, outputFile);
        fputs("\n", outputFile);
    }

    *currentMemoryAddress += memorySize;

    return;


}

int get_imidiate_data(char *operand, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash){

        char **splitedLine;                    /* Array to store the splited line */
        int numberOfElements = 0;              /* Reset the elemnts number - for the string spliter counter */
        int value;                         /* The value of the immediate data */
        cleanCommand(operand);

        if (operand[0] == '#') {

            splitedLine = splitString(operand, "#", &numberOfElements);
            printStringArray(splitedLine, numberOfElements);

            if(isValidInteger(splitedLine[0])){
                printf("valid integer %s\n", splitedLine[0]);
                value = stringToInt(splitedLine[0]);
                freeStringArray(splitedLine, numberOfElements);
                return value;
            }

            else if(ht_search(symbolsLabelsValuesHash, splitedLine[0]) != NULL && strcmp(ht_get_type(symbolsLabelsValuesHash, splitedLine[0]), "constant") == 0){
                printf("valid constant\n");
                value = stringToInt(ht_search(symbolsLabelsValuesHash, splitedLine[0]));
                freeStringArray(splitedLine, numberOfElements);
                return value;
            }

        }

        printf("Error: invalid immediate data\n");

        return 0;
}

int get_label_address(char *label, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash){

    cleanCommand(label);
    printf("label: %s\n", label);

    if (ht_search(symbolsLabelsValuesHash, label) != NULL && (strcmp(ht_get_type(symbolsLabelsValuesHash, label), "dataDirective") == 0 ||
                                                              strcmp(ht_get_type(symbolsLabelsValuesHash, label), "stringDirective") == 0 ||
                                                              strcmp(ht_get_type(symbolsLabelsValuesHash, label), "instruction") == 0)){
            printf("direct label\n");
            return stringToInt(ht_get_memory_address(symbolsLabelsValuesHash, label));

    }        

    else if(ht_search(entriesExternsHash, label) != NULL){
        printf("extern label\n");
        return 0;
    }

        return 0;
}

int get_indexed_label_address(char *label, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash, int * index){

    char **splitedLine;                    /* Array to store the splited line */
    int numberOfElements = 0;              /* Reset the elemnts number - for the string spliter counter */
    int address;                           /* The address of the indexed label */

    if (strchr(label, '[') != NULL && strchr(label, ']') != NULL) {

            splitedLine = splitString(label, "[", &numberOfElements);
            removeSubstring(splitedLine[1], "]");
            
            if(ht_search(symbolsLabelsValuesHash,splitedLine[0]) != NULL && (strcmp(ht_get_type(symbolsLabelsValuesHash, splitedLine[0]), "dataDirective") == 0 || strcmp(ht_get_type(symbolsLabelsValuesHash, splitedLine[0]), "stringDirective") == 0)){
                if(isValidInteger(splitedLine[1])){

                    *index = stringToInt(splitedLine[1]);
                    address = stringToInt(ht_get_memory_address(symbolsLabelsValuesHash, splitedLine[0]));
                    freeStringArray(splitedLine, numberOfElements);
                    return address;
                }

                else if (ht_search(symbolsLabelsValuesHash, splitedLine[1]) != NULL && strcmp(ht_get_type(symbolsLabelsValuesHash, splitedLine[1]), "constant") == 0){

                    *index = stringToInt(ht_search(symbolsLabelsValuesHash, splitedLine[1]));
                    address = stringToInt(ht_get_memory_address(symbolsLabelsValuesHash, splitedLine[0]));
                    freeStringArray(splitedLine, numberOfElements);
                    return address;
                }

            }

            else{
                
                printf("extern label\n");
                if(isValidInteger(splitedLine[1])){

                    *index = stringToInt(splitedLine[1]);
                    freeStringArray(splitedLine, numberOfElements);
                    return 0;
                }

                else if (ht_search(symbolsLabelsValuesHash, splitedLine[1]) != NULL && strcmp(ht_get_type(symbolsLabelsValuesHash, splitedLine[1]), "constant") == 0){

                    *index = stringToInt(ht_search(symbolsLabelsValuesHash, splitedLine[1]));
                    freeStringArray(splitedLine, numberOfElements);
                    return 0;
                }

                return 0;
            }
    }

    return 0;
}

/**
 * Transforms a binary string by replacing each pair of bits with a corresponding symbol.
 * '00' is replaced with '*', '01' with '#', '10' with '%', and '11' with '!'.
 * 
 * @param binaryString The input binary string to transform.
 * @return The transformed string. Memory is dynamically allocated and should be freed by the caller.
 */
char* encodeBinaryString(const char* binaryString) {

    size_t i;
    size_t length = strlen(binaryString);
    char* transformedString = (char*)malloc((length + 1) * sizeof(char)); /* +1 for null terminator */
    if (transformedString == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < length; i += 2) {
        if (binaryString[i] == '0') {
            if (binaryString[i + 1] == '0') {
                transformedString[i / 2] = '*';
            } else if (binaryString[i + 1] == '1') {
                transformedString[i / 2] = '#';
            }
        } else if (binaryString[i] == '1') {
            if (binaryString[i + 1] == '0') {
                transformedString[i / 2] = '%';
            } else if (binaryString[i + 1] == '1') {
                transformedString[i / 2] = '!';
            }
        }
    }

    transformedString[length / 2] = '\0'; /* Null-terminate the transformed string */
    return transformedString;
}

int get_directives_memory_size(HashTable *table){

    int i, j, order;
    int totalMemorySize = 0;

    for (j = 0; j < table -> size; j++){

        for (i = 0; i < table -> size; i++)
        {
            if ((table -> items[i]) && (table -> items[i] -> order != NULL)){
                order = stringToInt(table -> items[i] -> order);
                if (order == j && (strcmp(table -> items[i] -> type, "dataDirective") == 0 || strcmp(table -> items[i] -> type, "stringDirective") == 0))
                {
                    totalMemorySize += stringToInt(table -> items[i] -> memorySize);
                }
            }
        }
    }

    return totalMemorySize;
}

bool entryExists(HashTable *table) {

    int i, j;

    for (j = 0; j < table -> size; j++){

        for (i = 0; i < table -> size; i++)
        {
            if ((table -> items[i]) && (table -> items[i] -> order != NULL)){
                if ((strcmp(table -> items[i] -> type, "entryDirective") == 0))
                {
                    return True;
                }
            }
        }
    }

    return False;
}

bool externExists(HashTable *table) {

    int i, j;

    for (j = 0; j < table -> size; j++){

        for (i = 0; i < table -> size; i++)
        {
            if ((table -> items[i]) && (table -> items[i] -> order != NULL)){
                if ((strcmp(table -> items[i] -> type, "externDirective") == 0))
                {
                    return True;
                }
            }
        }
    }

    return False;
}











