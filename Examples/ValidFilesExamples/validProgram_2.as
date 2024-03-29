.extern W
.define sz = 2

LOOP:       jmp W
            prn #-5
            mov STR[5], STR[2]
            mcr m_mcr
                cmp r3, #sz
            bne END
            endmcr
            ; A comment
            sub r1, r4

            m_mcr

            ; The next line is empty comment

            bne W
            

STR:        .string "abcdef"
.entry LOOP
            bne LOOP
END:        hlt
