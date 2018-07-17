// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// player_frame_selector.h
// Selects the correct metasprite upon player state

// You may (will) need to tinker with this for your game.
// Use player variables p* and end up with a correct value
// in psprid.

// For Bootèe

if (pstate == EST_REBOUND) {
	psprid = 6; 
} else {
	if (pfiregauge) {
		psprid = 6; 
	} else if (pfirereload) {
		psprid = 7;
	} else {
		if (pad0 & PAD_DOWN)
			psprid = 5;
		else if (pvy < -128)
			psprid = 2;
		else if (pvy < -64)
			psprid = 1;
		else if (pvy < 64)
			psprid = 0;
		else if (pvy < 128)
			psprid = 3;
		else
			psprid = 4;
	}
}

psprid += pfacing;
