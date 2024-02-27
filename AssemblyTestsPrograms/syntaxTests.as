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
            ; A comment
            sub r1, r4
            cmp K, #sz
            mcr nana
                        blabla
                        hello      hello
            endmcr
            m_mcr
            nana
            ; The next line is empty comment

            bne W
            
            nana
L1: inc L3
.entry LOOP
            bne LOOP
END:        hlt
.data ,1,-2,+3
.data 1,5,,6
.string "abc"

.define len = 4
STR:        .string "abcdef"
LIST:       .data 6, -9, len
K:          .data 22
            .data 30,-50
            .data       1,2,3,

TTT:        .data
.data       
.data           t       
    .data
.data       1,2,3,
A: B: .data 5,6
.define a = 5
    .define aaa=+6
        .define b     =     -6
.define d = 4
.define len = 4
.define
.define c  = -600001
.define sz = 100
.data 1,2,3
AAA:    .data      1,-2,3
BBB:     .data      1,a,c
.data ,1,2
.data 1,,2
.data 1,a,2
.data 1,-2,
AAA: .data 1
CCC: .data 1
QQQ: .define 4 = 3
EEE: .data 1,-2,+3,     
AAA: .string "abc"
    .string "   d   de"
RRR:            .string         "a1b2"
XXX: .string fff
.string "aaa
.string bbb"

.extern L3 