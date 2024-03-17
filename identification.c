#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "global_definitions.h"
#include "utility_functions.h"
#include "identification.h"

/**
 * Clean a command by removing leading whitespaces and replacing
 * consecutive spaces or tabs with a single space.
 *
 * @param line - The command string to be cleaned (modified in place).
 */
void cleanCommand(char *line) {
    int i, j;
    int leadingSpace = 1;  /* Flag to track leading spaces */
    int insideQuotes = 0; /* Flag to track if inside quotes */

    /* Iterate through each character in the line */
    for (i = 0, j = 0; line[i] != '\0'; i++) {
        /* Check if the current character is a quote */
        if (line[i] == '"') {
            /* Toggle the insideQuotes flag */
            insideQuotes = !insideQuotes;
        }

        /* Replace consecutive spaces or tabs with a single space */
        if (!insideQuotes && isspace(line[i])) {
            if (!leadingSpace) {
                line[j++] = ' ';
                leadingSpace = 1;
            }
        } else {
            line[j++] = line[i];
            leadingSpace = 0;
        }
    }

    /* Remove leading spaces */
    while (j > 0 && isspace(line[j - 1])) {
        j--;
    }

    /* Null-terminate the cleaned line */
    line[j] = '\0';
}

bool isEmpty(char *line) {
    int i;
    for (i = 0; line[i] != '\0'; i++) {
        if (!isspace((unsigned char)line[i])) {
            return False;
        }
    }
    return True;
}

bool isComment(char *line) {
    return line[0] == ';';
}

bool isDirective(char *line) {
    char directive[MAX_LINE_LENGTH];
    int i = 0;

    /* Skip leading whitespaces */
    while (*line && (*line == ' ' || *line == '\t')) {
        line++;
    }

    /* Check for a label */
    while (*line) {
        if (*line == ':') {
            line++;  /* Skip the colon */
            break;
        } else if (*line == ' ' || *line == '\t' || *line == '.') {
            break;  /* End of label, no colon found */
        }
        line++;
    }

    /* Skip whitespaces after the label */
    while (*line && (*line == ' ' || *line == '\t')) {
        line++;
    }

    /* Check for directive starting with '.' */
    if (*line == '.') {
        /* Extract the directive name */
        line++;  /* Skip the '.' */
        while (*line && (*line != ' ' && *line != '\t' && *line != '\n')) {
            directive[i++] = *line++;
        }
        directive[i] = '\0';

        /* Compare directive name */
        if (strcmp(directive, "data") == 0 || strcmp(directive, "string") == 0 || strcmp(directive, "entry") == 0 || strcmp(directive, "extern") == 0) {
            return True;
        }
    }

    return False;
}

CommandType identifyDirective(char *line) {

    char directive[MAX_LINE_LENGTH];
    int i = 0;

    /* Skip leading whitespaces */
    while (*line && (*line == ' ' || *line == '\t')) {
        line++;
    }

    /* Check for a label */
    while (*line) {
        if (*line == ':') {
            line++;  /* Skip the colon */
            break;
        } else if (*line == ' ' || *line == '\t' || *line == '.') {
            break;  /* End of label, no colon found */
        }
        line++;
    }

    /* Skip whitespaces after the label */
    while (*line && (*line == ' ' || *line == '\t')) {
        line++;
    }

    /* Check for directive starting with '.' */
    if (*line == '.') {
        /* Extract the directive name */
        line++;  /* Skip the '.' */
        while (*line && (*line != ' ' && *line != '\t' && *line != '\n')) {
            directive[i++] = *line++;
        }
        directive[i] = '\0';


        /* Compare directive name */
        if (strcmp(directive, "data") == 0) {
            return DATA_DIRECTIVE;
        } else if (strcmp(directive, "string") == 0) {
            return STRING_DIRECTIVE;
        } else if (strcmp(directive, "entry") == 0) {
            return ENTRY_DIRECTIVE;
        } else if (strcmp(directive, "extern") == 0) {
            return EXTERN_DIRECTIVE;
        }
    }

    return UNDEFINED_DIRECTIVE;
}

