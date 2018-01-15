title_module:
	.word @chn0,@chn1,@chn2,@chn3,@chn4,music_instruments
	.byte $04

@chn0:
@chn0_loop:
@chn0_0:
	.byte $42,$1f,$86,$1c,$82,$18,$82,$20,$86,$22,$82,$20,$82,$1f,$86,$1c
	.byte $82,$18,$82,$20,$86,$22,$82,$20,$82
@chn0_1:
	.byte $24,$86,$1f,$82,$1c,$82,$22,$82,$20,$82,$1f,$82,$20,$82,$24,$86
	.byte $1f,$82,$1c,$82,$22,$82,$20,$82,$1f,$82,$20,$82
@chn0_2:
	.byte $44,$2b,$80,$28,$80,$24,$80,$28,$80,$2b,$80,$28,$80,$24,$80,$28
	.byte $80,$2b,$80,$28,$80,$24,$80,$28,$80,$2b,$80,$28,$80,$24,$80,$28
	.byte $80,$2c,$80,$29,$80,$25,$80,$29,$80,$2c,$80,$29,$80,$25,$80,$29
	.byte $80,$2c,$80,$29,$80,$25,$80,$29,$80,$2c,$80,$29,$80,$25,$80,$29
	.byte $80
@chn0_3:
	.byte $30,$84,$2c,$80,$2b,$80,$29,$80,$28,$86,$2c,$82,$2b,$82,$2c,$82
	.byte $30,$84,$2c,$80,$2b,$80,$2c,$80,$29,$80,$28,$80,$2e,$82,$2c,$82
	.byte $2b,$82,$2c,$82
@chn0_4:
	.byte $42,$18,$8e,$19,$8e,$18,$82,$1f,$9a
@chn0_5:
	.byte $1c,$9e,$24,$9e
@chn0_6:
	.byte $18,$a2,$1f,$82,$20,$82,$22,$82,$24,$82,$22,$82,$20,$82,$1d,$82
@chn0_7:
	.byte $1c,$a2,$1a,$82,$1c,$82,$1d,$82,$1f,$82,$20,$82,$1f,$82,$1d,$82
@chn0_8:
	.byte $ff,$40
	.word @chn0_7
@chn0_9:
	.byte $43,$1f,$82,$1f,$80,$1f,$80,$1c,$82,$3f,$86,$20,$82,$22,$82,$3f
	.byte $82,$1f,$82,$1f,$80,$1f,$80,$1c,$82,$3f,$86,$20,$82,$22,$82,$20
	.byte $82
@chn0_10:
	.byte $1f,$82,$1f,$80,$1f,$80,$1c,$82,$3f,$86,$20,$82,$22,$82,$3f,$82
	.byte $1f,$82,$1f,$80,$1f,$80,$1c,$82,$3f,$86,$20,$82,$22,$82,$20,$82
	.byte $fe
	.word @chn0_loop

@chn1:
@chn1_loop:
@chn1_0:
	.byte $3f,$81,$42,$1f,$86,$1c,$82,$18,$82,$20,$86,$22,$82,$20,$82,$1f
	.byte $86,$1c,$82,$18,$82,$20,$86,$22,$82,$20
@chn1_1:
	.byte $82,$24,$86,$1f,$82,$1c,$82,$22,$82,$20,$82,$1f,$82,$20,$82,$24
	.byte $86,$1f,$82,$1c,$82,$22,$82,$20,$82,$1f,$82,$20
@chn1_2:
	.byte $43,$1f,$8a,$1f,$82,$20,$8a,$20,$82,$1f,$82,$1d,$86,$1d,$82,$19
	.byte $82,$1d,$82,$25,$86
@chn1_3:
	.byte $24,$8e,$25,$8e,$24,$8e,$22,$8e
@chn1_4:
	.byte $3f,$80,$18,$8e,$19,$92,$1f,$80,$20,$82,$22,$82,$24,$82,$22,$82
	.byte $20,$82,$1d,$82
@chn1_5:
	.byte $81,$1c,$a0,$1a,$82,$1c,$82,$1d,$82,$1f,$82,$20,$82,$1f,$82,$1d
	.byte $82
