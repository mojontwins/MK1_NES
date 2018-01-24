music_instruments:
@ins:
	.word @env_default,@env_default,@env_default
	.byte $30,$00
	.word @env_vol2,@env_default,@env_default
	.byte $30,$00
	.word @env_vol3,@env_default,@env_default
	.byte $30,$00
	.word @env_vol4,@env_default,@env_default
	.byte $30,$00
	.word @env_vol5,@env_default,@env_default
	.byte $30,$00
	.word @env_vol2,@env_arp0,@env_default
	.byte $30,$00
	.word @env_vol4,@env_default,@env_default
	.byte $30,$00
	.word @env_vol6,@env_default,@env_default
	.byte $30,$00
	.word @env_vol7,@env_arp3,@env_default
	.byte $30,$00
	.word @env_vol8,@env_arp1,@env_default
	.byte $30,$00
	.word @env_vol1,@env_arp1,@env_default
	.byte $70,$00
	.word @env_vol1,@env_arp11,@env_default
	.byte $30,$00
	.word @env_vol6,@env_default,@env_pitch1
	.byte $70,$00
	.word @env_vol9,@env_arp2,@env_default
	.byte $70,$00
	.word @env_vol9,@env_arp13,@env_default
	.byte $30,$00
	.word @env_vol9,@env_arp5,@env_default
	.byte $30,$00
	.word @env_vol6,@env_default,@env_pitch2
	.byte $30,$00
	.word @env_vol9,@env_arp5,@env_default
	.byte $30,$00
	.word @env_vol9,@env_arp14,@env_default
	.byte $30,$00
@env_default:
	.byte $c0,$7f,$00
@env_vol1:
	.byte $cf,$ce,$cd,$cc,$cb,$ca,$c9,$c1,$7f,$07
@env_vol2:
	.byte $cf,$cd,$cb,$c9,$c7,$c5,$c3,$c1,$c0,$7f,$08
@env_vol3:
	.byte $cb,$ca,$c8,$c7,$c5,$c4,$c3,$c1,$c0,$7f,$08
@env_vol4:
	.byte $c8,$c7,$c6,$c5,$c4,$c3,$c2,$c1,$c0,$7f,$08
@env_vol5:
	.byte $c4,$c4,$c3,$c3,$c2,$c2,$c1,$c1,$c0,$7f,$08
@env_vol6:
	.byte $c1,$c3,$c4,$c6,$c7,$c8,$c9,$ca,$ca,$cb,$cc,$cc,$cd,$0c,$cc,$cc
	.byte $cb,$ca,$c7,$c1,$7f,$13
@env_vol7:
	.byte $c2,$7f,$00
@env_vol8:
	.byte $cf,$7f,$00
@env_vol9:
	.byte $ce,$ce,$cd,$01,$cc,$03,$cb,$03,$ca,$02,$c9,$04,$c8,$05,$c7,$03
	.byte $c6,$05,$c5,$08,$c4,$07,$c3,$0b,$c2,$13,$c1,$1d,$7f,$1b
@env_arp0:
	.byte $c7,$c3,$c0,$bd,$7f,$02
@env_arp1:
	.byte $cc,$c0,$7f,$01
@env_arp2:
	.byte $be,$01,$bf,$c0,$7f,$03
@env_arp3:
	.byte $c0,$bf,$be,$bd,$bc,$bb,$ba,$b9,$b8,$b7,$b6,$b5,$b4,$b3,$b2,$b1
	.byte $b0,$af,$ae,$ad,$ac,$ab,$aa,$a9,$a8,$a7,$a6,$a5,$a4,$a3,$a2,$a1
	.byte $7f,$1f
@env_arp5:
	.byte $c0,$02,$c3,$02,$c7,$02,$c3,$02,$7f,$00
@env_arp10:
	.byte $c0,$01,$c5,$01,$c9,$01,$c5,$01,$7f,$00
@env_arp11:
	.byte $c0,$c7,$c7,$c0,$7f,$03
@env_arp13:
	.byte $c0,$01,$c4,$01,$c9,$01,$c4,$01,$7f,$00
@env_arp14:
	.byte $c0,$02,$c3,$02,$c8,$02,$c3,$02,$7f,$00
@env_pitch0:
	.byte $c0,$7f,$00
@env_pitch1:
	.byte $c0,$bf,$bf,$c0,$7f,$00
@env_pitch2:
	.byte $c0,$c1,$c2,$01,$c1,$c0,$c0,$c3,$c6,$c9,$cc,$cf,$02,$7f,$00
