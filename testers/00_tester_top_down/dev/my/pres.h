// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

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
	music_play (MUSIC_TITLE);

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
	music_play (MUSIC_GAME_OVER);
	while (!pad_poll (0));
	bat_out ();
}

void pres_game_over (void) {
	unrle_vram (cuts_rle, 0x2000);
	_x = 10; _y = 12; pr_str ("GAME OVER!");
	_x =  4; _y = 14; pr_str ("NICE TRY, BUT NO BANANA!");
	_x =  5; _y = 16; pr_str ("DO IT BETTER NEXT TIME");
	_x =  9; _y = 18; pr_str ("MISSION FAILED");
	pres_common ();
}

void pres_ending (void) {
	unrle_vram (cuts_rle, 0x2000);
	_x = 9; _y = 12; pr_str ("CONGRATULATIONS!");
	_x = 2; _y = 14; pr_str ("YOU MANAGED TO SET THE BOMBS");
	_x = 4; _y = 16; pr_str ("AND DESTROY THE COMPUTER");
	_x = 5; _y = 18; pr_str ("MISSION ACCOMPLISHED!!");	
	pres_common ();
}

signed int lower_end;
void credits (void) {
	pal_bg (palts0);
	pal_spr (palss0);
	vram_adr (0x2000); vram_fill (0xff,0x400);
	oam_clear (); scroll (0, 0);
	
	lower_end = 0; rdy = 240;

	_x = 3; _y = 22; pr_str ("SGT. HELMET - TRAINING DAY");
	        _y = 26; pr_str ("NESLIB & SFX CODE BY SHIRU");
	        _y = 27; pr_str ("TITLE TUNE ARR BY DAVIDIAN");
	_x = 0; _y = 24; pr_str ("(C) 2013,2018 BY THE MOJON TWINS");
	        _y = 25; pr_str ("DESIGN:ANJUEL  NES PORT:NA-TH-AN");
	
	
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
