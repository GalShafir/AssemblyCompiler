/** check_errors - Check for syntax errors in the input assembly file.
 * @param commandType - The type of the command to be checked.
 * @param line - The input string to be checked.
 * @param lineNumber - The line number where the error occurred.
 * @param fileName - The name of the file where the error occurred.
 * @param symbolsLabelsValuesHash - The hash table to store the symbols-labels values.
 * @param directiveOrder - The order of the directive in the file.
 * @param entriesExternsHash - The hash table to store the entries and externs.
 * @param foundError - A flag to indicate if an error was found.
 */
void check_errors(CommandType commandType, char *line, int lineNumber, char * fileName, HashTable *symbolsLabelsValuesHash, int * directiveOrder, HashTable *entriesExternsHash, bool * foundError);

/** check_entries_externs_errors - Check for syntax errors in the input assembly file for entries and externs.
 * @param commandType - The type of the command to be checked.
 * @param line - The input string to be checked.
 * @param lineNumber - The line number where the error occurred.
 * @param fileName - The name of the file where the error occurred.
 * @param entriesExternsHash - The hash table to store the entries and externs.
 * @param symbolsLabelsValuesHash - The hash table to store the symbols-labels values.
 * @param foundError - A flag to indicate if an error was found.
 */
void check_entries_externs_errors(CommandType commandType, char *line, int lineNumber, char * fileName, HashTable *entriesExternsHash, HashTable *symbolsLabelsValuesHash, bool * foundError);

/** check_instruction_errors - Check for syntax errors in the input assembly file for instructions.
 * @param commandType - The type of the command to be checked.
 * @param line - The input string to be checked.
 * @param lineNumber - The line number where the error occurred.
 * @param fileName - The name of the file where the error occurred.
 * @param entriesExternsHash - The hash table to store the entries and externs.
 * @param symbolsLabelsValuesHash - The hash table to store the symbols-labels values.
 * @param foundError - A flag to indicate if an error was found.
 */
void check_instruction_errors(CommandType commandType, char *line, int lineNumber, char * fileName, HashTable *entriesExternsHash, HashTable *symbolsLabelsValuesHash, bool * foundError);

/** print_error - Print the error message to the console.
 * @param error - The error message to be printed.
 * @param line - The input string where the error occurred.
 * @param lineNumber - The line number where the error occurred.
 * @param fileName - The name of the file where the error occurred.
 */
void print_error(char *error, const char *line, int lineNumber, char *fileName);

/** hasSomethingAfterSection - Check if there is something after the section.
 * @param input - The input string to be checked.
 * @param section - The section to be checked.
 * @return bool - True if there is something after the section, false otherwise.
 */
bool hasSomethingAfterSection(const char *input, const char *section);

/** checkCommas - Check for syntax errors in the input assembly file for commas.
 * @param line - The input string to be checked.
 * @param originalLine - The original input string to be checked.
 * @param lineNumber - The line number where the error occurred.
 * @param fileName - The name of the file where the error occurred.
 * @return bool - True if there is a syntax error, false otherwise.
 */
bool checkCommas(const char* line, const char* originalLine, int lineNumber, char * fileName);

/** checkQuotes - Check for syntax errors in the input assembly file for quotes.
 * @param line - The input string to be checked.
 * @param originalLine - The original input string to be checked.
 * @param lineNumber - The line number where the error occurred.
 * @param fileName - The name of the file where the error occurred.
 * @return bool - True if there is a syntax error, false otherwise.
 */
bool checkQuotes(const char* line, const char* originalLine, int lineNumber, char * fileName);

/** checkLabelLength - Check for syntax errors in the input assembly file for label length.
 * @param label - The label to be checked.
 * @return bool - True if there is a syntax error, false otherwise.
 */
bool checkLabelLength(const char* label);

/** checkLabelName - Check for syntax errors in the input assembly file for label name.
 * @param label - The label to be checked.
 * @return bool - True if there is a syntax error, false otherwise.
 */
bool checkLabelName(const char* label);


/** check_constant_error - Check for syntax errors in the input assembly file for constants.
 * @param line - The input string to be checked.
 * @param lineNumber - The line number where the error occurred.
 * @param fileName - The name of the file where the error occurred.
 * @param symbolsLabelsValuesHash - The hash table to store the symbols-labels values.
 * @param foundError - A flag to indicate if an error was found.
 */
void check_constant_error(char * line, int lineNumber, char * fileName, HashTable *symbolsLabelsValuesHash, bool * foundError);

/** check_data_directive_error - Check for syntax errors in the input assembly file for data directives.
 * @param line - The input string to be checked.
 * @param lineNumber - The line number where the error occurred.
 * @param fileName - The name of the file where the error occurred.
 * @param symbolsLabelsValuesHash - The hash table to store the symbols-labels values.
 * @param directiveOrder - The order of the directive in the file.
 * @param foundError - A flag to indicate if an error was found.
 */
