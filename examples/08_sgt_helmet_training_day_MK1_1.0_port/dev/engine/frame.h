// NES MK1 v0.7
// Copyleft Mojon Twins 2013, 2015, 2016

// frame.h
// Custom game frame

/*
const unsigned char game_frame [] = {
	 64,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  65,
	 69,   0,   3,  29,  16,  16,   0,   0,   0,   0,   0,   0,   0,  47,  34,  42,  29,   0,   0,   0,   0,   0,   0,   0,   0,   0,   5,  29,  16,  16,   0,  71,
	 66,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  67
};
*/

void __fastcall__ draw_game_frame (void) {
	/*
	vram_write ((unsigned char *) game_frame, 0x2360, 96);
	for (rdit = 0; rdit < 16; rdit ++) attr_table [48 + rdit] = 0x00;
	*/

	unrle_vram (frame_rle, 0x2000);
	memfill (attr_table, 0xff, 64);
}

void hud_update (void) {
	// Update frame
#ifdef KILLED_X
	if (okilled != pkilled) {
		okilled = pkilled;
		p_t (KILLED_X, KILLED_Y, baddies_count - pkilled);
	}
#endif

#if !defined (DEACTIVATE_OBJECTS) && defined (OBJECTS_X)
	if (oobjs != pobjs) {
		oobjs = pobjs;
		p_t (OBJECTS_X, OBJECTS_Y, pobjs);
	}
#endif

#if !defined (DEACTIVATE_KEYS) && defined (KEYS_X)
	if (okeys != pkeys) {
		okeys = pkeys;
		p_t (KEYS_X, KEYS_Y, pkeys);
	}
#endif

#if defined (LIFE_X)			
	if (olife != plife) {
		olife = plife;
		p_t (LIFE_X, LIFE_Y, plife);
	}
#endif

#if defined (MAX_AMMO) && defined (AMMO_X)
	if (oammo != pammo) {
		oammo = pammo;
		p_t (AMMO_X, AMMO_Y, pammo);
	}
#endif
}
