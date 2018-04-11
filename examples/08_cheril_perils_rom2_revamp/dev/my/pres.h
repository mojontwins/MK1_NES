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

void scr_cuts (void) {
	//unrle_vram (ending_rle, 0x2000);

	_x =3; _y = 18; pr_str ("FINALLY, CHERIL MANAGED TO");
	       _y = 20; pr_str (" ESCAPE FROM CASTLE MOJON");
	       _y = 22; pr_str ("WHAT NEW ADVENTURES AWAIT?");
	       _y = 24; pr_str ("   THANKS FOR PLAYING!!");
}
