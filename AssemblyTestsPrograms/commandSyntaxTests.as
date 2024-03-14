.entry LIST
.extern W
.define sz = 2
    .define     aaa     =    -30
MAIN:       mov  r3, LIST[sz]

LOOP:       jmp W
            prn #-5
            mov STR[5], STR[2]
            mcr m_mcr
                cmp r3, #sz
            bne END
            endmcr
            ; A comment
            sub r1, r4
            cmp K, #sz
            m_mcr
            ; The next line is an empty comment

            bne W
            
L1: inc L3
.entry LOOP
            bne LOOP
END:        hlt

.define len = 4
STR:        .string "abcdef"
PPP: .string            "eefwedf   wef      333 3"
.string "   "
.string "sss
LIST:       .data 6, -9, len
K:          .data 22
.extern L3




