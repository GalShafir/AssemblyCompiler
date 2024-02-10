#include <stdlib.h> 
#include <stdio.h>

#include "global_definitions.h"
#include "compilation.h"


int main(int argc, char *argv[])
{
    int i;
    if (argc < 2) {
        fprintf(stderr, EXECUTION_FORMAT_ERROR);
        fprintf(stderr, "Error while executing the program. Expected: %s program1.as program2.as ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (i = 1; i < argc; i++) {
        compile(argv[i]);
    }

    return 0;
}