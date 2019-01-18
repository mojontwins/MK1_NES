// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Cutscenes, title screen, etc.

void bat_in (void) {
	pal_bright (0);
	ppu_on_all ();
	//while (pad_poll (0));
	fade_delay = 4;
	fade_in ();
}

void bat_out (void) {	
	music_stop ();
	fade_out ();
	oam_clear ();
	ppu_off ();
}

void pres (void) {
	cls ();
	pal_bg (gp_gen);
	(*gp_func) ();
	bat_in ();
	while (1) {
		pad_read ();
		if (pad_this_frame & (PAD_A|PAD_B|PAD_START)) break;
	}
	bat_out ();
}

const unsigned char level_name_0 [] = "BENNY HILL";
const unsigned char level_name_1 [] = "BROKEN FRIDGE";
const unsigned char level_name_2 [] = "BOSQUE BADAJOZ";
const unsigned char level_name_3 [] = "WET RUINS";
const unsigned char level_name_4 [] = "CRAP BRAIN";
const unsigned char level_name_5 [] = "OLA K ASE";

const unsigned char * const level_names [] = {
	level_name_0, level_name_1, level_name_2, level_name_3, level_name_4, level_name_5
};

void show_emeralds (void) {
	// Set rdy before calling!

	oam_index = 0; rdx = 128-32; for (gpit = 0; gpit < 6; ++ gpit) {
		oam_index = oam_meta_spr (
			rdx, rdy, oam_index, pemmeralds & bits [gpit] ? ssit_07 : ssit_03
		);
		rdx += 24; if (gpit == 2) { rdx = 128-32; rdy += 24; }
	}
}

void scr_level (void) {
	pal_spr (palss0);

	_x =  8; _y = 15; pr_str ("]]]]]]]]]]]]]]]]");
					  pr_str ((unsigned char *) level_names [level_world]);
					  vram_put ('['-32);
	         _y = 16; pr_str ("]]]]^ZONE, ACT ");
	vram_put (level_act + 17);

	rdy = 17*8; show_emeralds ();
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
	music_play (MUSIC_TITLE);
	while (1) {
		pad_read ();
		if (pad_this_frame & PAD_START) {
			level = 0;
			pemmeralds = 0;
			free_play = 0;
			break;
		}
		if (!first_game && (pad_this_frame & PAD_SELECT)) {
			select_level = free_play;
			break;
		}
	}
	music_stop ();
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
	scroll (0, 16);
	gp_gen = cuts_pal [rdm]; gp_func = scr_cutscene; pres ();	
	//pres (cuts_pal [rdm], scr_cutscene);
	//pres (cuts_pal [rdm], scr_the_end);
	bankswitch (0);
}

void zone_select (void) {
	pal_spr (palss0);
	pal_bg (paltstitle);

	cls ();
	_x = 11; _y = 5; pr_str ("ZONE SELECT");
	for (gpjt = 0; gpjt < 6; ++ gpjt) {
		_y = 8 + (gpjt << 1); pr_str ((unsigned char *) level_names [gpjt]);
	}
	
	rdy = 21 * 8; show_emeralds ();
	scroll (0, 0);

	bat_in ();
	music_play (MUSIC_CUTS);
	while (1) {
		oam_spr (72, 63 + (base_world [level] << 4), COCO_PATTERN, COCO_PALETTE, oam_index);
		pad_read ();
		rda = level;
		if (pad_this_frame & PAD_DOWN) {
			level += 3; if (level >= 18) level = 0;
		}
		if (pad_this_frame & PAD_UP) {
			if (level > 2) level -= 3; else level = 15;
		}
		if (rda != level) sfx_play (SFX_HITTER, 1);
		if (pad_this_frame & (PAD_A|PAD_B|PAD_START)) break;		
		ppu_waitnmi ();
	}
	sfx_play (SFX_START, 1);
	bat_out ();

	level = base_level [level];
}
