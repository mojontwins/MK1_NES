// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Resonators code.
if (res_on) {

	// Resonator on screen, paint countdown
	if (hrt == HOTSPOT_TYPE_RESONATOR) {
		oam_index = oam_spr (
			hrx + RESONATOR_COUNTER_OFFS_X, 
			hry + RESONATOR_COUNTER_OFFS_Y + SPRITE_ADJUST, 
			RESONATOR_BASE_PATTERN + res_ct, 
			RESONATOR_PALETTE, 
			oam_index);
	} 

	// Resonator logic
	if (res_subct) res_subct --; else {
		res_subct = 50;
		if (res_ct) res_ct --; else {
			res_on = 0;

			#ifdef RESONATOR_CHANGE_BG_PAL
				pal_bg (c_pal_bg);
			#endif

			#ifdef RESONATOR_CHANGE_SPR_PAL
				pal_spr (c_pal_fg);
			#endif
		}
	}
}
