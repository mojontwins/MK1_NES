// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// Hotspot interaction

if (hrt) {
	hotspots_paint ();

	if (collide_in (prx + 4, pry + 8, hrx, hry)) {
#ifdef CARRY_ONE_HS_OBJ
		// This was custom code used in Goddess and is not
		// very general, plus it is incomplete. So off
		/*
		if (!(i & PAD_B)) pfiring = 0;

		if ((i & PAD_B) && !pfiring && ppossee ) {
			gpjt = 0;
			pfiring = 1;
			
			if (hrt >= HS_INV_MIN && hrt <= HS_INV_MAX) {
				rda = pinv; pinv = hrt; hrt = rda; ht [n_pant] = hrt;
				sfx_play (3, 1);
				oam_index = oam_meta_spr (
					hrx, hry + SPRITE_ADJUST, 
					oam_index, 
					spr_hs [hrt - 1]
				);
			}

			if (gpjt) {
				// Show NO.
				no_ct = 50;
				sfx_play (4, 0);
			}
		}

		if (hrt < HS_INV_MIN) 
		*/
#endif					
		{
			switch (hrt) {
				#ifndef DEACTIVATE_OBJECTS
					case HOTSPOT_TYPE_OBJECT:
						pobjs ++;
						rda = 2;
						break;
				#endif
				#ifndef DEACTIVATE_KEYS
					case HOTSPOT_TYPE_KEYS:
						pkeys ++;
						rda = 2;
						break;
				#endif
					case HOTSPOT_TYPE_REFILL:
						plife += PLAYER_REFILL;
						#ifdef LIMIT_LIFE
							if (plife > PLAYER_LIFE) plife = PLAYER_LIFE;
						#endif
						rda = 3;
						break;
				#ifdef MAX_AMMO
					case HOTSPOT_TYPE_AMMO:
						rda = 2;
						if (MAX_AMMO - pammo > AMMO_REFILL)
							pammo += AMMO_REFILL;
						else
							pammo = MAX_AMMO;
						break;
				#endif
				// CUSTOM {
					case HOTSPOT_TYPE_BOOT:
						gp_gen = text_boots;
						textbox_do ();
						has_boots = 1;
						break;

					case HOTSPOT_TYPE_SIGN:
						gp_gen = text_intro;
						textbox_do ();
						break;
				// } END_OF_CUSTOM
			}

			// CUSTOM {
				/*
				sfx_play (rda, 1);
				hrt = 0;
				hact [n_pant] = 0;
				*/
				if (hrt != HOTSPOT_TYPE_SIGN) {
					hry = 240;
				} else {
					sfx_play (rda, 1);
					hrt = 0;
					hact [n_pant] = 0;	
				}
			// } END_OF_CUSTOM
		}
	}
}