@chn1_6:
	.byte $24,$80,$24,$80,$26,$80,$24,$80,$24,$80,$26,$80,$28,$80,$29,$80
	.byte $28,$86,$26,$82,$24,$86,$24,$82,$28,$82,$2b,$82,$25,$82,$25,$82
	.byte $29,$82,$2c,$82
@chn1_7:
	.byte $41,$1c,$8a,$1f,$86,$19,$86,$18,$86,$20,$9a
@chn1_8:
	.byte $1c,$8a,$1f,$86,$19,$86,$18,$86,$20,$9a
@chn1_9:
	.byte $44,$2b,$80,$24,$80,$28,$80,$29,$80,$2b,$80,$24,$80,$28,$80,$29
	.byte $80,$2b,$80,$24,$80,$28,$80,$29,$80,$2b,$80,$24,$80,$28,$80,$29
	.byte $80,$2b,$80,$24,$80,$28,$80,$29,$80,$2b,$80,$24,$80,$28,$80,$29
	.byte $80,$2b,$80,$24,$80,$28,$80,$29,$80,$2b,$80,$24,$80,$28,$80,$29
	.byte $80
@chn1_10:
	.byte $2c,$80,$25,$80,$29,$80,$2b,$80,$2c,$80,$25,$80,$29,$80,$2b,$80
	.byte $2c,$80,$25,$80,$29,$80,$2b,$80,$2c,$80,$25,$80,$29,$80,$2b,$80
	.byte $42,$22,$93,$43,$20,$82,$22,$82,$20,$81
	.byte $fe
	.word @chn1_loop

@chn2:
@chn2_loop:
@chn2_0:
	.byte $41,$18,$82,$18,$82,$1c,$82,$1f,$82,$19,$82,$19,$82,$1d,$82,$20
	.byte $82,$18,$82,$18,$82,$1c,$82,$1f,$82,$19,$82,$19,$82,$1d,$82,$20
	.byte $82
@chn2_1:
	.byte $18,$82,$18,$82,$1c,$82,$1f,$82,$19,$82,$19,$82,$1d,$82,$20,$82
	.byte $18,$82,$18,$82,$1c,$82,$1f,$82,$19,$82,$19,$82,$1d,$82,$20,$82
@chn2_2:
	.byte $0c,$80,$18,$18,$18,$80,$0c,$80,$18,$18,$18,$80,$0c,$80,$18,$18
	.byte $11,$80,$1d,$1d,$1d,$80,$11,$80,$1d,$1d,$1d,$80,$11,$80,$1d,$1d
	.byte $10,$80,$1c,$1c,$1c,$80,$10,$80,$1c,$1c,$1c,$80,$10,$80,$1c,$1c
	.byte $0d,$80,$0d,$80,$19,$80,$0d,$80,$0d,$80,$19,$80,$0f,$80,$1b,$80
@chn2_3:
	.byte $ff,$40
	.word @chn2_2
@chn2_4:
	.byte $18,$82,$18,$82,$19,$82,$18,$82,$19,$82,$1c,$82,$19,$82,$18,$82
	.byte $18,$82,$18,$82,$19,$82,$18,$82,$19,$82,$1c,$82,$18,$82,$19,$82
@chn2_5:
	.byte $18,$86,$19,$82,$18,$82,$19,$82,$1c,$82,$19,$82,$18,$81,$18,$10
	.byte $82,$1c,$1c,$1c,$80,$10,$82,$1c,$80,$1c,$1c,$0d,$82,$19,$19,$19
	.byte $80,$0d,$82,$19,$80,$19,$19
@chn2_6:
	.byte $ff,$40
	.word @chn2_4
@chn2_7:
	.byte $ff,$40
	.word @chn2_5
@chn2_8:
	.byte $18,$86,$19,$82,$18,$82,$19,$82,$1c,$86,$10,$80,$10,$10,$1c,$82
	.byte $28,$28,$28,$80,$1c,$82,$28,$80,$28,$28,$19,$82,$25,$25,$25,$80
	.byte $19,$82,$25,$80,$25,$25
@chn2_9:
	.byte $18,$82,$18,$82,$1c,$82,$1c,$82,$19,$82,$19,$82,$1d,$82,$19,$82
	.byte $18,$82,$18,$82,$1c,$82,$1c,$82,$19,$82,$19,$82,$1d,$82,$20,$82
@chn2_10:
	.byte $ff,$40
	.word @chn2_9
	.byte $fe
	.word @chn2_loop

