typedef enum CommandType {
    UNDEFINED,
    EMPTY,
    COMMENT,
    DATA_DIRECTIVE,
    STRING_DIRECTIVE,
    ENTRY_DIRECTIVE,
    EXTERN_DIRECTIVE,
    UNDEFINED_DIRECTIVE,
    INSTRUCTION,
    CONSTANT

} CommandType;

typedef enum InstructionType {
    MOV,
    CMP,
    ADD,
    SUB,
    NOT,
    CLR,
    LEA,
    INC,
    DEC,
    JMP,
    BNE,
    RED,
    PRN,
    JSR,
    RTS,
    HLT,
    UNDEFINED_INSTRUCTION
} InstructionType;

typedef enum AddressingMode {

    IMMEDIATE,
    DIRECT,
    INDEX,
    REGISTER,
    UNDEFINED_ADDRESSING,
    UNDEFINED_CONSTANT,
    UNDEFINED_LABEL,
    INDEX_OVERFLOW

} AddressingMode;


/* ------------------------------------ Functions ------------------------------------ */

/** identifyCommandType
 *  @brief Identifies the type of command in the given line.
 *  @param line The line to be analyzed.
 *  @param instructionsHash The hash table containing the instructions.
 *  @return The type of command in the given line.
 */
CommandType identifyCommandType(char *line, HashTable* instructionsHash);

/** isComment
 *  @brief Checks if the given line is a comment.
 *  @param line The line to be analyzed.
 *  @return True if the given line is a comment, False otherwise.
 */
bool isComment(char *line);

/** isEmpty
 *  @brief Checks if the given line is empty.
 *  @param line The line to be analyzed.
 *  @return True if the given line is empty, False otherwise.
 */
bool isEmpty(char *line);

/** cleanCommand
 *  @brief Cleans the given line from any unnecessary characters.
 *  @param line The line to be cleaned.
 */
void cleanCommand(char *line);

/** isDirective
 *  @brief Checks if the given line is a directive.
 *  @param line The line to be analyzed.
 *  @return True if the given line is a directive, False otherwise.
 */
bool isDirective(char *line);

/** identifyDirective
 *  @brief Identifies the type of directive in the given line.
 *  @param line The line to be analyzed.
 *  @return The type of directive in the given line.
 */
CommandType identifyDirective(char *line);

/** IsConstant
 *  @brief Checks if the given line is a constant.
 *  @param line The line to be analyzed.
 *  @return True if the given line is a constant, False otherwise.
 */
bool isConstant(char *line);

/** hasLabel
 *  @brief Checks if the given line has a label.
 *  @param line The line to be analyzed.
 *  @return True if the given line has a label, False otherwise.
 */
bool hasLabel(char *line);

/** removeLabel
 *  @brief Removes the label from the given line.
 *  @param line The line to be analyzed.
 */
void removeLabel(char *line);


/** identifyAddressingMode
 *  @brief Identifies the addressing mode of the given operand.
 *  @param operand The operand to be analyzed.
 *  @param symbolsLabelsValuesHash The hash table containing the symbols, labels and values.
 *  @param entriesExternsHash The hash table containing the entries and externs.
 *  @return The addressing mode of the given operand.
 */
AddressingMode identifyAddressingMode(char *operand, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash);