void check_data_directive_error(char * line, int lineNumber, char * fileName, HashTable *symbolsLabelsValuesHash, int * directiveOrder, bool * foundError);

/** check_string_directive_error - Check for syntax errors in the input assembly file for string directives.
 * @param line - The input string to be checked.
 * @param lineNumber - The line number where the error occurred.
 * @param fileName - The name of the file where the error occurred.
 * @param symbolsLabelsValuesHash - The hash table to store the symbols-labels values.
 * @param directiveOrder - The order of the directive in the file.
 * @param foundError - A flag to indicate if an error was found.
 */
void check_string_directive_error(char * line, int lineNumber, char * fileName, HashTable *symbolsLabelsValuesHash, int * directiveOrder, bool * foundError);

/** check_entry_directive_error - Check for syntax errors in the input assembly file for entry directives.
 * @param line - The input string to be checked.
 * @param lineNumber - The line number where the error occurred.
 * @param fileName - The name of the file where the error occurred.
 * @param entriesExternsHash - The hash table to store the entries and externs.
 * @param symbolsLabelsValuesHash - The hash table to store the symbols-labels values.
 * @param foundError - A flag to indicate if an error was found.
 */
void check_entry_directive_error(char * line, int lineNumber, char * fileName, HashTable *entriesExternsHash, HashTable *symbolsLabelsValuesHash, bool * foundError);

/** temp_instruction_addition - Add the instruction to the hash table.
 * @param line - The input string to be checked.
 * @param lineNumber - The line number where the error occurred.
 * @param fileName - The name of the file where the error occurred.
 * @param entriesExternsHash - The hash table to store the entries and externs.
 * @param symbolsLabelsValuesHash - The hash table to store the symbols-labels values.
 * @param foundError - A flag to indicate if an error was found.
 */
void temp_instruction_addition(char * line, int lineNumber, char * fileName, HashTable *entriesExternsHash, HashTable *symbolsLabelsValuesHash, bool * foundError);

/** check_extern_directive_error - Check for syntax errors in the input assembly file for extern directives.
 * @param line - The input string to be checked.
 * @param lineNumber - The line number where the error occurred.
 * @param fileName - The name of the file where the error occurred.
 * @param entriesExternsHash - The hash table to store the entries and externs.
 * @param foundError - A flag to indicate if an error was found.
 */
void check_extern_directive_error(char * line, int lineNumber, char * fileName, HashTable *entriesExternsHash, bool * foundError);

/** check_instruction_error - Check for syntax errors in the input assembly file for instructions.
 * @param line - The input string to be checked.
 * @param lineNumber - The line number where the error occurred.
 * @param fileName - The name of the file where the error occurred.
 * @param entriesExternsHash - The hash table to store the entries and externs.
 * @param symbolsLabelsValuesHash - The hash table to store the symbols-labels values.
 * @param foundError - A flag to indicate if an error was found.
 */
void check_instruction_error(char * line, int lineNumber, char * fileName, HashTable *entriesExternsHash, HashTable *symbolsLabelsValuesHash, bool * foundError);

/** number_of_operands_is_valid - Check if the number of operands is valid.
 * @param instructionName - The name of the instruction.
 * @param instructionContent - The content of the instruction.
 * @return bool - True if the number of operands is valid, false otherwise.
 */
bool number_of_operands_is_valid(char *instructionName, char *instructionContent);

/** check_instruction_commas - Check for syntax errors in the input assembly file for instruction commas.
 * @param line - The input string to be checked.
 * @param originalLine - The original input string to be checked.
 * @param lineNumber - The line number where the error occurred.
 * @param fileName - The name of the file where the error occurred.
 * @return bool - True if there is a syntax error, false otherwise.
 */
bool checkInstructionCommas(const char* line, const char* originalLine, int lineNumber, char * fileName);

/** checkCommandExists - Check if the command exists.
 * @param instruction - The instruction to be checked.
 * @param originalLine - The original input string to be checked.
 * @param lineNumber - The line number where the error occurred.
 * @param fileName - The name of the file where the error occurred.
 * @return bool - True if the command exists, false otherwise.
 */
bool checkCommandExists(char * instruction, const char* originalLine, int lineNumber, char * fileName);

/** stringLength - Get the length of the string.
 * @param str - The string to be checked.
 * @return size_t - The length of the string.
 */
size_t stringLength(const char *str);

/** checkIfLineLengthValid - Check if the line length is valid.
 * @param line - The input string to be checked.
 * @param lineNumber - The line number where the error occurred.
 * @param fileName - The name of the file where the error occurred.
 * @return bool - True if the line length is valid, false otherwise.
 */
bool checkIfLineLengthValid(char * line, int lineNumber, char * fileName);
