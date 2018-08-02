// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Add here your code. Player has been hit by an enemy.

// The default action can be overriden assigning values to flags:
// en_sg_1 controls if enemy suffers or not.
// en_sg_2 controls if player suffers or not.

// If:
// - Player will suffer (hit registered, not spinning): en_sg_2
// - Player has rings (prings)
// Then create animated ring, and simulate hit but don't hurt player!

if (en_sg_2) {
	if (prings) {

		// Create animated ring:
		ring_create ();
		prings = 0;

		// Simulate hit
		pvx = ADD_SIGN (_en_mx, PLAYER_V_REBOUND); 
		if (_en_my) pvy = ADD_SIGN (_en_my, PLAYER_V_REBOUND); else pvy = -(PLAYER_V_REBOUND >> 1);
		if (_en_t != 20) {
			if (!_en_mx) _en_my = ADD_SIGN (_en_y - pry, ABS (_en_my));
			_en_mx = ADD_SIGN (_en_x - prx, ABS (_en_mx));
		}

		touched = 1; 
		sfx_play (SFX_PHIT, 0);
		pbouncing = 16;
		pflickering = 100;

		// But don't hurt player!
		en_sg_2 = 0;
	} else psprid = pfacing + CELL_HIT;
}
