mus_game_module:
	.word @chn0,@chn1,@chn2,@chn3,@chn4,music_instruments
	.byte $06

@chn0:
@chn0_0:
	.byte $bf
@chn0_loop:
@chn0_1:
	.byte $bf
@chn0_2:
	.byte $52,$1c,$be
@chn0_3:
	.byte $1c,$be
@chn0_4:
	.byte $41,$1d,$be
@chn0_5:
	.byte $ff,$40
	.word @chn0_2
@chn0_6:
	.byte $ff,$40
	.word @chn0_3
@chn0_7:
	.byte $ff,$40
	.word @chn0_3
@chn0_8:
	.byte $ff,$40
	.word @chn0_4
@chn0_9:
	.byte $ff,$40
	.word @chn0_2
@chn0_10:
	.byte $ff,$40
	.word @chn0_3
@chn0_11:
	.byte $ff,$40
	.word @chn0_3
@chn0_12:
	.byte $ff,$40
	.word @chn0_3
@chn0_13:
	.byte $ff,$40
	.word @chn0_3
@chn0_14:
	.byte $bf
	.byte $fe
	.word @chn0_loop

@chn1:
@chn1_0:
	.byte $bf
@chn1_loop:
@chn1_1:
	.byte $bf
@chn1_2:
	.byte $4d,$1c,$98,$21,$80,$1f,$80,$1d,$80,$1c,$9e
@chn1_3:
	.byte $1c,$98,$21,$80,$1f,$80,$1d,$80,$1c,$9e
@chn1_4:
	.byte $1d,$96,$21,$82,$21,$80,$22,$80,$21,$86,$1f,$86,$1d,$86,$1a,$86
@chn1_5:
	.byte $1c,$98,$1a,$80,$18,$80,$1a,$80,$1c,$8e,$10,$8e
@chn1_6:
	.byte $1c,$96,$21,$80,$21,$80,$1f,$80,$1d,$80,$1c,$9e
@chn1_7:
	.byte $ff,$40
	.word @chn1_6
@chn1_8:
	.byte $ff,$40
	.word @chn1_4
@chn1_9:
	.byte $ff,$40
	.word @chn1_5
@chn1_10:
	.byte $4a,$23,$8a,$23,$80,$21,$80,$23,$82,$24,$82,$26,$82,$1f,$82,$28
	.byte $86,$26,$82,$24,$8a,$23,$82,$21,$82
@chn1_11:
	.byte $23,$8a,$23,$80,$21,$80,$23,$82,$21,$82,$1f,$82,$1c,$82,$1e,$86
	.byte $1f,$82,$21,$86,$23,$82,$24,$86
@chn1_12:
	.byte $23,$8a,$23,$80,$21,$80,$23,$82,$24,$82,$26,$82,$1f,$82,$28,$86
	.byte $26,$82,$24,$8a,$23,$82,$21,$82
@chn1_13:
	.byte $23,$8a,$23,$80,$21,$80,$23,$82,$21,$82,$1f,$82,$1c,$82,$1e,$86
	.byte $1f,$82,$1e,$8a,$1f,$82,$1e,$82
@chn1_14:
	.byte $1c,$be
	.byte $fe
	.word @chn1_loop

@chn2:
@chn2_0:
	.byte $44,$10,$84,$10,$80,$15,$82,$11,$82,$10,$84,$10,$80,$15,$82,$13
	.byte $80,$11,$80,$10,$84,$10,$80,$15,$82,$11,$82,$10,$84,$10,$80,$15
	.byte $82,$13,$80,$11,$80
@chn2_loop:
@chn2_1:
	.byte $ff,$40
	.word @chn2_0
@chn2_2:
	.byte $10,$84,$10,$80,$15,$82,$11,$82,$10,$84,$10,$80,$15,$82,$13,$80
	.byte $11,$80,$10,$84,$10,$80,$15,$82,$11,$82,$10,$84,$10,$80,$15,$82
	.byte $13,$80,$11,$80
@chn2_3:
	.byte $ff,$40
	.word @chn2_2
@chn2_4:
	.byte $11,$84,$11,$80,$18,$82,$15,$82,$11,$84,$11,$80,$18,$82,$17,$80
	.byte $15,$80,$11,$84,$11,$80,$18,$82,$15,$82,$11,$84,$11,$80,$18,$82
	.byte $17,$80,$15,$80
@chn2_5:
	.byte $ff,$40
	.word @chn2_2
@chn2_6:
	.byte $ff,$40
	.word @chn2_2
@chn2_7:
	.byte $ff,$40
	.word @chn2_2
@chn2_8:
	.byte $ff,$40
	.word @chn2_4
@chn2_9:
	.byte $ff,$40
	.word @chn2_2
@chn2_10:
	.byte $ff,$40
	.word @chn2_2
@chn2_11:
	.byte $ff,$40
	.word @chn2_2
@chn2_12:
	.byte $ff,$40
	.word @chn2_2
@chn2_13:
	.byte $ff,$40
	.word @chn2_2
@chn2_14:
	.byte $ff,$40
	.word @chn2_2
	.byte $fe
	.word @chn2_loop

@chn3:
@chn3_0:
	.byte $45,$00,$86,$48,$00,$86,$45,$00,$80,$00,$82,$00,$80,$48,$00,$82
	.byte $00,$82,$45,$00,$86,$48,$00,$86,$45,$00,$80,$00,$82,$00,$80,$48
	.byte $00,$82,$00,$82
@chn3_loop:
@chn3_1:
	.byte $ff,$40
	.word @chn3_0
@chn3_2:
	.byte $ff,$40
	.word @chn3_0
@chn3_3:
	.byte $ff,$40
	.word @chn3_0
@chn3_4:
	.byte $ff,$40
	.word @chn3_0
@chn3_5:
	.byte $ff,$40
	.word @chn3_0
@chn3_6:
	.byte $ff,$40
	.word @chn3_0
@chn3_7:
	.byte $ff,$40
	.word @chn3_0
@chn3_8:
	.byte $ff,$40
	.word @chn3_0
@chn3_9:
	.byte $ff,$40
	.word @chn3_0
@chn3_10:
	.byte $ff,$40
	.word @chn3_0
@chn3_11:
	.byte $ff,$40
	.word @chn3_0
@chn3_12:
	.byte $ff,$40
	.word @chn3_0
@chn3_13:
	.byte $ff,$40
	.word @chn3_0
@chn3_14:
	.byte $ff,$40
	.word @chn3_0
	.byte $fe
	.word @chn3_loop

@chn4:
@chn4_0:
	.byte $bf
@chn4_loop:
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
	.byte $fe
	.word @chn4_loop
