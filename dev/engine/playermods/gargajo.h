// NES MK1 v0.6
// Copyleft Mojon Twins 2013, 2015

// First implementation, simple, bullets rewrite, just side-view

void fire_gargajo (unsigned char gl) {
	// Create a new gargajo
	gpit = MAX_GARGAJOS; while (gpit --) {
		if (0 == gst [gpit]) {
			gst [gpit] = gl;
			gx [gpit] = pfacing ? prx - 4 - GARGAJO_X_OFFSET: prx + 12 + GARGAJO_X_OFFSET;
			gy [gpit] = pry + GARGAJO_Y_OFFSET;
			gmx [gpit] = pfacing ? -GARGAJO_V : GARGAJO_V;
			sfx_play (5, 0);
			break;
		}
	}
}

void gargajos_move (void) {
	gpit = MAX_GARGAJOS; while (gpit --) {
		if (gst [gpit]) {
			// Very, very simple.
			gx [gpit] += gmx [gpit];
			if (gx [gpit] < GARGAJO_V || gx [gpit] > 255 - GARGAJO_V) { gst [gpit] = 0; continue; }

			// Hotspot in the middle of the sprite
			ghsx = gx [gpit] + 3; ghsy = gy [gpit] + 3;
			
			// Collide with background
			if (attr (ghsx >> 4, ghsy >> 4) > 7) { gst [gpit] = 0; sfx_play (6, 2); continue; }

			// Collide with enemies
			gpjt = 3; while (gpjt --) if (collide_in (ghsx, ghsy, en_x [gpjt], en_y [gpjt])) {
				// Horizontal or vertical ?
				if (en_mx [gpjt]) {
					add_sign (gmx [gpit], abs (en_mx [gpjt]));
				} else {
					if (!attr ((en_x [gpjt] << 4) + sgni (gmx [gpit]), (en_y [gpjt] + 8) << 4)) 
						en_x [gpjt] += add_sign (gmx [gpit], 16);
				}
				gst [gpit] = 0; sfx_play (6, 2); continue;
			}

			// Render
			oam_spr (
				gx [gpit], 
				SPRITE_ADJUST + gy [gpit], 
				GARGAJO_BASE_PATTERN + ((ghsx >> 3) & GARGAJO_CELL_MOD) + (gmx [gpit] > 0 ? 0 : GARGAJO_FACING_OFFSET), 
				1, 
				112 + (gpit << 2)
			);

			// Let it die.
			if (gst [gpit] < GARGAJO_V) gst [gpit] = 0; else gst [gpit] -= GARGAJO_V;
		} else {
			oam_spr (0, 240, 251, 0, 112 + (gpit << 2));
		}
	}
}

