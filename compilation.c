#include <stdio.h>
#include <stdlib.h>

#include "pre_processor.h"
#include "global_definitions.h"
#include "compilation.h"

int compile(char * fileName){


    /* Pre-process the file */
    preProcessFile(fileName);




    return EXIT_SUCCESS;
}