#ifndef _GLOBAL_DEFINITION_H
#define _GLOBAL_DEFINITION_H


/* ------------------------------------ Static Definitions ------------------------------------ */


#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define MAX_LINE_LENGTH 81
#define MAX_LABEL_LENGTH 31



/* ------------------------------------ Data Types ------------------------------------ */

/* Defines a boolean type (T/F) */
typedef enum booleans {
    FALSE = 0,
    TRUE = 1
} bool;


/* ------------------------------------ Errors Collection ------------------------------------ */

#define EXECUTION_FORMAT_ERROR "Error while executing the program. Invalid format."
#define FILE_OPEN_ERROR "Error while opening the file."
#define MEMORY_ALLOCATION_ERROR "Error while allocating memory."










#endif