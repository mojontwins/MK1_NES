// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Cutscenes, title screen, etc.

void bat_in (void) {
	pal_bright (0);
	ppu_on_all ();
	while (pad_poll (0));
	fade_delay = 4;
	fade_in ();
}

void bat_out (void) {
	music_stop ();
	fade_out ();
	oam_clear ();
	ppu_off ();
}

void pres (const unsigned char *p, void (*func) (void)) {
	cls ();
	pal_bg (p);
	(*func) ();
	bat_in ();
	while (1) {
		pad_read ();
		if (pad_this_frame & (PAD_A|PAD_B|PAD_START)) break;
	}
	bat_out ();
	bank_bg (0);
}

void title (void) {
	scroll (0,0);
	pal_spr (palss0);
	pal_bg (palts1);
	unrle_vram (title_rle, 0x2000);

	bat_in ();

	while (1) {
		oam_meta_spr (84, 170 + (level << 4), 0, sspl_00_a);
		ppu_waitnmi ();
		pad_read ();
		rda = level;
		if (pad_this_frame & (PAD_SELECT|PAD_DOWN)) {
			level ++; if (level == 2) level = 0;
		}
		if (pad_this_frame & PAD_UP) {
			if (level) level --; else level = 1;
		}
		if (level != rda) sfx_play (SFX_USE, 0);
		if (pad_this_frame & PAD_START) break;
	}
	sfx_play (SFX_START, 0); delay (20);

	bat_out ();
}

void scr_game_over (void) {
	_x = 11; _y = 15; pr_str ("GAME OVER!");
}

void scr_the_end (void) {
	oam_meta_spr (100, 64, 0, ssending_00);
	_x = 12; _y = 22; pr_str ("THE  END");
}
