;this file for FamiTone2 library generated by text2data tool

music_music_data:
	.byte 3
	.word @instruments
	.word @samples-3
	.word @song0ch0,@song0ch1,@song0ch2,@song0ch3,@song0ch4,307,256
	.word @song1ch0,@song1ch1,@song1ch2,@song1ch3,@song1ch4,307,256
	.word @song2ch0,@song2ch1,@song2ch2,@song2ch3,@song2ch4,307,256

@instruments:
	.byte $30 ;instrument $00
	.word @env1,@env0,@env16
	.byte $00
	.byte $30 ;instrument $04
	.word @env3,@env10,@env0
	.byte $00
	.byte $30 ;instrument $06
	.word @env2,@env11,@env0
	.byte $00
	.byte $30 ;instrument $08
	.word @env4,@env12,@env0
	.byte $00
	.byte $30 ;instrument $09
	.word @env5,@env0,@env0
	.byte $00
	.byte $30 ;instrument $0a
	.word @env2,@env9,@env0
	.byte $00
	.byte $30 ;instrument $0b
	.word @env2,@env13,@env0
	.byte $00
	.byte $30 ;instrument $0d
	.word @env6,@env0,@env15
	.byte $00
	.byte $30 ;instrument $0e
	.word @env7,@env14,@env0
	.byte $00
	.byte $30 ;instrument $0f
	.word @env8,@env0,@env0
	.byte $00

@samples:
@env0:
	.byte $c0,$00,$00
@env1:
	.byte $cf,$00,$00
@env2:
	.byte $c6,$c6,$c5,$c4,$c3,$09,$c2,$08,$c1,$0f,$c0,$00,$0a
@env3:
	.byte $c8,$c6,$00,$01
@env4:
	.byte $c5,$c5,$c4,$c4,$c3,$c0,$00,$05
@env5:
	.byte $cc,$ca,$c9,$c7,$c6,$c5,$c4,$c4,$c3,$c3,$c2,$02,$c1,$03,$c9,$c7
	.byte $c6,$c5,$c4,$c3,$02,$c2,$03,$c1,$02,$c4,$c3,$c3,$c2,$03,$c1,$04
	.byte $c0,$00,$20
@env6:
	.byte $c1,$c2,$c4,$c5,$00,$03
@env7:
	.byte $cb,$c8,$c6,$c4,$c3,$c2,$c1,$c0,$00,$07
@env8:
	.byte $cb,$c9,$c7,$c5,$c4,$c2,$c1,$c1,$c0,$00,$08
@env9:
	.byte $c0,$02,$c3,$02,$c7,$c7,$c7,$00,$00
@env10:
	.byte $c6,$c3,$c0,$00,$02
@env11:
	.byte $c0,$02,$c4,$02,$c7,$c7,$c7,$00,$00
@env12:
	.byte $c6,$c6,$c0,$00,$02
@env13:
	.byte $c0,$02,$c3,$02,$c8,$c8,$c8,$00,$00
@env14:
	.byte $c7,$c3,$c0,$bd,$00,$02
@env15:
	.byte $c4,$00,$00
@env16:
	.byte $c0,$0b,$c1,$c2,$c3,$c4,$c3,$c2,$c1,$c0,$00,$02


@song0ch0:
	.byte $fb,$10
@song0ch0loop:
@ref0:
	.byte $88,$58,$83,$4e,$83,$52,$91,$60,$83,$5c,$83,$58,$91
@ref1:
	.byte $4e,$83,$60,$83,$4f,$52,$8d,$58,$83,$4e,$83,$52,$91
@ref2:
	.byte $58,$83,$4e,$83,$52,$91,$60,$83,$5c,$83,$58,$91
	.byte $ff,$0d
	.word @ref1
@ref4:
	.byte $58,$89,$5c,$52,$8f,$62,$89,$60,$5c,$83,$61,$63,$66,$81
@ref5:
	.byte $60,$89,$4e,$52,$8d,$4e,$53,$53,$59,$60,$4e,$8f
	.byte $ff,$0c
	.word @ref2
	.byte $ff,$0d
	.word @ref1
	.byte $ff,$0c
	.word @ref2
	.byte $ff,$0d
	.word @ref1
	.byte $ff,$0e
	.word @ref4
	.byte $ff,$0c
	.word @ref5
@ref12:
	.byte $00,$9d
@ref13:
	.byte $9f
@ref14:
	.byte $82,$4e,$4e,$4a,$4a,$4e,$52,$54,$52,$4e,$4e,$4a,$4a,$4e,$52,$5c
	.byte $5e
@ref15:
	.byte $4e,$4e,$4a,$4a,$4e,$54,$52,$50,$4e,$4e,$4a,$4a,$4e,$52,$5c,$5e
@ref16:
	.byte $88,$58,$83,$59,$5e,$5c,$58,$54,$83,$54,$83,$4f,$5e,$83,$5e,$83
	.byte $4f,$5c,$83,$5c,$83,$4e,$81
