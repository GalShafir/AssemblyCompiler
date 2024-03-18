/** calculate_memory_addresses_for_instructions
 *  @brief This function calculates the memory addresses for the instructions
 *  @param commandType The type of the command
 *  @param line The line to be analyzed
 *  @param symbolsLabelsValuesHash The hash table containing the symbols, labels and their values
 *  @param directiveOrder The order of the directive
 *  @param entriesExternsHash The hash table containing the entries and externs
 *  @param currentMemoryAddress The current memory address
 */
void calculate_memory_addresses_for_instructions(CommandType commandType, char *line, HashTable *symbolsLabelsValuesHash, int * directiveOrder, HashTable *entriesExternsHash, int * currentMemoryAddress);

/** calculate_memory_addresses_for_directives
 *  @brief This function calculates the memory addresses for the directives
 *  @param commandType The type of the command
 *  @param line The line to be analyzed
 *  @param symbolsLabelsValuesHash The hash table containing the symbols, labels and their values
 *  @param directiveOrder The order of the directive
 *  @param entriesExternsHash The hash table containing the entries and externs
 *  @param currentMemoryAddress The current memory address
 */
void calculate_memory_addresses_for_directives(CommandType commandType, char *line, HashTable *symbolsLabelsValuesHash, int * directiveOrder, HashTable *entriesExternsHash, int * currentMemoryAddress);

/** build_binary_file
 *  @brief This function builds the binary file
 *  @param inputFileName The name of the input file
 *  @param symbolsLabelsValuesHash The hash table containing the symbols, labels and their values
 *  @param entriesExternsHash The hash table containing the entries and externs
 *  @param instructionsHash The hash table containing the instructions
 *  @param registersHash The hash table containing the registers
 */
void build_binary_file(char * inputFileName, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash, HashTable *instructionsHash, HashTable *registersHash);

/** build_encoded_file
 *  @brief This function builds the encoded file
 *  @param inputFileName The name of the input file
 *  @param symbolsLabelsValuesHash The hash table containing the symbols, labels and their values
 *  @param currentMemoryAddress The current memory address
 */
void build_encoded_file(char * inputFileName, HashTable *symbolsLabelsValuesHash, int currentMemoryAddress);

/** decimalToBinary
 *  @brief This function converts a decimal number to a binary number
 *  @param decimal The decimal number to be converted
 *  @param numBits The number of bits
 *  @param binary The binary number
 */
void decimalToBinary(int decimal, int numBits, char *binary);

/** calculate_data_directive_memory_address
 *  @brief This function calculates the memory address for the data directive
 *  @param line The line to be analyzed
 *  @param symbolsLabelsValuesHash The hash table containing the symbols, labels and their values
 *  @param directiveOrder The order of the directive
 *  @param currentMemoryAddress The current memory address
 */
void calculate_data_directive_memory_address(char * line, HashTable *symbolsLabelsValuesHash, int * directiveOrder, int * currentMemoryAddress);

/** calculate_string_directive_memory_address
 *  @brief This function calculates the memory address for the string directive
 *  @param line The line to be analyzed
 *  @param symbolsLabelsValuesHash The hash table containing the symbols, labels and their values
 *  @param directiveOrder The order of the directive
 *  @param currentMemoryAddress The current memory address
 */
void calculate_string_directive_memory_address(char * line, HashTable *symbolsLabelsValuesHash, int * directiveOrder, int * currentMemoryAddress);

/** calculate_instruction_memory_address
 *  @brief This function calculates the memory address for the instruction
 *  @param line The line to be analyzed
 *  @param symbolsLabelsValuesHash The hash table containing the symbols, labels and their values
 *  @param currentMemoryAddress The current memory address
 */
void calculate_instruction_memory_address(char * line, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash, int * currentMemoryAddress);

