// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Make stuff shine

void shines_add (void) {
	if (max_shines == SHINES_MAX) return;
	shines [max_shines ++] = rdm;
}

void shines_do (void) {
	if (shine_active_ct) {
		shine_active_ct --;
		if (shine_active_ct >= 48) {
			oam_index = oam_spr (
				shine_active_x, 
				shine_active_y, 
				SHINES_BASE_PATTERN + (rand8() & 1), 
				SHINES_PALETTE, 
				oam_index
			);
		}
	} else {
		gpit = rand8 () & (SHINES_MAX - 1);
		if (gpit < max_shines) {
			shine_active_ct = 64;
			rda = shines [gpit];
			shine_active_y = (rda & 0xf0) + (rand8 () & 7) + SPRITE_ADJUST + 16;
			shine_active_x = ((rda & 0x0f) << 4) + (rand8 () & 7);
		}
	}
}
