// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// Make stuff shine

void shines_add (unsigned char x, unsigned char y) {
	shines [max_shines ++] = (x << 4) | y;
}

void p_t (unsigned char, unsigned char, unsigned char);

void shines_do (void) {

	if (last_shine_ct) {
		last_shine_ct --;
		if (last_shine_ct < 48) oam_spr (0, 240, 0, 0, 96); else {
			rda = shines [shine_active];
			oam_spr (shine_active_x, shine_active_y, 235 + (rand8() & 1), 3, 96);
		}
	} else {
		gpit = rand8 () & (MAX_GANCHOS - 1);
		if (gpit < max_shines) {
			last_shine_ct = 64;
			rda = shines [gpit];
			shine_active_x = (rda & 0xf0) + (rand8 () & 7);
			shine_active_y = ((rda & 0x0f) << 4) + (rand8 () & 7) + SPRITE_ADJUST;
		}
	}
}
