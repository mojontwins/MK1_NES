// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Extra routines.
// Add here calls to routines which need to update sprites and/or bg.

	// Animate ring if active
	ring_do ();

	// Do a palette cycle in the jungle
	if (level_world == 2 && (frame_counter & 7) == 0) {
		rda = pal_cycle [2];
		pal_cycle [2] = pal_cycle [1]; pal_col (15, pal_cycle [2]);
		pal_cycle [1] = pal_cycle [0]; pal_col (14, pal_cycle [1]);
		pal_cycle [0] = rda;           pal_col (13, pal_cycle [0]);
	}