@ref17:
	.byte $58,$83,$58,$83,$53,$54,$83,$54,$83,$4f,$52,$83,$52,$83,$4d,$4e
	.byte $83,$4e,$83,$46,$40
@ref18:
	.byte $58,$83,$59,$5e,$5c,$58,$54,$83,$54,$83,$4f,$5e,$83,$5e,$83,$4f
	.byte $5c,$83,$5c,$83,$4e,$81
	.byte $ff,$15
	.word @ref17
	.byte $fd
	.word @song0ch0loop

@song0ch1:
@song0ch1loop:
@ref20:
	.byte $00,$bd
@ref21:
	.byte $bf
@ref22:
	.byte $00,$bd
@ref23:
	.byte $bf
@ref24:
	.byte $88,$66,$89,$6a,$60,$8f,$6a,$89,$66,$66,$8f
@ref25:
	.byte $66,$89,$4e,$60,$8d,$5c,$5d,$61,$63,$66,$5c,$8f
@ref26:
	.byte $00,$bd
@ref27:
	.byte $bf
@ref28:
	.byte $00,$bd
@ref29:
	.byte $bf
@ref30:
	.byte $66,$89,$6a,$60,$8f,$6a,$89,$66,$66,$8f
	.byte $ff,$0c
	.word @ref25
@ref32:
	.byte $8a,$4f,$4e,$4e,$83,$37,$4f,$4e,$4e,$83,$36,$81
@ref33:
	.byte $8c,$4f,$4e,$4e,$83,$37,$8a,$4f,$4e,$4e,$83,$36,$81
@ref34:
	.byte $4f,$4e,$4e,$83,$37,$4f,$4e,$4e,$83,$36,$81
	.byte $ff,$0b
	.word @ref33
@ref36:
	.byte $84,$51,$50,$51,$38,$51,$8a,$4f,$4e,$4f,$36,$4f,$84,$47,$46,$47
	.byte $5e,$47,$4f,$4e,$4f,$36,$4e,$81
@ref37:
	.byte $8a,$59,$58,$59,$40,$59,$4f,$4e,$4f,$36,$4f,$84,$45,$44,$45,$5c
	.byte $45,$8a,$4f,$36,$4f,$36,$4e,$36
	.byte $ff,$15
	.word @ref36
	.byte $ff,$15
	.word @ref37
	.byte $fd
	.word @song0ch1loop

@song0ch2:
@song0ch2loop:
@ref40:
	.byte $80,$28,$83,$40,$83,$22,$85,$3a,$22,$3a,$85,$28,$83,$40,$83,$22
	.byte $85,$3a,$22,$3a,$85
@ref41:
	.byte $28,$83,$40,$83,$22,$85,$3a,$22,$3a,$85,$28,$83,$40,$83,$22,$85
	.byte $3a,$40,$3a,$85
@ref42:
	.byte $28,$83,$40,$83,$22,$85,$3a,$22,$3a,$85,$28,$83,$40,$83,$22,$85
	.byte $3a,$22,$3a,$85
	.byte $ff,$14
	.word @ref41
@ref44:
	.byte $29,$31,$37,$40,$3a,$83,$37,$31,$29,$2d,$33,$3b,$44,$3e,$83,$37
	.byte $49,$44,$81
	.byte $ff,$13
	.word @ref44
	.byte $ff,$14
	.word @ref42
	.byte $ff,$14
	.word @ref41
	.byte $ff,$14
	.word @ref42
	.byte $ff,$14
	.word @ref41
	.byte $ff,$13
	.word @ref44
	.byte $ff,$13
	.word @ref44
@ref52:
	.byte $36,$89,$3d,$36,$89,$3c,$81
@ref53:
	.byte $46,$89,$3d,$36,$89,$32,$81
	.byte $ff,$07
	.word @ref52
	.byte $ff,$07
	.word @ref53
@ref56:
	.byte $38,$89,$28,$2e,$36,$89,$24,$2c,$2e,$89,$24,$2e,$36,$8d
@ref57:
	.byte $28,$83,$1e,$28,$1e,$2e,$28,$24,$83,$1e,$24,$1e,$2c,$24,$22,$83
	.byte $2c,$34,$2c,$22,$1c,$1e,$83,$2d,$1e,$83
	.byte $ff,$0e
	.word @ref56
	.byte $ff,$1a
	.word @ref57
	.byte $fd
	.word @song0ch2loop

@song0ch3:
@song0ch3loop:
@ref60:
	.byte $86,$1b,$08,$1a,$83,$09,$1b,$08,$1a,$83,$09,$1b,$08,$1a,$83,$09
	.byte $1b,$08,$1b,$08,$08,$81
@ref61:
	.byte $1b,$08,$1a,$83,$09,$1b,$08,$1a,$83,$09,$1b,$08,$1a,$83,$09,$1b
	.byte $08,$1b,$08,$08,$08
