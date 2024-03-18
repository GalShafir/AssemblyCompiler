void check_errors(CommandType commandType, char *line, int lineNumber, char * fileName, HashTable *symbolsLabelsValuesHash, int * directiveOrder, HashTable *entriesExternsHash, bool * foundError);
void check_entries_externs_errors(CommandType commandType, char *line, int lineNumber, char * fileName, HashTable *entriesExternsHash, HashTable *symbolsLabelsValuesHash, bool * foundError);
void check_instruction_errors(CommandType commandType, char *line, int lineNumber, char * fileName, HashTable *entriesExternsHash, HashTable *symbolsLabelsValuesHash, bool * foundError);
void print_error(char *error, const char *line, int lineNumber, char *fileName);
bool hasSomethingAfterSection(const char *input, const char *section);
bool checkCommas(const char* line, const char* originalLine, int lineNumber, char * fileName);
bool checkQuotes(const char* line, const char* originalLine, int lineNumber, char * fileName);
bool checkLabelLength(const char* label);
bool checkLabelName(const char* label);
void check_constant_error(char * line, int lineNumber, char * fileName, HashTable *symbolsLabelsValuesHash, bool * foundError);
void check_data_directive_error(char * line, int lineNumber, char * fileName, HashTable *symbolsLabelsValuesHash, int * directiveOrder, bool * foundError);
void check_string_directive_error(char * line, int lineNumber, char * fileName, HashTable *symbolsLabelsValuesHash, int * directiveOrder, bool * foundError);
void check_entry_directive_error(char * line, int lineNumber, char * fileName, HashTable *entriesExternsHash, HashTable *symbolsLabelsValuesHash, bool * foundError);
void temp_instruction_addition(char * line, int lineNumber, char * fileName, HashTable *entriesExternsHash, HashTable *symbolsLabelsValuesHash, bool * foundError);
void check_extern_directive_error(char * line, int lineNumber, char * fileName, HashTable *entriesExternsHash, bool * foundError);
void check_instruction_error(char * line, int lineNumber, char * fileName, HashTable *entriesExternsHash, HashTable *symbolsLabelsValuesHash, bool * foundError);
bool number_of_operands_is_valid(char *instructionName, char *instructionContent);
bool checkInstructionCommas(const char* line, const char* originalLine, int lineNumber, char * fileName);
bool checkCommandExists(char * instruction, const char* originalLine, int lineNumber, char * fileName);
size_t stringLength(const char *str);
bool checkIfLineLengthValid(char * line, int lineNumber, char * fileName);
