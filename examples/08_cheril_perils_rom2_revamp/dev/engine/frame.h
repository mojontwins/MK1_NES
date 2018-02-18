// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// frame.h
// Custom game frame

#include "my/game_frame.h"

void hud_update (void) {
	// Update frame
#ifdef KILLED_X
	if (okilled != pkilled) {
		okilled = pkilled;
		p_t (KILLED_X, KILLED_Y, c_max_enems - pkilled);
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

#if defined (ENABLE_EASY_OBJECTS) && defined (HS_INV_X)
	oam_index = oam_meta_spr (
		HS_INV_X, HS_INV_Y,
		oam_index,
		spr_hs [pinv]
	);
#endif

}