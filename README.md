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
$ ./assembler Examples/ValidFilesExamples/validProgram1.as
```
The files look like this:
```bash
$ cat Examples/ValidFilesExamples/validProgram1.as
```

```assembly
.entry LIST
.extern W
.define sz = 2

MAIN:       mov  r3, LIST[sz]

LOOP:       jmp W
            prn #-5
            mov STR[5], STR[2]

            ; A comment
            sub r1, r4
            cmp K, #sz
            ; The next line is an empty comment

            bne W
L1: inc L3
.entry LOOP
            bne LOOP
END:        hlt
.define len = 4

STR:        .string "abcdef"
LIST:       .data 6, -9, len
K:          .data 22
.extern L3
```

The files generated by the assembler will be in the same directory as the input file.
The encoded file will be named as the input file with the extension .ob
The externs file will be named as the input file with the extension .ext
The entry file will be named as the input file with the extension .ent
The files look like this:
```bash
$ cat Examples/ValidFilesExamples/validProgram1.ob
```

```bash

  25 11
0100 ****!%*
0101 ***#%**
0102 **%*#*%
0103 *****%*
0104 **%#*#*
0105 ******#
0106 **!****
0107 !!!!%!*
0108 ****%%*
0109 **#!!#%
0110 ****##*
0111 **#!!#%
0112 *****%*
0113 ***!!!*
0114 ****!**
0115 ***##**
0116 **%*#!%
0117 *****%*
0118 **%%*#*
0119 ******#
0120 **#!*#*
0121 ******#
0122 **%%*#*
0123 **#%%*%
0124 **!!***
0125 ***#%*#
0126 ***#%*%
0127 ***#%*!
0128 ***#%#*
0129 ***#%##
0130 ***#%#%
0131 *******
0132 *****#%
0133 !!!!!#!
0134 *****#*
0135 ****##%


```

```bash

$ cat Examples/ValidFilesExamples/validProgram1.ext
```

```bash

W 0105
W 0119
L3 0121

```

```bash

$ cat Examples/ValidFilesExamples/validProgram1.ent
```

```bash

LOOP 0104
LIST 0132


```
The assembler can also recognize errors in the input file.
Errors can look like:


![screenshot](Examples/ErrorDetectionExamples/errorsProgram_1.png)


