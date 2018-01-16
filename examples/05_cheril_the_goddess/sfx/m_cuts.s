m_cuts_module:
	.word @chn0,@chn1,@chn2,@chn3,@chn4,music_instruments
	.byte $06

@chn0:
@chn0_loop:
@chn0_0:
	.byte $bf
	.byte $fe
	.word @chn0_loop

@chn1:
@chn1_loop:
@chn1_0:
	.byte $bf
	.byte $fe
	.word @chn1_loop

@chn2:
@chn2_loop:
@chn2_0:
	.byte $42,$24,$80,$28,$80,$2b,$80,$2f,$80,$30,$80,$2f,$80,$2b,$80,$28
	.byte $80,$24,$80,$28,$80,$2b,$80,$2f,$80,$30,$80,$2f,$80,$34,$80,$30
	.byte $80,$1f,$80,$23,$80,$26,$80,$2a,$80,$2b,$80,$2a,$80,$26,$80,$23
	.byte $80,$1f,$80,$23,$80,$26,$80,$2a,$80,$2b,$80,$2f,$80,$32,$80,$2f
	.byte $80
	.byte $fe
	.word @chn2_loop

@chn3:
@chn3_loop:
@chn3_0:
	.byte $44,$00,$8c,$00,$80,$00,$8e,$00,$8c,$00,$80,$00,$8e
	.byte $fe
	.word @chn3_loop

@chn4:
@chn4_loop:
@chn4_0:
	.byte $bf
	.byte $fe
	.word @chn4_loop
