.entry LIST
.extern W
.define sz = 2
MAIN:       mov  r3, LIST[sz]

LOOP:       jmp W
            prn #-5
            mov STR[5], STR[2]
            mcr m_mcr
                cmp r3, #sz
            bne END
            endmcr
            sub r1, r4
            cmp K, #sz
            mcr nana
                        blabla
                        hello      hello
            endmcr
            m_mcr
            nana
            bne W
            nana
L1: inc L3
.entry LOOP
            bne LOOP
END:        hlt

.define len = 4
STR:        .string “abcdef”
LIST:       .data 6, -9, len
K:          .data 22
.extern L3 