bool hasLabel(char *line) {
    while (*line) {
        if (*line == ':') {
            return True;
        } else if (*line == ' ' || *line == '\t') {
            return False;
        }
        line++;
    }
    return False;
}

void removeLabel(char *line) {
    while (*line) {
        if (*line == ':') {
            *line = '\0';
            break;
        }
        line++;
    }
}

bool isInstruction(char *line, HashTable* instructionsHash) {

    char instruction[MAX_LINE_LENGTH];
    char ** splitedLine;                    /* Array to store the splited line */
    int numberOfElements = 0;               /* Reset the elemnts number - for the string spliter counter */


    /* Skip leading whitespaces */
    while (*line && (*line == ' ' || *line == '\t')) {
        line++;
    }

    splitedLine = splitString(line, " ", &numberOfElements);

    /* Check for a label */
    if(hasLabel(line)){
        strcpy(instruction, splitedLine[1]);
    }
    else{
        strcpy(instruction, splitedLine[0]);
    }

    /* Free the memory allocated for the splitedLine */
    freeStringArray(splitedLine, numberOfElements);

    /* Compare instruction name */
    if (existsInHash(instructionsHash, instruction)) {
        return True;
    }

    return False;
}

CommandType identifyInstruction(char *line, HashTable* instructionsHash){
    if (isInstruction(line, instructionsHash)) {
        return INSTRUCTION;
    } else {
        return UNDEFINED;
    }

}

bool isConstant(char *line) {

    char constant[MAX_LINE_LENGTH];
    int i = 0;

    /* Skip leading whitespaces */
    while (*line && (*line == ' ' || *line == '\t')) {
        line++;
    }

    /* Check for a label */
    while (*line) {
        if (*line == ':') {
            line++;  /* Skip the colon */
            break;
        } else if (*line == ' ' || *line == '\t' || *line == '.') {
            break;  /* End of label, no colon found */
        }
        line++;
    }

    /* Skip whitespaces after the label */
    while (*line && (*line == ' ' || *line == '\t')) {
        line++;
    }

    /* Check for constant starting with '.' */
    if (*line == '.') {
        /* Extract the constant name */
        line++;  /* Skip the '.' */
        while (*line && (*line != ' ' && *line != '\t' && *line != '\n')) {
            constant[i++] = *line++;
        }
        constant[i] = '\0';

        /* Compare constant name */
        if (strcmp(constant, "define") == 0) {
            return True;
        }
    }

    return False;
}


CommandType identifyCommandType(char *line, HashTable* instructionsHash) {

    cleanCommand(line);

    if (isEmpty(line)) {
        return EMPTY;
    } else if (isComment(line)) {
        return COMMENT;
    } else if (isDirective(line)) {
        return identifyDirective(line);
    }  else if (isConstant(line)) {
        return CONSTANT;
    } else if (isInstruction(line, instructionsHash)) {
        return INSTRUCTION;
    } else {
        return UNDEFINED;
    }
}

