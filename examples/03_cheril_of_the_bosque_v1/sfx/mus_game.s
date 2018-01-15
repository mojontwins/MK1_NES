mus_game_module:
	.word @chn0,@chn1,@chn2,@chn3,@chn4,music_instruments
	.byte $04

@chn0:
@chn0_loop:
@chn0_0:
	.byte $42,$1f,$86,$1f,$80,$1f,$80,$1f,$86,$1d,$86,$1c,$86,$1d,$86,$1c
	.byte $86,$1d,$86,$1d,$82
@chn0_1:
	.byte $1b,$86,$1b,$86,$18,$82,$1b,$86,$1b,$82,$1d,$86,$1d,$86,$1d,$82
	.byte $1c,$86,$18,$82
@chn0_2:
	.byte $1f,$86,$1f,$80,$1f,$80,$1f,$86,$1d,$86,$1c,$86,$1d,$86,$1c,$86
	.byte $1d,$86,$1d,$82
@chn0_3:
	.byte $ff,$40
	.word @chn0_1
@chn0_4:
	.byte $43,$1f,$82,$18,$82,$1c,$82,$1f,$86,$18,$82,$1f,$82,$18,$82,$1d
	.byte $82,$16,$82,$1c,$82,$1d,$86,$16,$82,$1d,$82,$16,$82
@chn0_5:
	.byte $1b,$82,$14,$82,$18,$82,$1b,$86,$14,$82,$1b,$82,$14,$82,$1d,$82
	.byte $16,$82,$1c,$82,$1d,$82,$22,$21,$20,$1f,$1e,$1d,$1c,$1b,$1a,$19
	.byte $18,$17,$16,$15,$14,$13
@chn0_6:
	.byte $42,$18,$9a,$16,$86,$18,$96,$16,$82
@chn0_7:
	.byte $83,$18,$96,$1b,$86,$18,$8a,$1b,$82,$1d,$8a
@chn0_8:
	.byte $18,$9a,$16,$86,$18,$96,$16,$82
@chn0_9:
	.byte $ff,$40
	.word @chn0_7
@chn0_10:
	.byte $41,$1d,$82,$1c,$92,$1d,$82,$1f,$92,$1f,$86,$20,$82,$22,$82
@chn0_11:
	.byte $97,$22,$82,$24,$82,$24,$80,$24,$80,$22,$86,$20,$86,$1f,$8a
@chn0_12:
	.byte $44,$30,$82,$2e,$82,$2c,$82,$2b,$82,$30,$82,$2e,$82,$2c,$82,$2b
	.byte $82,$30,$82,$2e,$82,$2c,$82,$2b,$82,$30,$82,$2e,$82,$2c,$82,$2b
	.byte $82
@chn0_13:
	.byte $30,$82,$2e,$82,$2c,$82,$2b,$82,$30,$82,$2e,$82,$2c,$82,$2b,$82
	.byte $30,$82,$2e,$82,$2c,$82,$2b,$82,$30,$82,$2e,$82,$2c,$82,$2b,$82
@chn0_14:
	.byte $ff,$40
	.word @chn0_13
@chn0_15:
	.byte $ff,$40
	.word @chn0_13
@chn0_16:
	.byte $43,$24,$82,$24,$82,$22,$82,$22,$82,$24,$82,$22,$82,$22,$82,$27
	.byte $82,$27,$82,$24,$82,$24,$82,$24,$82,$22,$82,$22,$82,$24,$82,$22
	.byte $82
@chn0_17:
	.byte $22,$82,$27,$82,$27,$82,$24,$82,$24,$82,$24,$82,$22,$82,$22,$82
	.byte $24,$82,$22,$82,$22,$82,$27,$82,$27,$82,$24,$82,$24,$82,$24,$82
@chn0_18:
	.byte $24,$82,$24,$82,$22,$82,$22,$82,$24,$82,$22,$82,$22,$82,$27,$82
	.byte $27,$82,$24,$82,$24,$82,$24,$82,$22,$82,$22,$82,$24,$82,$22,$82
