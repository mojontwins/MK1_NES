; scripts.s
; Generado por Mojon Script Compiler de la Churrera NES
; Copyleft 2013/2014 The Mojon Twins
 
; Script data & pointers
 
.export _e_scripts
.export _f_scripts
 
mscce_0:

    .byte $07, $80, $00, $FF, $01, $01, $02, $FF, $FF

mscce_1:

    .byte $23, $10, $01, $02, $FF, $01, $01, $01, $E3, $00, $00, $00, $00, $37, $25, $2C
    .byte $23, $2F, $2D, $25, $00, $34, $2F, $00, $24, $21, $2C, $25, $26, $32, $21, $2E
    .byte $29, $21, $EE, $FF, $FF

 
_e_scripts:
    .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, mscce_1, 0, 0, 0, mscce_0, 0, 0
 
_f_scripts:
    .word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
 
