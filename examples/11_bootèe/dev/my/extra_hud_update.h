// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// game_frame.h
// Custom code to update the HUD. 

if (opfiregauge < pfiregauge) {
	// Fire gauge increased
	gp_addr = 0x20A4 + (pfiregauge >> 1); _n = 9; ul_putc ();
} else if (opfiregauge > pfiregauge) {
	// Clear
	gp_addr = 0x20A4; _n = 8; gpit = 24; while (gpit --) ul_putc ();
}

opfiregauge = pfiregauge;
