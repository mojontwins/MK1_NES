// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// Skip?
if ((i & PAD_A) && (i & PAD_SELECT) && (i & PAD_UP)) {
	n_pant = SCR_END;
	prx = PLAYER_END_X << 4;
	pry = PLAYER_END_Y << 4;
}
