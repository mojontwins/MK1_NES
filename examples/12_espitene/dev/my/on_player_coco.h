// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Add here your code. Player has been hit by a coco.

// The default action can be overriden assigning values to flags:
// en_sg_2 controls if player suffers or not.

// If 
// - Player has rings (prings)
// Then create animated ring, but don't hurt the player!

if (prings) {

	// Create animated ring:
	ring_create ();
	/*
	// Moved these to ring_create
	prings = 0;
	pbouncing = 16;
	pflickering = 100;
	*/

	// Simulate hit
	sfx_play (SFX_PHIT, 0);

	// But don't hurt player!
	en_sg_2 = 0;
} else psprid = pfacing + CELL_HIT;
