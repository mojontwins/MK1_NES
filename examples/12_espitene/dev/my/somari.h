// Super custom for Espinete Bad:
// Final boss somari implementation.

// Pretty simple. four states: idle / hit / dying / end level

void somari_do (void) {

	switch (somari_state) {
		case 0:
			en_spr = 42 + ((frame_counter >> 3) & 3);
			break;

		case 1:
			//en_spr = half_life ? 44 : 46;
			__asm__ ("lda %v", half_life);
			__asm__ ("asl a");
			__asm__ ("ora #44");
			__asm__ ("sta %v", en_spr);
			if (somari_ct) -- somari_ct; else somari_state = 0;
			break;

		case 2:
			en_spr = 46;
			if (somari_ct) -- somari_ct; else somari_state = 3;
			break;
	}

	oam_index = oam_meta_spr (
		232, SPRITE_ADJUST + 128, 
		oam_index, 
		spr_enems [en_spr]
	);

}
