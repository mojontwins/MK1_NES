// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// No!
if (no_ct) {
	-- no_ct;
	oam_index = oam_meta_spr (
		prx + NO_OFFS_X, pry + NO_OFFS_Y + SPRITE_ADJUST,
		oam_index,
		NO_METASPRITE
	);
}
