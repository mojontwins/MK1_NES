// NES MK1 v0.7
// Copyleft Mojon Twins 2013, 2015, 2016

void bat_in (void) {
	ppu_on_all ();
	while (pad_poll (0));
	fade_delay = 4;
	fade_in ();
}

void bat_out (void) {
	fade_out ();
	oam_clear ();
	ppu_off ();
}

unsigned char pres_title (void) {
	unrle_vram (title_rle, 0x2000);
	bat_in ();
	music_play (m_title);

	gpit = 0;
	while (1) {
		i = pad_poll (0);
		if (i & PAD_START) break;
		if (i & PAD_SELECT) {
			sfx_play (0, 1);
			gpit = 1 - gpit;
			while (pad_poll (0));
		}
		oam_meta_spr (84, 156 + (gpit << 4) + 7, 128, spr_player [0]);
		ppu_waitnmi ();
	}

	music_stop ();
	sfx_play (7, 1);

	bat_out ();

	return gpit;
}

void pres_common (void) {
	bat_in ();
	music_play (m_gameover);
	while (!pad_poll (0));
	bat_out ();
}

void pres_game_over (void) {
	unrle_vram (cuts_rle, 0x2000);
	pr_str (10, 12, "GAME OVER!");
	pr_str (4, 14, "NICE TRY, BUT NO BANANA!");
	pr_str (5, 16, "DO IT BETTER NEXT TIME");
	pr_str (9, 18, "MISSION FAILED");
}

void pres_ending (void) {
	unrle_vram (cuts_rle, 0x2000);
	pr_str (9, 12, "CONGRATULATIONS!");
	pr_str (2, 14, "YOU MANAGED TO SET THE BOMBS");
	pr_str (4, 16, "AND DESTROY THE COMPUTER");
	pr_str (5, 18, "MISSION ACCOMPLISHED!!");	
}

signed int lower_end;
void credits (void) {
	pal_bg (palts0);
	pal_spr (palss0);
	vram_adr (0x2000); vram_fill (0xff,0x400);
	oam_clear (); scroll (0, 0);
	
	lower_end = 0; rdy = 240;

	pr_str (3, 22, "SGT. HELMET - TRAINING DAY");
	pr_str (0, 24, "COPYLEFT 2013 BY THE MOJON TWINS");
	pr_str (0, 25, "DESIGN:ANJUEL  NES PORT:NA-TH-AN");
	pr_str (3, 26, "NESLIB & SFX CODE BY SHIRU");
	pr_str (3, 27, "TITLE TUNE ARR BY DAVIDIAN");
	
	pal_bright (0);
	bat_in ();

	while (!(pad_poll (0) & PAD_START) && lower_end < 300) {
		oam_meta_spr (102, rdy, 0, spr_logo_00);
		if (rdy > 112) rdy --;
		ppu_waitnmi ();
		lower_end ++;
	};

	bat_out ();
}