@chn0_19:
	.byte $ff,$40
	.word @chn0_17
@chn0_20:
	.byte $ff,$40
	.word @chn0_18
@chn0_21:
	.byte $ff,$40
	.word @chn0_17
@chn0_22:
	.byte $ff,$40
	.word @chn0_18
@chn0_23:
	.byte $ff,$40
	.word @chn0_17
@chn0_24:
	.byte $ff,$40
	.word @chn0_18
@chn0_25:
	.byte $ff,$40
	.word @chn0_17
@chn0_26:
	.byte $ff,$40
	.word @chn0_17
@chn0_27:
	.byte $42,$1d,$9a,$1d,$82,$20,$82,$22,$9a
	.byte $fe
	.word @chn0_loop

@chn1:
@chn1_loop:
@chn1_0:
	.byte $80,$43,$1f,$86,$1f,$80,$1f,$80,$1f,$86,$1d,$86,$1c,$86,$1d,$86
	.byte $1c,$86,$1d,$86,$1d,$81
@chn1_1:
	.byte $80,$1b,$86,$1b,$86,$18,$82,$1b,$86,$1b,$82,$1d,$86,$1d,$86,$1d
	.byte $82,$1c,$86,$18,$81
@chn1_2:
	.byte $80,$1f,$86,$1f,$80,$1f,$80,$1f,$86,$1d,$86,$1c,$86,$1d,$86,$1c
	.byte $86,$1d,$86,$1d,$81
@chn1_3:
	.byte $ff,$40
	.word @chn1_1
@chn1_4:
	.byte $44,$2b,$80,$2b,$80,$24,$80,$24,$80,$28,$80,$28,$80,$29,$80,$29
	.byte $80,$2b,$80,$2b,$80,$24,$80,$24,$80,$28,$80,$28,$80,$29,$80,$29
	.byte $80,$29,$80,$29,$80,$22,$80,$22,$80,$26,$80,$26,$80,$28,$80,$28
	.byte $80,$29,$80,$29,$80,$22,$80,$22,$80,$26,$80,$26,$80,$28,$80,$28
	.byte $80
@chn1_5:
	.byte $27,$80,$27,$80,$20,$80,$20,$80,$24,$80,$24,$80,$26,$80,$26,$80
	.byte $27,$80,$27,$80,$20,$80,$20,$80,$24,$80,$24,$80,$26,$80,$26,$80
	.byte $29,$80,$29,$80,$22,$80,$22,$80,$26,$80,$26,$80,$28,$80,$28,$80
	.byte $29,$80,$29,$80,$22,$80,$22,$80,$26,$80,$26,$80,$28,$80,$28,$80
@chn1_6:
	.byte $41,$18,$82,$18,$86,$18,$82,$1f,$82,$1d,$82,$1c,$82,$16,$86,$18
	.byte $82,$18,$92,$16,$82
@chn1_7:
	.byte $16,$82,$18,$82,$18,$92,$18,$82,$1b,$82,$18,$82,$18,$82,$18,$82
	.byte $1b,$82,$1d,$82,$1d,$86
@chn1_8:
	.byte $18,$82,$18,$86,$18,$82,$1f,$82,$1d,$82,$1c,$82,$16,$86,$18,$82
	.byte $18,$92,$16,$82
@chn1_9:
	.byte $ff,$40
	.word @chn1_7
@chn1_10:
	.byte $44,$1f,$80,$1d,$80,$1c,$80,$1d,$80,$1f,$80,$1d,$80,$1c,$80,$1d
	.byte $80,$1f,$80,$1d,$80,$1c,$80,$1d,$80,$1f,$80,$1d,$80,$1c,$80,$1d
	.byte $80,$20,$80,$1f,$80,$1d,$80,$1f,$80,$20,$80,$1f,$80,$1d,$80,$1f
	.byte $80,$20,$80,$1f,$80,$1d,$80,$1f,$80,$20,$80,$1f,$80,$1d,$80,$1f
	.byte $80
