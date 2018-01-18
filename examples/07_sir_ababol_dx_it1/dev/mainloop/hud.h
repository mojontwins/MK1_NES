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
