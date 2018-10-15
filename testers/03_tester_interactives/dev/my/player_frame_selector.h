// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// player_frame_selector.h
// Selects the correct metasprite upon player state

// You may (will) need to tinker with this for your game.
// Use player variables p* and end up with a correct value
// in psprid.

#ifdef ENABLE_USE_ANIM
	if (use_ct) {
		psprid = CELL_USE + use_ct - 1;
	} else 
#endif	
	if (ppossee || pgotten) {
		if (pvx > PLAYER_VX_MIN || pvx < -PLAYER_VX_MIN) {
			psprid = CELL_WALK_CYCLE + ((prx >> 3) & 3);
		} else {
			psprid = CELL_IDLE;
		}
	} else {
		if (pvy < PLAYER_VY_FALLING_MIN)
			psprid = CELL_ASCENDING;
		else
			psprid = CELL_DESCENDING;	
	}

	psprid += pfacing;