@chn1_11:
	.byte $2e,$80,$2c,$80,$2b,$80,$2c,$80,$2e,$80,$2c,$80,$2b,$80,$2c,$80
	.byte $2e,$80,$2c,$80,$2b,$80,$2c,$80,$2e,$80,$2c,$80,$2b,$80,$2c,$80
	.byte $30,$82,$2e,$82,$2c,$82,$2b,$82,$30,$82,$2e,$82,$2c,$82,$2b,$82
@chn1_12:
	.byte $43,$22,$82,$22,$82,$20,$86,$1f,$82,$22,$82,$24,$86,$22,$82,$22
	.byte $82,$20,$86,$1f,$82,$1d,$8a
@chn1_13:
	.byte $1b,$96,$20,$82,$1f,$86,$1d,$86,$1b,$86,$1d,$8a
@chn1_14:
	.byte $22,$82,$22,$82,$20,$86,$1f,$82,$22,$82,$24,$86,$22,$82,$22,$82
	.byte $20,$86,$1f,$82,$1d,$8a
@chn1_15:
	.byte $ff,$40
	.word @chn1_13
@chn1_16:
	.byte $42,$18,$9e,$1b,$9e
@chn1_17:
	.byte $1d,$9e,$20,$9e
@chn1_18:
	.byte $18,$9e,$1b,$9e
@chn1_19:
	.byte $18,$86,$16,$86,$18,$82,$16,$86,$1b,$86,$18,$9a
@chn1_20:
	.byte $24,$8e,$2b,$82,$29,$86,$24,$8e,$24,$82,$2b,$82,$29,$86,$24,$82
@chn1_21:
	.byte $8f,$18,$86,$16,$86,$18,$82,$16,$86,$1b,$86,$18,$8a
@chn1_22:
	.byte $44,$29,$82,$28,$82,$29,$82,$28,$86,$42,$24,$82,$27,$82,$2b,$a2
@chn1_23:
	.byte $22,$82,$24,$96,$24,$82,$27,$82,$29,$9a
@chn1_24:
	.byte $24,$92,$24,$82,$26,$86,$27,$9e
@chn1_25:
	.byte $44,$24,$80,$22,$80,$24,$80,$27,$80,$24,$80,$22,$80,$24,$80,$27
	.byte $80,$24,$80,$22,$80,$24,$80,$27,$80,$24,$80,$22,$80,$24,$80,$27
	.byte $80,$24,$80,$22,$80,$24,$80,$27,$80,$24,$80,$22,$80,$24,$80,$27
	.byte $80,$24,$80,$22,$80,$24,$80,$27,$80,$24,$80,$22,$80,$24,$80,$27
	.byte $80
@chn1_26:
	.byte $24,$80,$22,$80,$24,$80,$27,$80,$24,$80,$22,$80,$24,$80,$27,$80
	.byte $24,$80,$22,$80,$24,$80,$27,$80,$24,$80,$22,$80,$24,$80,$27,$80
	.byte $24,$80,$22,$80,$24,$80,$27,$80,$24,$80,$22,$80,$24,$80,$27,$80
	.byte $24,$80,$22,$80,$24,$80,$27,$80,$24,$80,$22,$80,$24,$80,$27,$80
@chn1_27:
	.byte $ff,$40
	.word @chn1_26
	.byte $fe
	.word @chn1_loop

@chn2:
@chn2_loop:
@chn2_0:
	.byte $41,$18,$82,$18,$82,$18,$82,$18,$82,$2b,$80,$1f,$80,$18,$80,$1f
	.byte $80,$18,$82,$2b,$82,$16,$82,$16,$82,$16,$82,$16,$82,$29,$80,$1d
	.byte $80,$16,$80,$1d,$80,$16,$82,$29,$82
