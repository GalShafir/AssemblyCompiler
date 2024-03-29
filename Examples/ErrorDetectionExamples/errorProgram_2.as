.entry LIST
.extern W
.define sz = 2
MAIN:       mov  r3, LIST[sz]
W:          cmp r3, #sz
LOOP:       jmp W
            prn #-5
            ; A comment
            sub r1, r4
            cmp K, #sz
            cmp r3, #sz
            ; The next line is empty comment
