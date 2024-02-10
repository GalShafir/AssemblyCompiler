#include <stdio.h>
#include <stdlib.h>

#include "pre_processor.h"
#include "global_definitions.h"
#include "compilation.h"

int compile(char * fileName){

    FILE *file = NULL;

    /* Open the file for reading */    
    file = openFile(fileName);

    if (file == NULL) {
        return EXIT_FAILURE;
    }

    /* Pre-process the file */
    preProcessFile(file);

    return EXIT_SUCCESS;
}