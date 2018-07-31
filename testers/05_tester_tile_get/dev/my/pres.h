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

const unsigned char level_name_0 [] = "BENNY HILL[]]]";
const unsigned char level_name_1 [] = "BROKEN FRIDGE[";
const unsigned char level_name_2 [] = "BOSQUE BADAJOZ";
const unsigned char level_name_3 [] = "ESTO ES RAWRO[";

const unsigned char * const level_names [] = {
	level_name_0, level_name_1, level_name_2, level_name_3
};

void scr_level (void) {
	pal_spr (palss0);

	_x =  9; _y = 15; pr_str ((unsigned char *) level_names [level_world]);
	         _y = 16; pr_str ("]]^ZONE, ACT ");
	vram_put (level_act + 17);

	oam_index = 0; rdx = 128-44; for (gpit = 0; gpit < 4; gpit ++) {
		oam_index = oam_meta_spr (
			rdx, 18*8, oam_index, pemmeralds & bits [gpit] ? ssit_07 : ssit_03
		);
		rdx += 24;
	}
}

void scr_title (void) {
	_x =  6; _y = 10; pr_str ("CONGRATS, IT WORKED!");
	_x = 10; _y = 21; pr_str ("PRESS START!");
}

void scr_game_over (void) {
	_x = 11; _y = 15; pr_str ("GAME OVER!");
}

void scr_the_end (void) {
	_x = 12; _y = 15; pr_str ("THE END");
}
