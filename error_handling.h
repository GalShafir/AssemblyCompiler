void check_errors(CommandType commandType, char *line, int lineNumber, char * fileName, HashTable *symbolsLabelsValuesHash, int * directiveOrder);
void print_error(char *error, const char *line, int lineNumber, char *fileName);
bool hasSomethingAfterSection(const char *input, const char *section);
bool isValidInteger(const char *str);
bool checkCommas(const char* line, const char* originalLine, int lineNumber, char * fileName);
bool checkQuotes(const char* line, const char* originalLine, int lineNumber, char * fileName);
void check_constant_error(char * line, int lineNumber, char * fileName, HashTable *symbolsLabelsValuesHash);
void check_data_directive_error(char * line, int lineNumber, char * fileName, HashTable *symbolsLabelsValuesHash, int * directiveOrder);
void check_string_directive_error(char * line, int lineNumber, char * fileName, HashTable *symbolsLabelsValuesHash, int * directiveOrder);