@chn3:
@chn3_loop:
@chn3_0:
	.byte $46,$00,$80,$47,$0d,$80,$46,$00,$80,$00,$80,$45,$00,$80,$47,$0d
	.byte $80,$46,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$46,$00,$80
	.byte $00,$80,$45,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$46,$00
	.byte $47,$0d,$0f,$0d,$46,$00,$80,$00,$80,$45,$00,$80,$47,$0d,$80,$46
	.byte $00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$46,$00,$80,$00,$80
	.byte $45,$00,$80,$46,$00,$80,$45,$00,$80,$46,$0d,$80
@chn3_1:
	.byte $00,$80,$47,$0d,$80,$46,$00,$80,$00,$80,$45,$00,$80,$47,$0d,$80
	.byte $46,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$46,$00,$80,$00
	.byte $80,$45,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$46,$00,$47
	.byte $0d,$0f,$0d,$46,$00,$80,$00,$80,$45,$00,$80,$47,$0d,$80,$46,$00
	.byte $80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$46,$00,$80,$00,$80,$45
	.byte $00,$80,$46,$00,$80,$45,$00,$80,$46,$0d,$80
@chn3_2:
	.byte $ff,$40
	.word @chn3_1
@chn3_3:
	.byte $ff,$40
	.word @chn3_1
@chn3_4:
	.byte $00,$80,$47,$0d,$80,$0f,$80,$0d,$80,$46,$00,$80,$47,$0d,$80,$0f
	.byte $80,$0d,$80,$46,$00,$80,$47,$0d,$80,$0f,$80,$0d,$80,$46,$00,$80
	.byte $47,$0d,$80,$46,$00,$80,$00,$80,$00,$80,$0d,$80,$47,$0d,$80,$0f
	.byte $80,$46,$00,$80,$47,$0d,$0f,$0d,$0f,$0d,$80,$46,$00,$80,$47,$0d
	.byte $80,$0f,$80,$0d,$80,$46,$00,$80,$47,$0d,$80,$46,$00,$80,$00,$80
@chn3_5:
	.byte $ff,$40
	.word @chn3_4
@chn3_6:
	.byte $ff,$40
	.word @chn3_4
@chn3_7:
	.byte $ff,$40
	.word @chn3_4
@chn3_8:
	.byte $ff,$40
	.word @chn3_4
@chn3_9:
	.byte $00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$45,$00,$80,$47,$0d
	.byte $80,$46,$00,$47,$0d,$46,$00,$80,$47,$0d,$80,$46,$00,$80,$00,$80
	.byte $47,$0d,$80,$45,$00,$80,$47,$0d,$80,$0f,$80,$0d,$80,$46,$00,$80
	.byte $47,$0d,$80,$46,$00,$80,$47,$0d,$80,$45,$00,$80,$47,$0d,$0f,$46
	.byte $00,$47,$0d,$0f,$0d,$0f,$80,$46,$0d,$80,$00,$80,$47,$0d,$80,$45
	.byte $00,$80,$46,$00,$80,$45,$00,$80,$46,$00,$80
@chn3_10:
	.byte $00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$45,$00,$80,$47,$0d
	.byte $80,$46,$00,$47,$0d,$46,$00,$80,$47,$0d,$80,$46,$00,$80,$00,$80
	.byte $47,$0d,$80,$45,$00,$80,$47,$0d,$80,$0f,$80,$0d,$80,$46,$00,$80
	.byte $47,$0d,$80,$46,$00,$80,$47,$0d,$80,$45,$00,$80,$47,$0d,$80,$45
	.byte $00,$80,$0f,$80,$00,$80,$00,$80,$0f,$80,$0f,$80,$01,$80,$01,$80
	.byte $00,$80,$00,$80
	.byte $fe
	.word @chn3_loop

@chn4:
@chn4_loop:
@chn4_0:
	.byte $bf
@chn4_1:
	.byte $bf
@chn4_2:
	.byte $bf
@chn4_3:
	.byte $bf
@chn4_4:
	.byte $bf
@chn4_5:
	.byte $bf
@chn4_6:
	.byte $bf
@chn4_7:
	.byte $bf
@chn4_8:
	.byte $bf
@chn4_9:
	.byte $bf
@chn4_10:
	.byte $bf
	.byte $fe
	.word @chn4_loop
