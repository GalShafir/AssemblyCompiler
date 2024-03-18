.entry LIST
.extern W
.define sz = 2
    .define     aaa     =    -30
MAIN:       mov  r3, LIST[sz]

LOOP:       jmp W
            prn #-5
            mov STR[5], STR[2]

            ; A comment
            sub r1, r4
            cmp K, #sz
            ; The next line is an empty comment

            mcr m_mcr
                cmp r3, #sz
            bne END
            endmcr

            bne W
            m_mcr
L1: inc L3
.entry LOOP
            bne LOOP
END:        hlt
m_mcr
.define len = 4
STR2:        .string "abcdef"
STR:        .string "abcdef"
LIST:       .data 6, -9, len
K:          .data 22
.extern L3