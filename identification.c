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

    /* Iterate through each character in the line */
    for (i = 0, j = 0; line[i] != '\0'; i++) {
        /* Replace consecutive spaces or tabs with a single space */
        if (isspace(line[i])) {
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
    printf("Got the instruction: %s\n", instruction);
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

    printf("Line before cleaning: %s\n", line);
    cleanCommand(line);
    printf("Line after cleaning: %s\n", line);

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





