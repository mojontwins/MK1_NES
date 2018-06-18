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

void scr_title (void) {
	#ifdef SPANISH
		unrle_vram (title_sp_rle, 0x2000);
		_x = 10; _y = 19; pr_str ("PULSA START!"); 
	#else
		unrle_vram (title_en_rle, 0x2000);
		_x = 10; _y = 19; pr_str ("PRESS START!"); 
	#endif
	
	_x = 5;  _y = 26; pr_str ("@ 2018 THE MOJON TWINS"); 
}

void scr_game_over (void) {
	_x = 11; _y = 15; pr_str ("GAME OVER!");
}

void scr_the_end (void) {
	_x = 12; _y = 15; pr_str ("THE END");
}

void scr_level (void) {
	_x = 12; _y = 15; pr_str ("LEVEL 0"); vram_put (level + 17);
}

const unsigned char * const cuts_rle [] = {
//	level 0    level 1    level 2    level 3    level 4    ending
	cuts0_rle, cuts0_rle, cuts1_rle, cuts2_rle, cuts3_rle, cuts3_rle
};

void scr_cutscene (void) {
	// show cuts + text in level;
	unrle_vram (cuts_rle [level], 0x2000);
	_x = 2; _y = 18; pr_str ((unsigned char *) cutscenes [level]);
	music_play (MUSIC_CUTS);
}
