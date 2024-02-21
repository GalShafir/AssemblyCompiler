void check_errors(CommandType commandType, char *line, HashTable *symbolsLabelsValuesHash);
void print_error(char *error, const char *line);
bool hasSomethingAfterSection(const char *input, const char *section);
bool isValidInteger(const char *str);
bool checkCommas(const char* line, const char* originalLine);
void check_constant_error(char *line, HashTable *symbolsLabelsValuesHash);
void check_data_directive_error(char * line, HashTable *symbolsLabelsValuesHash);
