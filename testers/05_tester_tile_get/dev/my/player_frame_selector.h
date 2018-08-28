// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// player_frame_selector.h
// Selects the correct metasprite upon player state

// You may (will) need to tinker with this for your game.
// Use player variables p* and end up with a correct value
// in psprid.

	if (pbouncing || phit || pkill) {
		psprid = CELL_HIT;
	} else if (pspin) {
		psprid = CELL_SPIN_CYCLE + ((frame_counter >> 4) & 3);
	} else if (ppossee || pgotten) {
		// On floor
		if (pvx > PLAYER_VX_MIN || pvx < -PLAYER_VX_MIN) {
			psprid = CELL_WALK_CYCLE + ((prx >> 3) & 3);
		} else {
			psprid = CELL_IDLE;
		}
	} else {
		psprid = CELL_AIRBORNE;
	}

	psprid += pfacing;
