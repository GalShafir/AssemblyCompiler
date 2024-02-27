#include <stdlib.h> 
#include <stdio.h>

#include "global_definitions.h"
#include "compilation.h"


int main(int argc, char *argv[])
{
    int i;
    if (argc < 2) {
        fprintf(stderr, EXECUTION_FORMAT_ERROR);
        fprintf(stderr, "\nExpected: %s program1 program2 ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (i = 1; i < argc; i++) {
        compile(argv[i]);
    }

    return 0;
}