@chn2_1:
	.byte $14,$82,$14,$82,$14,$82,$14,$82,$27,$80,$1b,$80,$14,$80,$1b,$80
	.byte $14,$82,$27,$82,$16,$82,$16,$82,$16,$82,$16,$82,$29,$80,$1d,$80
	.byte $16,$80,$1d,$80,$16,$82,$29,$82
@chn2_2:
	.byte $18,$82,$18,$82,$18,$82,$18,$82,$2b,$80,$1f,$80,$18,$80,$1f,$80
	.byte $18,$82,$2b,$82,$16,$82,$16,$82,$16,$82,$16,$82,$29,$80,$1d,$80
	.byte $16,$80,$1d,$80,$16,$82,$29,$82
@chn2_3:
	.byte $ff,$40
	.word @chn2_1
@chn2_4:
	.byte $18,$82,$18,$82,$18,$82,$18,$82,$3f,$82,$18,$82,$18,$82,$3f,$82
	.byte $16,$82,$16,$82,$16,$82,$16,$82,$3f,$82,$16,$82,$16,$82,$3f,$82
@chn2_5:
	.byte $14,$82,$14,$82,$14,$82,$14,$82,$14,$82,$14,$82,$14,$82,$14,$82
	.byte $16,$82,$16,$82,$16,$82,$16,$82,$3a,$39,$38,$37,$36,$35,$34,$33
	.byte $32,$31,$30,$2f,$2e,$2d,$2c,$2b
@chn2_6:
	.byte $18,$80,$18,$80,$18,$80,$18,$80,$18,$82,$13,$82,$1f,$82,$1d,$82
	.byte $1c,$82,$16,$82,$16,$80,$16,$80,$16,$80,$16,$80,$16,$80,$16,$80
	.byte $13,$82,$1f,$82,$1d,$82,$1c,$82,$14,$82
@chn2_7:
	.byte $14,$80,$14,$80,$14,$80,$14,$80,$14,$82,$13,$82,$1f,$82,$1d,$82
	.byte $1c,$82,$16,$82,$16,$80,$16,$80,$16,$80,$16,$80,$16,$80,$16,$80
	.byte $13,$82,$1f,$82,$1d,$82,$1c,$82,$1f,$82
@chn2_8:
	.byte $ff,$40
	.word @chn2_6
@chn2_9:
	.byte $ff,$40
	.word @chn2_7
@chn2_10:
	.byte $1f,$82,$1f,$82,$1f,$82,$1f,$82,$1f,$82,$1f,$82,$1f,$82,$1f,$82
	.byte $20,$82,$20,$82,$20,$82,$20,$82,$20,$82,$20,$82,$20,$82,$20,$82
@chn2_11:
	.byte $22,$82,$22,$82,$22,$82,$22,$82,$22,$82,$22,$82,$22,$82,$22,$82
	.byte $24,$82,$24,$82,$24,$82,$24,$82,$24,$82,$24,$82,$24,$82,$24,$82
@chn2_12:
	.byte $0c,$82,$18,$82,$0c,$82,$18,$82,$0c,$80,$0c,$80,$18,$82,$0c,$82
	.byte $18,$82,$0a,$82,$16,$82,$0a,$82,$16,$82,$0a,$80,$16,$80,$14,$80
	.byte $16,$80,$0a,$82,$16,$82
@chn2_13:
	.byte $0f,$82,$1b,$82,$0f,$82,$1b,$82,$0f,$80,$0f,$80,$1b,$82,$0f,$82
	.byte $1b,$82,$11,$82,$1d,$82,$11,$82,$1d,$82,$11,$80,$1d,$80,$1c,$80
	.byte $1d,$80,$11,$82,$1d,$82
@chn2_14:
	.byte $ff,$40
	.word @chn2_12
@chn2_15:
	.byte $ff,$40
	.word @chn2_13
