#include <stdio.h>
#include <stdlib.h>
#include "pre_processor.h"
#include "global_definitions.h"

FILE* openFile(char * fileName) {
    
    FILE *file = NULL;

    /* Open the file for reading and check for errors */
    file = fopen(fileName, "r");
    if (file == NULL) {
        perror(FILE_OPEN_ERROR);
        return NULL;
    }

    return file;
}

int preProcessFile(FILE *file) {
    
    printf("Pre-processing file...\n");
    return 0;
}