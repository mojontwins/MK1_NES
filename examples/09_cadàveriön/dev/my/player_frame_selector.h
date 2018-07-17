// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// player_frame_selector.h
// Selects the correct metasprite upon player state

// You may (will) need to tinker with this for your game.
// Use player variables p* and end up with a correct value
// in psprid.

// Frame selection for top-down view games, extended

#ifdef ENABLE_USE_ANIM
	if (use_ct) {
		psprid = CELL_USE + use_ct - 1;
	} else 
#endif
if (pad0 & PAD_B) {
	psprid = pfacing + CELL_PUSHING;
} else if (pvx) {
	psprid = pfacing + CELL_WALK_CYCLE + ((prx >> 4) & 3);
} else if (pvy) {
	psprid = pfacing + CELL_WALK_CYCLE + ((pry >> 4) & 3);
} else psprid = pfacing;
