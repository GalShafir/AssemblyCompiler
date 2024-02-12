FILE* openFile(const char * fileName, const char * accessMode);
char *removeFileExtension(const char *inputFileName);
void removeWhiteSpaces(char *inputString);
char **splitString(const char *inputString, const char *separators, int *count);
void freeStringArray(char **splittedStrings, int count);
void printStringArray(char **strings, int count);
