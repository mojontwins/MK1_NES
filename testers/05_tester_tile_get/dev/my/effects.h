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

	// Level 5 flashing should be 50Hz in PAL and 60Hz in NTSC
	// So we put it here.
	if (level_world == 5) {
		if (elec_state == 2) {
			pal_bg (half_life ? palts5 : palts5a);
			ppu_mask (0x1e);
		} else {
			ppu_mask (0xfe); 
		}
	}
