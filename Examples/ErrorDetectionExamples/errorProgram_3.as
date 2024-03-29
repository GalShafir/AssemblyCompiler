.entry LIST
.extern W
.define sz = 2

MAIN:       mov  r3, LIST[bla]

LOOP:       jmp W
            prn #-5
            mov STR[8], STR[2]

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
STR2:        .string "abcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdef"
LIST:       .data 6, -9, len
K:          .data 22
.extern L3