@chn2_16:
	.byte $0c,$82,$0c,$82,$0c,$82,$0c,$82,$0c,$82,$0c,$82,$0c,$82,$0c,$82
	.byte $0f,$82,$0f,$82,$0f,$82,$0f,$82,$0f,$82,$0f,$82,$0f,$82,$0f,$82
@chn2_17:
	.byte $11,$82,$11,$82,$11,$82,$11,$82,$11,$82,$11,$82,$11,$82,$11,$82
	.byte $14,$82,$14,$82,$14,$82,$14,$82,$3a,$39,$38,$37,$36,$35,$34,$33
	.byte $32,$31,$30,$2f,$2e,$2d,$2c,$2b
@chn2_18:
	.byte $ff,$40
	.word @chn2_16
@chn2_19:
	.byte $11,$82,$11,$82,$11,$82,$11,$82,$11,$82,$11,$82,$11,$82,$11,$82
	.byte $14,$82,$14,$82,$14,$82,$14,$82,$14,$82,$14,$82,$14,$82,$14,$82
@chn2_20:
	.byte $ff,$40
	.word @chn2_16
@chn2_21:
	.byte $ff,$40
	.word @chn2_19
@chn2_22:
	.byte $ff,$40
	.word @chn2_16
@chn2_23:
	.byte $ff,$40
	.word @chn2_19
@chn2_24:
	.byte $ff,$40
	.word @chn2_16
@chn2_25:
	.byte $ff,$40
	.word @chn2_19
@chn2_26:
	.byte $ff,$40
	.word @chn2_19
@chn2_27:
	.byte $ff,$40
	.word @chn2_17
	.byte $fe
	.word @chn2_loop

@chn3:
@chn3_loop:
@chn3_0:
	.byte $46,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$45,$00,$80,$47
	.byte $0d,$80,$46,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$0f,$80
	.byte $0d,$80,$45,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$46,$00
	.byte $80,$47,$0d,$80,$0f,$80,$0d,$80,$45,$00,$80,$47,$0d,$80,$46,$00
	.byte $80,$47,$0d,$80,$0f,$80,$0d,$80,$46,$00,$80,$47,$0d,$80,$45,$00
	.byte $80,$46,$00,$80,$00,$80,$0d,$80
@chn3_1:
	.byte $00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$45,$00,$80,$47,$0d
	.byte $80,$46,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$0f,$80,$0d
	.byte $80,$45,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$46,$00,$80
	.byte $47,$0d,$80,$0f,$80,$0d,$80,$45,$00,$80,$47,$0d,$80,$46,$00,$80
	.byte $47,$0d,$80,$45,$00,$80,$00,$80,$01,$80,$01,$80,$0f,$80,$0f,$80
	.byte $00,$80,$00,$80
@chn3_2:
	.byte $ff,$40
	.word @chn3_0
@chn3_3:
	.byte $ff,$40
	.word @chn3_1
@chn3_4:
	.byte $46,$00,$80,$47,$0d,$80,$46,$00,$80,$00,$80,$45,$00,$80,$47,$0d
	.byte $80,$46,$00,$80,$47,$0d,$80,$0f,$80,$46,$00,$80,$00,$80,$47,$0d
	.byte $80,$45,$00,$80,$47,$0d,$80,$0f,$80,$45,$00,$80,$46,$00,$80,$47
	.byte $0d,$80,$46,$00,$80,$00,$80,$45,$00,$80,$47,$0d,$80,$46,$00,$80
	.byte $47,$0d,$80,$0f,$80,$46,$00,$80,$00,$80,$47,$0d,$80,$45,$00,$80
	.byte $47,$0d,$80,$45,$00,$80,$00,$80
