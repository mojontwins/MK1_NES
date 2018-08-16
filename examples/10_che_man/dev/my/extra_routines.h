// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Extra routines.
// Add here calls to routines which need to update sprites and/or bg.

if ( 
	(level == 0 && (n_pant == 2 || n_pant == 7 || n_pant == 12 || n_pant == 17)) ||
	(level == 1 && n_pant == 18)
) {
	if (flags [ONLY_ONE_OBJECT_FLAG] && (half_life)) {
		oam_index = oam_meta_spr (
			112, level == 1 ? 135 : 183, 
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
		update_cycle ();
		oam_index = 4;
		textbox_do ();	
	}
}
