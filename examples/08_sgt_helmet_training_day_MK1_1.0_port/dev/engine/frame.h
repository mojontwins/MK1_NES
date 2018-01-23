// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// frame.h
// Custom game frame

void __fastcall__ draw_game_frame (void) {
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
