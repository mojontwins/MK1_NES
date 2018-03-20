// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// player_frame_selector.h
// Selects the correct metasprite upon player state

// You may (will) need to tinker with this for your game.
// Use player variables p* and end up with a correct value
// in psprid.

// Todo: USE

rdx = (prx + 4) >> 4;
rda = ATTR(rdx, pry >> 4) & 12;
rdb = ATTR(rdx, (pry-2) >> 4) == 32;
if (use_ct) {
	psprid = CELL_USE + use_ct - 1;
} else if (ponladder && !rda && rdb) {
	if (pvy) ponladderctr ++;
	psprid = CELL_CLIMB_CYCLE + ((ponladderctr >> 2) & 3);
} else {
	if (ppossee || pgotten || ponladder) {
		if (pvx > PLAYER_VX_MIN || pvx < -PLAYER_VX_MIN) {
			psprid = CELL_WALK_CYCLE + ((prx >> 3) & 3);
		} else {
			psprid = CELL_IDLE;
		}
	} else {
		psprid = CELL_AIRBORNE;
	}

	psprid += pfacing;
}
