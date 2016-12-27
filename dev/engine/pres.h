// Cutscene

unsigned char cutsi, cutc;
unsigned int frc;

const unsigned char zone_clear [] = {
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5a, 0x4f, 0x4e, 0x45, 0x20, 0x43,
	0x4c, 0x45, 0x41, 0x52, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xee
};
void stupid_animation (void) {
	// Play this when you kill all the baddies
	pal_bg (mypal_bw);

	// Print text
	clear_update_list ();
	update_index = 0;
	pr_str_upd ((unsigned char *) zone_clear);
	ppu_waitnmi ();
	clear_update_list ();
	update_index = 0;

	music_play (m_sclear);

	// Float jill a while
	cutsi = 60; cutc = 0;
	while (cutsi --) {
		if (!(cutsi & 0x0f)) cutc = 1 - cutc;
		psprid = 6 + cutc;
		render_player ();
		enems_move ();
		ppu_waitnmi ();
	}	

	// Fly jill out of here
	cutsi = 0;
	frc = 0;
	while (pry) {
		frc ++;
		cutsi ++;
		if (!(cutsi & 0x0f)) cutc = 1 - cutc;
		psprid = 6 + cutc;
		pry --;
		prx += ((rand8 () & 1) << 1) - 1;
		render_player ();
		ppu_waitnmi ();
	}

	oam_meta_spr (0, 240, 128, spr_pl_empty);

	while (frc ++ < 400) {
		ppu_waitnmi ();
		if (pad_poll (0)) break;
	}

	music_stop ();
}

const unsigned char level_str [] = {
	0x4c, 0x45, 0x56, 0x45, 0x4c, 0x20, 0x30, 0
};
const unsigned char dalefran_str [] = {
	0x44, 0x41, 0x4c, 0x45, 0x20, 0x46, 0x52, 0x41, 0x4e, 0x21, 0
};
void level_screen (void) {
	cls ();
	pal_bg (mypal_bw);

	pr_str (12, 13, (unsigned char *) level_str);
	vram_put (BASE_LEVEL + level + 1 + 16);
	pr_str (11, 15, (unsigned char *) dalefran_str);

	pal_bright (0);
	ppu_on_all ();
	fade_delay = 4;
	fade_in ();

	cutsi = 100;
	while (cutsi -- && !pad_poll (0)) {
		ppu_waitnmi ();
	}

	fade_out ();
	ppu_off ();
}
