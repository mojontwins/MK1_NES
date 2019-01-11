// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Custom player kill routine. This will replace the standard `player_kill ()`
// function *completely*, so beware.

	// For espitene, we just start the "dying animation".
	// The actual code for this is in player_move_custom.h.

	if (!pdying) {
		sfx_play (SFX_PHIT, 0);
		if (plife) --plife; else game_over = 1;
		pdying = 1;
		pvy = -320;
	}	
