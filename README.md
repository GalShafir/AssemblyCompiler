## Description
Special Assembly compiler (Assembler) - written in C
It analyzes the input file and generates a machine code file.
The machine code is an encoded version of the assembly code.
In addition the assembler generates two more files, an externs file and an entry file.
The externs file contains the names of the external variables and their addresses.
The entry file contains the names of the entry variables and their addresses.

## Error Handling
The assembler checks for errors in the input file and prints the error message to the screen.
The assembler stops the compilation process if an error is found.

## Usage
```bash
$ make
$ ./assembler <input_file1> <input_file2> ...
```

## Example
```bash
$ ./assembler test.as
```

