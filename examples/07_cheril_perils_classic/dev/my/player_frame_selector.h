// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// player_frame_selector.h
// Selects the correct metasprite upon player state

// You may (will) need to tinker with this for your game.
// Use player variables p* and end up with a correct value
// in psprid.

#ifdef PLAYER_TOP_DOWN

	// Frame selection for top-down view games

	if (pvx != 0 || pvy != 0) {
		++ pctfr;
		if (pctfr == 4) {
			pctfr = 0;
			pfr = !pfr;
			psprid = pfacing + pfr;
		}
	}

#else

	// Frame selection for side view games

	#ifdef PLAYER_SWIMS
		if (i && (rdx != prx || rdy != pry)) {
			if (pvx) {
				psprid = CELL_SWIM_CYCLE + ((prx >> 3) & 3);
			} else {
				psprid = CELL_SWIM_CYCLE + ((pry >> 3) & 3);
			}
		} else psprid = CELL_SWIM_CYCLE + 1;
	#else
		if (ppossee || pgotten) {
			// On floor
			if (pvx > PLAYER_VX_MIN || pvx < -PLAYER_VX_MIN) {
				psprid = CELL_WALK_CYCLE + ((prx >> 3) & 3);
			} else {
				psprid = CELL_IDLE;
			}
		} else {
			psprid = CELL_AIRBORNE;
			/*
			if (pvy < PLAYER_VY_FALLING_MIN)
				psprid = CELL_ASCENDING;
			else
				psprid = CELL_DESCENDING;
			*/	
		}
	#endif

	psprid += pfacing;
#endif
