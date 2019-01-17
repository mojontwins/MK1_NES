// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Extra routines.
// Add here calls to routines which need to update sprites and/or bg.

	// Animate ring if active
	ring_do ();

	// Animate bridges 
	bridge_do ();

	// Underwater
	if (underwater) {
		// slower movements. Cheesy but kinda works!
		if (pvx < -96) pvx = -96;
		else if (pvx > 96) pvx = 96;

		if (pvy > 96) pvy = 96;
		if (pvy < -160 && !ptrampoline) pvy = -160;

		// spawn a bubble?
		if (timer_frames == 25) { bubble_x = prx; bubble_y = pry - 8; }

		// Update bubble
		if (bubble_y) {
			rda = rand8 ();

			-- bubble_y;
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
		-- toggle_timer;
		if (toggle_timer == 0) {
			hrt = HOTSPOT_TYPE_TOGGLE_OFF;
			sfx_play (SFX_STEPON, 0);
		}
	}
	
	// Electricity
	if (level == 15) {
		if (ticker == 0 || ticker == 25) {
			if (elec_state_ct) -- elec_state_ct; else {
				++ elec_state; if (elec_state == 3) elec_state = 0;
				elec_state_ct = elec_state_max_ct [elec_state];
				if (elec_state == 0) pal_bg (palts5);
				else if (elec_state == 1) {
					//pal_col (11, 0x18);
					__asm__ ("lda #$18");
					__asm__ ("sta PAL_BUF+11");
					__asm__ ("inc %v", PALUPDATE);
				}
			}
		}
	}

	// Final boss
	if (level == 17 && n_pant == 2) {
		somari_do ();

		// Sound
		if (en_mx [0] < 0 && half_life) sfx_play (1, 2); 		

		// Collision with somari:
		if (prx > 216) {
			prx = 216; px = prx << FIXBITS; pvx = 0;
			if (pspin) {
				pvx = -512;			
				pflickering = 30;
				if (somari_life) {
					-- somari_life;
					somari_state = 1;	
				} else {
					somari_state = 2;
				}
				somari_ct = 120;
				sfx_play (SFX_BREAKB, 1);
			}
		}

		// Collision with ray
		if (pflickering == 0 &&
			en_mx [0] < 0 &&
			prx + 3 >= en_x [0] &&
			prx <= en_x [0] + 7
		) {
			pkill = 1;
		}
	}
