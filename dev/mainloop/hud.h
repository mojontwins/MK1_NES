			// Update frame
#ifdef PLAYER_SHOW_KILLS
			if (okilled != pkilled) {
				okilled = pkilled;
				p_t (KILLED_X, KILLED_Y, baddies_count - pkilled);
			}
#endif
#ifndef DEACTIVATE_OBJECTS
			if (oobjs != pobjs) {
				oobjs = pobjs;
				p_t (OBJECTS_X, OBJECTS_Y, PLAYER_MAX_OBJECTS - pobjs);
			}
#endif
#ifndef DEACTIVATE_KEYS
			if (okeys != pkeys) {
				okeys = pkeys;
				p_t (KEYS_X, KEYS_Y, pkeys);
			}
#endif
			if (olife != plife) {
				olife = plife;
				p_t (LIFE_X, LIFE_Y, plife);
			}
#ifdef MAX_AMMO
			if (oammo != pammo) {
				oammo = pammo;
				p_t (AMMO_X, AMMO_Y, pammo);
			}
#endif
			