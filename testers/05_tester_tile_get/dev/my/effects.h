// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Effects - This code gets included right before waiting for next NMI
// Add here palette cycles/etc, splits...

	// Do a palette cycle in the jungle
	if ((level_world == 2 || level_world == 4) && (frame_counter & 7) == 0) {
		rda = pal_cycle [2];
		pal_cycle [2] = pal_cycle [1]; pal_col (15, pal_cycle [2]);
		pal_cycle [1] = pal_cycle [0]; pal_col (14, pal_cycle [1]);
		pal_cycle [0] = rda;           pal_col (13, pal_cycle [0]);
	}

	// Do a split in the fridge
	if (water_strip) split (frame_counter, 0);
