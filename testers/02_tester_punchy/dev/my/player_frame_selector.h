// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// player_frame_selector.h
// Selects the correct metasprite upon player state

// You may (will) need to tinker with this for your game.
// Use player variables p* and end up with a correct value
// in psprid.

rda = (ATTR((prx + 4) >> 4, pry >> 4) & 12);
if (ponladder && !rda) {
	if (pvy) ponladderctr ++;
	psprid = CELL_CLIMB_CYCLE + ((ponladderctr >> 2) & 3);
} else {
	if (pfloating) {
		psprid = CELL_DESCENDING;
	} else if (ppunching) {
		psprid = CELL_PUNCHING;
	} else if (pkicking) {
		psprid = CELL_KICKING;
	} else if (rda || ppossee || pgotten) {
		// On floor
		if (pvx > PLAYER_VX_MIN || pvx < -PLAYER_VX_MIN) {
			psprid = CELL_WALK_CYCLE + ((prx >> 3) & 3);
		} else if (pvx) {
			psprid = CELL_WALK_INIT;
		} else {
			psprid = CELL_IDLE;
		}
	} else {
		//psprid = CELL_AIRBORNE;
		if (pvy < PLAYER_VY_FALLING_MIN)
			psprid = CELL_ASCENDING;
		else
			psprid = CELL_DESCENDING;	
	}

	psprid += pfacing;
}