@chn3_5:
	.byte $46,$00,$80,$47,$0d,$80,$46,$00,$80,$00,$80,$45,$00,$80,$47,$0d
	.byte $80,$46,$00,$80,$47,$0d,$80,$0f,$80,$46,$00,$80,$00,$80,$47,$0d
	.byte $80,$45,$00,$80,$47,$0d,$80,$0f,$80,$45,$00,$80,$46,$00,$80,$47
	.byte $0d,$80,$46,$00,$80,$00,$80,$45,$00,$80,$47,$0d,$80,$46,$00,$80
	.byte $47,$0d,$80,$45,$00,$80,$00,$80,$01,$80,$01,$80,$0f,$80,$0f,$80
	.byte $00,$80,$00,$80
@chn3_6:
	.byte $46,$00,$80,$47,$0d,$80,$45,$00,$80,$47,$0d,$80,$46,$00,$80,$00
	.byte $80,$45,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$45,$00,$80
	.byte $47,$0d,$0f,$0d,$80,$46,$00,$80,$45,$00,$80,$47,$0d,$80,$46,$00
	.byte $80,$47,$0d,$80,$45,$00,$80,$47,$0d,$80,$46,$00,$80,$00,$80,$45
	.byte $00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$45,$00,$80,$47,$0d
	.byte $0f,$0d,$80,$46,$00,$80,$45,$00,$80,$00,$80
@chn3_7:
	.byte $ff,$40
	.word @chn3_6
@chn3_8:
	.byte $ff,$40
	.word @chn3_6
@chn3_9:
	.byte $ff,$40
	.word @chn3_6
@chn3_10:
	.byte $46,$00,$80,$47,$0d,$80,$45,$00,$80,$47,$0d,$80,$46,$00,$80,$00
	.byte $80,$45,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$45,$00,$80
	.byte $47,$0d,$0f,$0d,$80,$46,$00,$80,$45,$00,$80,$47,$0d,$80,$46,$00
	.byte $80,$47,$0d,$80,$45,$00,$80,$47,$0d,$80,$46,$00,$80,$00,$80,$45
	.byte $00,$80,$47,$0d,$80,$45,$00,$80,$00,$80,$00,$80,$00,$80,$00,$80
	.byte $00,$80,$00,$80,$00,$80
@chn3_11:
	.byte $ff,$40
	.word @chn3_10
@chn3_12:
	.byte $46,$00,$80,$47,$0d,$80,$0f,$80,$0d,$80,$46,$00,$80,$47,$0d,$80
	.byte $0f,$80,$0d,$80,$46,$00,$80,$47,$0d,$80,$0f,$80,$0d,$80,$46,$00
	.byte $80,$47,$0d,$80,$46,$00,$80,$00,$80,$00,$80,$0d,$80,$47,$0d,$80
	.byte $0f,$80,$46,$00,$80,$47,$0d,$0f,$0d,$0f,$0d,$80,$46,$00,$80,$47
	.byte $0d,$80,$0f,$80,$0d,$80,$46,$00,$80,$47,$0d,$80,$46,$00,$80,$00
	.byte $80
@chn3_13:
	.byte $00,$80,$47,$0d,$80,$0f,$80,$0d,$80,$46,$00,$80,$47,$0d,$80,$0f
	.byte $80,$0d,$80,$46,$00,$80,$47,$0d,$80,$0f,$80,$0d,$80,$46,$00,$80
	.byte $47,$0d,$80,$46,$00,$80,$00,$80,$00,$80,$0d,$80,$47,$0d,$80,$0f
	.byte $80,$46,$00,$80,$47,$0d,$0f,$0d,$0f,$0d,$80,$46,$00,$80,$47,$0d
	.byte $80,$0f,$80,$0d,$80,$46,$00,$80,$47,$0d,$80,$46,$00,$80,$00,$80
@chn3_14:
	.byte $ff,$40
	.word @chn3_13
@chn3_15:
	.byte $ff,$40
	.word @chn3_13