@ref62:
	.byte $1b,$08,$1a,$83,$09,$1b,$08,$1a,$83,$09,$1b,$08,$1a,$83,$09,$1b
	.byte $08,$1b,$08,$08,$81
	.byte $ff,$15
	.word @ref61
	.byte $ff,$15
	.word @ref62
	.byte $ff,$15
	.word @ref61
	.byte $ff,$15
	.word @ref62
	.byte $ff,$15
	.word @ref61
	.byte $ff,$15
	.word @ref62
	.byte $ff,$15
	.word @ref61
	.byte $ff,$15
	.word @ref62
	.byte $ff,$15
	.word @ref61
@ref72:
	.byte $1b,$08,$1a,$83,$09,$1b,$08,$1a,$83,$08,$81
@ref73:
	.byte $1b,$08,$1a,$83,$09,$1b,$08,$1b,$08,$08,$81
	.byte $ff,$0b
	.word @ref72
	.byte $ff,$0b
	.word @ref73
	.byte $ff,$15
	.word @ref62
	.byte $ff,$15
	.word @ref61
	.byte $ff,$15
	.word @ref62
	.byte $ff,$15
	.word @ref61
	.byte $fd
	.word @song0ch3loop

@song0ch4:
@song0ch4loop:
@ref80:
	.byte $bf
@ref81:
	.byte $bf
@ref82:
	.byte $bf
@ref83:
	.byte $bf
@ref84:
	.byte $bf
@ref85:
	.byte $bf
@ref86:
	.byte $bf
@ref87:
	.byte $bf
@ref88:
	.byte $bf
@ref89:
	.byte $bf
@ref90:
	.byte $bf
@ref91:
	.byte $bf
@ref92:
	.byte $9f
@ref93:
	.byte $9f
@ref94:
	.byte $9f
@ref95:
	.byte $9f
@ref96:
	.byte $bf
@ref97:
	.byte $bf
@ref98:
	.byte $bf
@ref99:
	.byte $bf
	.byte $fd
	.word @song0ch4loop


@song1ch0:
	.byte $fb,$06
@song1ch0loop:
@ref100:
	.byte $f9,$85
	.byte $fd
	.word @song1ch0loop

@song1ch1:
@song1ch1loop:
@ref101:
	.byte $f9,$85
	.byte $fd
	.word @song1ch1loop

@song1ch2:
@song1ch2loop:
@ref102:
	.byte $80,$4b,$53,$59,$61,$63,$61,$59,$53,$4b,$53,$59,$61,$63,$61,$6b
	.byte $63,$41,$49,$4f,$57,$59,$57,$4f,$49,$41,$49,$4f,$57,$59,$61,$67
	.byte $60,$81
	.byte $fd
	.word @song1ch2loop

@song1ch3:
@song1ch3loop:
@ref103:
	.byte $90,$20,$99,$21,$20,$9d,$20,$99,$21,$20,$9d
	.byte $fd
	.word @song1ch3loop

@song1ch4:
@song1ch4loop:
@ref104:
	.byte $f9,$85
	.byte $fd
	.word @song1ch4loop


@song2ch0:
	.byte $fb,$06
@ref105:
	.byte $88,$58,$85,$58,$85,$52,$85,$4a,$85
@ref106:
	.byte $5a,$85,$5a,$85,$5e,$85,$5a,$85
@ref107:
	.byte $59,$5b,$59,$5b,$59,$5b,$59,$5a,$81
@ref108:
	.byte $58,$9b,$00
@song2ch0loop:
@ref109:
	.byte $81
	.byte $fd
	.word @song2ch0loop

@song2ch1:
@ref110:
	.byte $8e,$3a,$85,$40,$85,$3b,$37,$32,$85
@ref111:
	.byte $42,$85,$32,$85,$47,$45,$42,$85
@ref112:
	.byte $3a,$85,$32,$85,$3a,$85,$32,$85
@ref113:
	.byte $3a,$99,$00,$81
@song2ch1loop:
@ref114:
	.byte $81
	.byte $fd
	.word @song2ch1loop

@song2ch2:
@ref115:
	.byte $80,$1a,$8d,$28,$85,$22,$85
@ref116:
	.byte $1c,$8d,$24,$85,$2a,$85
@ref117:
	.byte $32,$9d
@ref118:
	.byte $1a,$99,$00,$81
@song2ch2loop:
@ref119:
	.byte $81
	.byte $fd
	.word @song2ch2loop

@song2ch3:
@ref120:
	.byte $92,$02,$85,$02,$85,$90,$10,$8d
	.byte $ff,$06
	.word @ref120
@ref122:
	.byte $92,$02,$85,$02,$85,$02,$85,$02,$85
@ref123:
	.byte $02,$8d,$90,$10,$8d
@song2ch3loop:
@ref124:
	.byte $81
	.byte $fd
	.word @song2ch3loop

@song2ch4:
@ref125:
	.byte $9f
@ref126:
	.byte $9f
@ref127:
	.byte $9f
@ref128:
	.byte $9f
@song2ch4loop:
@ref129:
	.byte $81
	.byte $fd
	.word @song2ch4loop