/** analyzeAddressingMode
 *  @brief This function analyzes the addressing mode
 *  @param operand The operand to be analyzed
 *  @param symbolsLabelsValuesHash The hash table containing the symbols, labels and their values
 *  @param entriesExternsHash The hash table containing the entries and externs
 *  @return The addressing mode
 */
AddressingMode analyzeAddressingMode(char *operand, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash);

/** analyze_data_directive
 *  @brief This function analyzes the data directive
 *  @param line The line to be analyzed
 *  @param symbolsLabelsValuesHash The hash table containing the symbols, labels and their values
 *  @param entriesExternsHash The hash table containing the entries and externs
 *  @param instructionsHash The hash table containing the instructions
 *  @param outputFile The output file
 *  @param entryFile The entry file
 *  @param externFile The extern file
 */
void analyze_data_directive(char * line, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash, HashTable *instructionsHash, FILE *outputFile, FILE *entryFile, FILE *externFile);

/** analyze_string_directive
 *  @brief This function analyzes the string directive
 *  @param line The line to be analyzed
 *  @param symbolsLabelsValuesHash The hash table containing the symbols, labels and their values
 *  @param entriesExternsHash The hash table containing the entries and externs
 *  @param instructionsHash The hash table containing the instructions
 *  @param outputFile The output file
 *  @param entryFile The entry file
 *  @param externFile The extern file
 */
void analyze_string_directive(char * line, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash, HashTable *instructionsHash, FILE *outputFile, FILE *entryFile, FILE *externFile);

/** analyze_instruction
 *  @brief This function analyzes the instruction
 *  @param line The line to be analyzed
 *  @param symbolsLabelsValuesHash The hash table containing the symbols, labels and their values
 *  @param entriesExternsHash The hash table containing the entries and externs
 *  @param instructionsHash The hash table containing the instructions
 *  @param registersHash The hash table containing the registers
 *  @param outputFile The output file
 *  @param entryFile The entry file
 *  @param externFile The extern file
 *  @param currentMemoryAddress The current memory address
 */
void analyze_instruction(char * line, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash, HashTable *instructionsHash, HashTable *registersHash, FILE *outputFile, FILE *entryFile, FILE *externFile, int * currentMemoryAddress);

/** get_imidiate_data
 *  @brief This function gets the imidiate data
 *  @param operand The operand to be analyzed
 *  @param symbolsLabelsValuesHash The hash table containing the symbols, labels and their values
 *  @param entriesExternsHash The hash table containing the entries and externs
 *  @return The imidiate data
 */
int get_imidiate_data(char *operand, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash);

/** get_label_address
 *  @brief This function gets the label address
 *  @param label The label to be analyzed
 *  @param symbolsLabelsValuesHash The hash table containing the symbols, labels and their values
 *  @param entriesExternsHash The hash table containing the entries and externs
 *  @return The label address
 */
int get_label_address(char *label, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash);

/** get_indexed_label_address
 *  @brief This function gets the indexed label address
 *  @param label The label to be analyzed
 *  @param symbolsLabelsValuesHash The hash table containing the symbols, labels and their values
 *  @param entriesExternsHash The hash table containing the entries and externs
 *  @param index The index
 *  @return The indexed label address
 */
int get_indexed_label_address(char *label, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash, int * index);

/** encodeBinaryString
 *  @brief This function encodes the binary string
 *  @param binaryString The binary string to be encoded
 *  @return The encoded binary string
 */
char* encodeBinaryString(const char* binaryString);

/** get_directives_memory_size
 *  @brief This function gets the directives memory size
 *  @param table The table to be analyzed
 * @return The directives memory size
 */
int get_directives_memory_size(HashTable *table);

/** entryExists
 *  @brief This function checks if the entry exists
 *  @param table The table to be analyzed
 * @return True if the entry exists, False otherwise
 */
bool entryExists(HashTable *table);

/** externExists
 *  @brief This function checks if the extern exists
 *  @param table The table to be analyzed
 * @return True if the extern exists, False otherwise
 */
bool externExists(HashTable *table);