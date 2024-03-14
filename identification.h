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

CommandType identifyCommandType(char *line, HashTable* instructionsHash);
bool isComment(char *line);
bool isEmpty(char *line);
void cleanCommand(char *line);
bool isDirective(char *line);
CommandType identifyDirective(char *line);
bool isConstant(char *line);
bool hasLabel(char *line);
void removeLabel(char *line);

AddressingMode identifyAddressingMode(char *operand, HashTable *symbolsLabelsValuesHash, HashTable *entriesExternsHash);
