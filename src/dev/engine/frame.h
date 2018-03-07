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
		_x = KILLED_X; _y = KILLED_Y; _n = c_max_enems - pkilled; p_t ();
	}
#endif

#if !defined (DEACTIVATE_OBJECTS) && defined (OBJECTS_X)
	if (oobjs != pobjs) {
		oobjs = pobjs;
		_x = OBJECTS_X; _y = OBJECTS_Y; _n = pobjs; p_t ();
	}
#endif

#if !defined (DEACTIVATE_KEYS) && defined (KEYS_X)
	if (okeys != pkeys) {
		okeys = pkeys;
		_x = KEYS_X; _y = KEYS_Y; _n = pkeys; p_t ();
	}
#endif

#if defined (LIFE_X)			
	if (olife != plife) {
		olife = plife;
		_x = LIFE_X; _y = LIFE_Y; _n = plife; p_t ();
	}
#endif

#if defined (MAX_AMMO) && defined (AMMO_X)
	if (oammo != pammo) {
		oammo = pammo;
		_x = AMMO_X; _y = AMMO_Y; _n = pammo; p_t ();
	}
#endif

#if defined (HS_INV_X)
	#if defined (ENABLE_CONTAINERS) && defined (FLAG_INVENTORY)
		oam_index = oam_meta_spr (
			HS_INV_X, HS_INV_Y,
			oam_index,
			spr_hs [flags [FLAG_INVENTORY]]
		);
	#else
		oam_index = oam_meta_spr (
			HS_INV_X, HS_INV_Y,
			oam_index,
			spr_hs [pinv]
		);
	#endif
#endif

#if defined (ENABLE_TIMER) && defined (TIMER_X)
	if (otimer != timer) {
		otimer = timer;
		_x = TIMER_X; _y = TIMER_Y; _n = timer; p_t ();
	}
#endif
}
