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
                cmp r3, #sz
                        blabla
            ; The next line is empty comment

            bne W

.define len = 4
STR:        .string "abcdef"
LIST:       .data 6, -9, len
K:          .data 22
            .data 30,-50
            .data       1,2,3,

STR:        .string "aaa"
.define sz = 4
.define variable = y

.data       1,,2,3,



