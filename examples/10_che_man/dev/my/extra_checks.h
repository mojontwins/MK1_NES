// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Add here your code.  This gets run right before checking
// for the level's end condition. The previous frame is on
// screen. 

// Do whatever you need. Everything you add here gets executed
// each frame!

// Whenever you want to WIN the game just set win_level = 1
// (Don't forget to define WIN_LEVEL_CUSTOM in config.h)

win_level = (pobjs == MAX_HOTSPOTS_TYPE_1_0 && pkilled == MAX_ENEMS_TYPE_3_0);

if (n_pant == 2 || n_pant == 7 || n_pant == 12 || n_pant == 17) {
	if (flags [ONLY_ONE_OBJECT_FLAG] && (half_life)) {
		oam_index = oam_meta_spr (
			112, 183, 
			oam_index, 
			ssit_04
		);
	}
}

if (first_game) {
	gp_gen = 0;

	if (ppossee && first_text) {
		first_text = 0;
		gp_gen = custom_text0;
	}

	if (pkilled && first_batucada) {
		first_batucada = 0;
		gp_gen = custom_text1;
	}

	if (flags [ONLY_ONE_OBJECT_FLAG] && first_medallion) {
		first_medallion = 0;
		gp_gen = custom_text2;
	}

	if (pobjs && first_totem) {
		first_totem = 0;
		gp_gen = custom_text3;
	}

	if (gp_gen) {
		oam_index = 4;
		player_render ();
		hud_update ();
		oam_hide_rest (oam_index); 
		oam_index = 4;
		textbox_do ();	
	}
}
