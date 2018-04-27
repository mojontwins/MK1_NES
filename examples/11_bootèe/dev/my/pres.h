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
	cls ();
	pal_bg (palts0);
	pal_spr (palss0);

	unrle_vram (title_rle, 0x2000);
	_x = 14; _y = 19; pr_str ("GAME A");
	       ; _y = 21; pr_str ("GAME B");
	       ; _y = 23; pr_str ("GAME C");
	_x = 5;  _y = 26; pr_str ("@ 2018 THE MOJON TWINS"); 

	bat_in ();

	while (1) {
		oam_spr (96, 138 + (level << 4), 63, 2, 0);
		ppu_waitnmi ();
		pad_read ();
		rda = level;
		if (pad_this_frame & (PAD_A|PAD_B|PAD_START)) break;
		if (pad_this_frame & (PAD_SELECT|PAD_DOWN)) {
			level ++; if (level == 3) level = 0;
		}
		if (pad_this_frame & PAD_UP) {
			if (level) level --; else level = 2;
		}
		if (level != rda) sfx_play (SFX_USE, 0);
	}
	sfx_play (SFX_START, 0); delay (20);

	bat_out ();
}

void scr_game_over (void) {
	_x = 11; _y = 15; pr_str ("GAME OVER!");
}


const unsigned char * const cuts_rle [] = {
//	level 0    level 1    level 2    level 3    level 4    ending
	cuts0_rle, ending_rle
};


void scr_cutscene (void) {
	// show cuts + text in level;
	unrle_vram (cuts_rle [rda], 0x2000);
	_x = 2; _y = 16; pr_str ((unsigned char *) cutscenes [rda]);	

	_x = 13; _y = 4; pr_str ("GAME "); vram_put (level + 33);
}
