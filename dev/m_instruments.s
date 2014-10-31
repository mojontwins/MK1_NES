music_instruments:
@ins:
	.word @env_default,@env_default,@env_default
	.byte $30,$00
	.word @env_vol2,@env_default,@env_default
	.byte $30,$00
	.word @env_vol0,@env_default,@env_pitch0
	.byte $30,$00
	.word @env_vol8,@env_default,@env_default
	.byte $30,$00
	.word @env_vol9,@env_arp1,@env_default
	.byte $30,$00
	.word @env_vol13,@env_arp7,@env_default
	.byte $30,$00
	.word @env_vol22,@env_default,@env_default
	.byte $30,$00
	.word @env_vol15,@env_default,@env_default
	.byte $30,$00
@env_default:
	.byte $c0,$7f,$00
@env_vol0:
	.byte $c1,$c3,$c7,$c9,$7f,$03
@env_vol1:
	.byte $cf,$cd,$cb,$c9,$c7,$c5,$c3,$c1,$c0,$7f,$08
@env_vol2:
	.byte $cd,$7f,$00
@env_vol3:
	.byte $c1,$c3,$c4,$c6,$c7,$c8,$c9,$ca,$ca,$cb,$cc,$cc,$cd,$0c,$cc,$cc
	.byte $cb,$ca,$c7,$c1,$7f,$13
@env_vol8:
	.byte $c9,$07,$c6,$04,$c9,$06,$7f,$05
@env_vol9:
	.byte $c8,$7f,$00
@env_vol13:
	.byte $cf,$cd,$cb,$c9,$c7,$c5,$c3,$c1,$c0,$7f,$08
@env_vol15:
	.byte $ca,$c5,$c3,$c2,$c0,$7f,$04
@env_vol22:
	.byte $cd,$cc,$ca,$c9,$c7,$c6,$c5,$c3,$c0,$7f,$08
@env_arp0:
	.byte $c0,$01,$c1,$c2,$7f,$03
@env_arp1:
	.byte $cc,$c0,$7f,$01
@env_arp7:
	.byte $c7,$c3,$c0,$bd,$7f,$02
@env_pitch0:
	.byte $c0,$0d,$bf,$be,$bd,$bc,$bd,$be,$bf,$c0,$7f,$02
@env_pitch1:
	.byte $c0,$bf,$bf,$c0,$7f,$00
