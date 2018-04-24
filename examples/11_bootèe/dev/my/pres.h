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

void scr_title (void) {
	unrle_vram (title_rle, 0x2000);
	_x = 10; _y = 19; pr_str ("PRESS START!"); 
	_x = 5;  _y = 26; pr_str ("@ 2018 THE MOJON TWINS"); 
}

void scr_game_over (void) {
	_x = 11; _y = 15; pr_str ("GAME OVER!");
}


const unsigned char * const cuts_rle [] = {
//	level 0    level 1    level 2    level 3    level 4    ending
	cuts0_rle, cuts0_rle
};


void scr_cutscene (void) {
	// show cuts + text in level;
	
	unrle_vram (cuts_rle [rda], 0x2000);
	_x = 2; _y = 16; pr_str ((unsigned char *) cutscenes [rda]);	
}
