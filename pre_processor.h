#include "global_definitions.h"

/* ------------------------------------ Static Definitions ------------------------------------ */

#define MAX_MACROS 20


/* ------------------------------------ Data Structures ------------------------------------ */


/* Structure to store macro information */
typedef struct {
    char name[MAX_LINE_LENGTH];
    char *content;
} Macro;

/* ------------------------------------ Functions ------------------------------------ */


/** processMacros
 * @brief Processes the macros in the given file.
 * 
 * @param inputFileName The name of the file to process.
 */
void processMacros(char *inputFileName);

/** preProcessFile
 * @brief Pre-processes the given file.
 * 
 * @param fileName The name of the file to pre-process.
 * @return int 0 if the pre-processing was successful, 1 otherwise.
 */
int preProcessFile(char * fileName);

/** printMacros
 * @brief Prints the macros.
 * 
 * @param macros The macros to print.
 * @param macroCount The number of macros.
 */
void printMacros(const Macro *macros, int macroCount);


