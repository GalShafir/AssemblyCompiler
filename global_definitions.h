#ifndef _GLOBAL_DEFINITION_H
#define _GLOBAL_DEFINITION_H


/* ------------------------------------ Static Definitions ------------------------------------ */


#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define MAX_LINE_LENGTH 81

/* The max length in 31 but in order to include /0 we set it to 32*/
#define MAX_LABEL_LENGTH 32

/* Size of the HashTable. Determines the number of slots or buckets in the hash table. Not the actual memory consumption */
#define HT_CAPACITY 5000

#define STARTING_MEMORY_LOCATION 100


/* ------------------------------------ Data Types ------------------------------------ */

/* Defines a boolean type (T/F) */
typedef enum booleans {
    False = 0,
    True = 1
} bool;


/* ------------------------------------ Errors Collection ------------------------------------ */

#define EXECUTION_FORMAT_ERROR "Error while executing the program. Invalid format."
#define FILE_OPEN_ERROR "Error while opening the file."
#define MEMORY_ALLOCATION_ERROR "Error while allocating memory."










#endif