// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Extra routines.
// Add here calls to routines which need to update sprites and/or bg.

	// Animate ring if active
	ring_do ();

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

		// Respawn time refills
		if (ht [n_pant] == HOTSPOT_TYPE_TIME) {
			if (timer < 10) hrt = HOTSPOT_TYPE_TIME;	// Reappear if needed w/o having to reenter
			hact [n_pant] = 1;
		}
	}
	
	// toggle switch timer:
	if (toggle_timer) {
		toggle_timer --;
		if (toggle_timer == 0) {
			hrt = HOTSPOT_TYPE_TOGGLE_OFF;
			sfx_play (SFX_STEPON, 0);
		}
	}
	