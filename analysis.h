void calculate_memory_addresses(CommandType commandType, char *line, HashTable *symbolsLabelsValuesHash, int * directiveOrder, HashTable *entriesExternsHash, int * currentMemoryAddress);
void build_binary_file(char * inputFileName, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash, HashTable *instructionsHash);
void decimalToBinary(int decimal, int numBits, char *binary);
void calculate_data_directive_memory_address(char * line, HashTable *symbolsLabelsValuesHash, int * directiveOrder, int * currentMemoryAddress);
void calculate_string_directive_memory_address(char * line, HashTable *symbolsLabelsValuesHash, int * directiveOrder, int * currentMemoryAddress);
void calculate_instruction_memory_address(char * line, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash, int * currentMemoryAddress);
AddressingMode analyzeAddressingMode(char *operand, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash);
void analyze_data_directive(char * line, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash, HashTable *instructionsHash, FILE *outputFile);
