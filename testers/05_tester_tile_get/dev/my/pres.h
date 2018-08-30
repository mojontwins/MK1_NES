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
}

//                                     XXXXXXXXXXXXXXXX
const unsigned char level_name_0 [] = "BENNY HILL[]]]]]";
const unsigned char level_name_1 [] = "BROKEN FRIDGE[]]";
const unsigned char level_name_2 [] = "BOSQUE BADAJOZ[]";
const unsigned char level_name_3 [] = "WET RUINS[]]]]]]";
const unsigned char level_name_4 [] = "CRAP BRAIN[]]]]]";
const unsigned char level_name_5 [] = "OLA K ASE[]]]]]]";

const unsigned char * const level_names [] = {
	level_name_0, level_name_1, level_name_2, level_name_3, level_name_4, level_name_5
};

void scr_level (void) {
	pal_spr (palss0);

	_x =  8; _y = 15; pr_str ((unsigned char *) level_names [level_world]);
	         _y = 16; pr_str ("]]]]^ZONE, ACT ");
	vram_put (level_act + 17);

	oam_index = 0; rdx = 128-32; rdy = 17*8; for (gpit = 0; gpit < 6; gpit ++) {
		oam_index = oam_meta_spr (
			rdx, rdy, oam_index, pemmeralds & bits [gpit] ? ssit_07 : ssit_03
		);
		rdx += 24; if (gpit == 2) { rdx = 128-32; rdy += 24; }
	}
}

void title (void) {
	bankswitch (1);
	unrle_vram (title_rle, 0x2000);
	pal_bg (paltstitle);
	if (first_game) {
		_x = 10; _y = 20; pr_str ("PRESS START!");
	} else {
		_x = 4; _y = 19; pr_str ("PRESS START  TO BEGIN%%PRESS SELECT TO CONTINUE");
	}
	_x = 5;  _y = 26; pr_str ("@ 2018 THE MOJON TWINS"); 

	bat_in ();
	while (1) {
		pad_read ();
		if (pad_this_frame & PAD_START) {
			level = 0;
			pemmeralds = 0;
			break;
		}
		if (pad_this_frame & PAD_SELECT) {
			level = base_level [level];
			break;
		}
	}
	sfx_play (SFX_START, 0);
	bat_out ();	
	bankswitch (0);
}

void scr_game_over (void) {
	_x = 11; _y = 15; pr_str ("GAME OVER!");
}

void scr_the_end (void) {
	_x = 12; _y = 15; pr_str ("THE END");
}

const unsigned char * const cuts_rle [] = {
	cuts0_rle, cuts2_rle, cuts2_rle, cuts1_rle, cuts1_rle
};

const unsigned char * const cuts_pal [] = {
	palcuts0, palcuts2, palcuts2, palcuts1, palcuts1
};

void scr_cutscene (void) {
	// show cuts + text in rda;
	unrle_vram (cuts_rle [rdm], 0x2000);
	_x = 2; _y = 18; pr_str ((unsigned char *) cutscenes [rdm]);
	//music_play (MUSIC_CUTS);
}

void cutscene (void) {
	bankswitch (1);
	pres (cuts_pal [rdm], scr_cutscene);
	bankswitch (0);
}
