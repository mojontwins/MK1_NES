// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Add here your code.  This gets run right before checking
// for screen boundaries. Do your custom screen flicking here.
// Set flick_override to invalidate normal behaviour

// On levels 6 or 8, we don't let the player exit the screen left/right:

if (level == 6 || level == 8) {
	flick_override = 1;

	// We have to stop player on left/right
	if (
		(prx == 4 && pvx < 0) ||
		(prx == 244 && pvx > 0) 
	) pvx = 0;

	// Just detect up/down connections
	flickscreen_do_vertical ();
}

// On level world 5, get killed by the bottom of the map

if ((level == 15 && n_pant >= 15) ||
	(level == 16 && n_pant >= 24)) {
	if (pry >= 192 && pvy > 0) {
		pkill = 1;
		flick_override = 1;
	}
}
