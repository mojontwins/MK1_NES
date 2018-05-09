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
	cls ();
	pal_bg (palts0);
	pal_spr (palss0);

	unrle_vram (title_rle, 0x2000);
	/*
	_x = 14; _y = 19; pr_str ("GAME A");
	       ; _y = 21; pr_str ("GAME B");
	       ; _y = 23; pr_str ("GAME C");
	*/
	_x = 10; _y = 21; pr_str ("PRESS START!");
	_x =  5; _y = 26; pr_str ("@ 2018 THE MOJON TWINS"); 

	bat_in ();

	while (1) {
		// oam_spr (96, 138 + (level << 4), 63, 2, 0);
		ppu_waitnmi ();
		pad_read ();
		// rda = level;
		if (pad_this_frame & (PAD_A|PAD_B|PAD_START)) break;
		/*
		if (pad_this_frame & (PAD_SELECT|PAD_DOWN)) {
			level ++; if (level == 3) level = 0;
		}
		if (pad_this_frame & PAD_UP) {
			if (level) level --; else level = 2;
		}
		if (level != rda) sfx_play (SFX_USE, 0);
		*/
	}
	sfx_play (SFX_START, 0); delay (20);

	bat_out ();
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
	cuts0_rle, ending_rle
};

void scr_cutscene (void) {
	// show cuts + text in level;
	unrle_vram (cuts_rle [rda>2], 0x2000);
	_x = 2; _y = 16; pr_str ((unsigned char *) cutscenes [rda]);	

	//_x = 13; _y = 4; pr_str ("GAME "); vram_put (level + 33);
	_x = 12; _y = 4; pr_str ("LEVEL 0"); vram_put (level + 17);

	music_play (1);
}

#define TOP 68
void scr_instructions (void) {
	_x = 4; _y = 6; pr_str (
		"     INSTRUCTIONS%%%%"
		"CHERIL IS IN A BOOT SO SHE%"
		"CAN'T STOP JUMPING!%%%%"
		"PRESS [DOWN] TO MAKE CHERIL%"
		"CROUCH AND JUMP LOWER%%%%"
		"PRESS [B] TO CHARGE, THEN%"
		"RELEASE TO SPIT!%%%%%"
		"   PRESS ANY BUTTON"
	);
	oam_index = oam_meta_spr (12, TOP, 0, sspl_00_a);
	oam_index = oam_meta_spr (12, TOP + 40, oam_index, sspl_05_a);
	oam_index = oam_meta_spr (12, TOP + 80, oam_index, sspl_06_a);
}

void credits (void) {
	bankswitch (1);
	pal_bg (palts0);
	pal_spr (palss0);
	cls ();
	oam_clear (); scroll (0, 0);
	
	rds16 = 0; rdy = 240;

	_x = 0; _y = 22; 
	pr_str ("    BOOTEE - STARRING CHERIL%%COPYLEFT 2018 BY THE MOJON TWINS%DESIGN & PORT BY THE MOJON TWINS%   NESLIB & FAMITONE BY SHIRU%       NES OGT BY DAVIDIAN");
	
	pal_bright (0);
	ppu_on_all ();
	fade_delay = 4;
	fade_in ();
	while (!(pad_poll (0) & PAD_START) && rds16 < 300) {
		oam_meta_spr (102, rdy, 0, logo_00);
		if (rdy > 112) rdy --;
		ppu_waitnmi ();
		rds16 ++;
	};
	fade_out ();
	
	ppu_off ();
	oam_clear ();
	bankswitch (0);
}
