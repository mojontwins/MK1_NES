// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// The code you write here is executed after moving (or not) the player
// during the main loop.
	
	if (pdying) {
		if (pvy <= PLAYER_VY_FALLING_MAX) pvy += PLAYER_G;
		py += pvy;
		if (py < 0) py = 0;
		pry = py >> FIXBITS;
		psprid = CELL_HIT;
	
		// End of animation: espinete exits the screen
		if (pry > 218) {
			level_reset = 1; 	// This will break the main loop
		}
	}