@chn3_16:
	.byte $47,$0d,$86,$0d,$86,$0d,$86,$0d,$82,$0d,$0f,$0d,$0f,$0d,$82,$0d
	.byte $0f,$0d,$0f,$0d,$82,$0f,$80,$0f,$80,$0d,$82,$0d,$82,$0f,$80,$0f
	.byte $80,$0d,$82
@chn3_17:
	.byte $0d,$86,$0d,$86,$0d,$86,$0d,$82,$0d,$0f,$0d,$0f,$0d,$82,$0d,$0f
	.byte $0d,$0f,$0d,$82,$0f,$80,$0f,$80,$0d,$82,$0d,$82,$0f,$80,$0f,$80
	.byte $0d,$82
@chn3_18:
	.byte $46,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$45,$00,$80,$47
	.byte $0d,$80,$46,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$46,$00
	.byte $80,$47,$0d,$80,$45,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80
	.byte $46,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$45,$00,$80,$47
	.byte $0d,$80,$46,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$46,$00
	.byte $80,$47,$0d,$80,$45,$00,$80,$46,$00,$80,$00,$80,$0d,$80
@chn3_19:
	.byte $00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$45,$00,$80,$47,$0d
	.byte $80,$46,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$46,$00,$80
	.byte $47,$0d,$80,$45,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$46
	.byte $00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$45,$00,$80,$47,$0d
	.byte $80,$46,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$46,$00,$80
	.byte $47,$0d,$80,$45,$00,$80,$46,$00,$80,$00,$80,$0d,$80
@chn3_20:
	.byte $ff,$40
	.word @chn3_19
@chn3_21:
	.byte $ff,$40
	.word @chn3_19
@chn3_22:
	.byte $ff,$40
	.word @chn3_19
@chn3_23:
	.byte $ff,$40
	.word @chn3_19
@chn3_24:
	.byte $ff,$40
	.word @chn3_19
@chn3_25:
	.byte $00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$45,$00,$80,$47,$0d
	.byte $80,$46,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$46,$00,$80
	.byte $47,$0d,$80,$45,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$46
	.byte $00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$45,$00,$80,$47,$0d
	.byte $80,$46,$00,$80,$00,$80,$45,$00,$80,$47,$0d,$80,$46,$00,$80,$00
	.byte $80,$45,$00,$80,$00,$80,$00,$80,$00,$80
@chn3_26:
	.byte $46,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$45,$00,$80,$47
	.byte $0d,$80,$46,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$46,$00
	.byte $80,$47,$0d,$80,$45,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80
	.byte $46,$00,$80,$47,$0d,$80,$46,$00,$80,$47,$0d,$80,$45,$00,$80,$47
	.byte $0d,$80,$46,$00,$80,$00,$80,$45,$00,$80,$47,$0d,$80,$46,$00,$80
	.byte $00,$80,$45,$00,$80,$00,$80,$00,$80,$00,$80
@chn3_27:
	.byte $47,$0d,$86,$0d,$86,$0d,$86,$0d,$82,$0d,$0f,$0d,$0f,$0d,$82,$0d
	.byte $0f,$0d,$0f,$0d,$82,$0f,$80,$0f,$80,$45,$00,$80,$00,$80,$01,$80
	.byte $01,$80,$0f,$80,$0f,$80,$00,$80,$00,$80
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
@chn4_11:
	.byte $bf
@chn4_12:
	.byte $bf
@chn4_13:
	.byte $bf
@chn4_14:
	.byte $bf
@chn4_15:
	.byte $bf
@chn4_16:
	.byte $bf
@chn4_17:
	.byte $bf
@chn4_18:
	.byte $bf
@chn4_19:
	.byte $bf
@chn4_20:
	.byte $bf
@chn4_21:
	.byte $bf
@chn4_22:
	.byte $bf
@chn4_23:
	.byte $bf
@chn4_24:
	.byte $bf
@chn4_25:
	.byte $bf
@chn4_26:
	.byte $bf
@chn4_27:
	.byte $bf
	.byte $fe
	.word @chn4_loop
