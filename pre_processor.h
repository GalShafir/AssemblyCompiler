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


void processMacros(const char *inputFileName);
int preProcessFile(char * fileName);
void printMacros(const Macro *macros, int macroCount);