AddressingMode identifyAddressingMode(char *operand, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash) {

    char **splitedLine;                    /* Array to store the splited line */
    int numberOfElements = 0;              /* Reset the elemnts number - for the string spliter counter */
    char * directiveIndex = NULL;
    char * constantValueString = NULL;

    cleanCommand(operand);

    /* If the operand starts with a '#' suspect immediate addressing mode */
    if (operand[0] == '#') {
        splitedLine = splitString(operand, "#", &numberOfElements);

        if(isValidInteger(splitedLine[0])){
            freeStringArray(splitedLine, numberOfElements);
            return IMMEDIATE;
        }

        else if(ht_search(symbolsLabelsValuesHash, splitedLine[0]) != NULL && strcmp(ht_get_type(symbolsLabelsValuesHash, splitedLine[0]), "constant") == 0){
            freeStringArray(splitedLine, numberOfElements);
            return IMMEDIATE;
        }
        else{
            freeStringArray(splitedLine, numberOfElements);
            return UNDEFINED_CONSTANT;
        }
    /* If there is a use of existing data directive or string directive return direct addressing mode*/
    } else if ((ht_search(symbolsLabelsValuesHash, operand) != NULL && (strcmp(ht_get_type(symbolsLabelsValuesHash, operand), "dataDirective") == 0 || strcmp(ht_get_type(symbolsLabelsValuesHash, operand), "stringDirective") == 0 || strcmp(ht_get_type(symbolsLabelsValuesHash, operand), "instruction") == 0))
     || (ht_search(entriesExternsHash, operand) != NULL && ((strcmp(ht_get_type(entriesExternsHash, operand), "entryDirective") == 0) || (strcmp(ht_get_type(entriesExternsHash, operand), "externDirective") == 0)))) {
        return DIRECT;

    /* If the opernad contains a '[' and a ']' and a valid integer in between suspect index addressing mode */
    } else if (strchr(operand, '[') != NULL && strchr(operand, ']') != NULL) {
        splitedLine = splitString(operand, "[", &numberOfElements);
        removeSubstring(splitedLine[1], "]");
        
        if(ht_search(symbolsLabelsValuesHash,splitedLine[0]) != NULL && (strcmp(ht_get_type(symbolsLabelsValuesHash, splitedLine[0]), "dataDirective") == 0 || strcmp(ht_get_type(symbolsLabelsValuesHash, splitedLine[0]), "stringDirective") == 0)){
            if(isValidInteger(splitedLine[1])){
                directiveIndex = malloc(14);
                strcpy(directiveIndex, ht_get_memory_size(symbolsLabelsValuesHash, splitedLine[0]));
                if(stringToInt(splitedLine[1]) < 0 || stringToInt(splitedLine[1]) > (stringToInt(directiveIndex) -1)){
                    freeStringArray(splitedLine, numberOfElements);
                    free(directiveIndex);
                    return INDEX_OVERFLOW;
                }
                freeStringArray(splitedLine, numberOfElements);
                free(directiveIndex);
                return INDEX;
            }

            else if (ht_search(symbolsLabelsValuesHash, splitedLine[1]) != NULL && strcmp(ht_get_type(symbolsLabelsValuesHash, splitedLine[1]), "constant") == 0){

                directiveIndex = malloc(14);
                constantValueString = malloc(14);
                strcpy(directiveIndex, ht_get_memory_size(symbolsLabelsValuesHash, splitedLine[0]));
                strcpy(constantValueString, ht_search(symbolsLabelsValuesHash, splitedLine[1]));

                if(stringToInt(constantValueString) < 0 || stringToInt(constantValueString) > (stringToInt(directiveIndex) -1)){
                    freeStringArray(splitedLine, numberOfElements);
                    free(directiveIndex);
                    free(constantValueString);
                    return INDEX_OVERFLOW;
                }

                freeStringArray(splitedLine, numberOfElements);
                free(directiveIndex);
                free(constantValueString);
                return INDEX;
            }

            else{
                freeStringArray(splitedLine, numberOfElements);
                return UNDEFINED_CONSTANT;
            }

        } else if(ht_search(entriesExternsHash, splitedLine[0]) != NULL && (strcmp(ht_get_type(entriesExternsHash, splitedLine[0]), "externDirective") == 0)){
            freeStringArray(splitedLine, numberOfElements);
            return INDEX;
        }


        else{
            freeStringArray(splitedLine, numberOfElements);
            return UNDEFINED_LABEL;
        }


    /* If the operand is a register return register addressing mode */
    } else if (strcmp(operand, "r0") == 0 || strcmp(operand, "r1") == 0 || strcmp(operand, "r2") == 0 || strcmp(operand, "r3") == 0 || strcmp(operand, "r4") == 0 || strcmp(operand, "r5") == 0 || strcmp(operand, "r6") == 0 || strcmp(operand, "r7") == 0) {
        return REGISTER;

    } else {
        return UNDEFINED_ADDRESSING;
    }
}





