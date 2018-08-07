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

	// Underwater
	if (underwater) {
		// slower movements. Cheesy but kinda works!
		if (pvx < -96) pvx = -96;
		else if (pvx > 96) pvx = 96;

		if (pvy > 96) pvy = 96;
		if (pvy < -160 && !ptrampoline) pvy = -160;

		// Update bubble
		if (bubble_y) {
			rda = rand8 ();

			bubble_y --;			
			bubble_x += ((rda & 2) - 1);

			oam_index = oam_spr (
				bubble_x, bubble_y + SPRITE_ADJUST, 
				0x0c + (rda & 1),
				2, 
				oam_index
			);
		}

		// Update countdown
		if (timer && timer < 10) oam_index = oam_spr (
			prx, pry - 16 + SPRITE_ADJUST - (12 - (timer_frames >> 2)), 
			2 + timer, 
			2, 
			oam_index
		);
	}