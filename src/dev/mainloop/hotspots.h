			// Hotspot interaction
			if (hrt) {
				if (collide_in (prx + 8, pry + 8, hrx, hry)) {
#ifdef CARRY_ONE_HS_OBJ
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
#endif					
					{
						switch (hrt) {
#ifndef DEACTIVATE_OBJECTS
							case HOTSPOT_TYPE_OBJECT:
								pobjs ++;
								sfx_play (3, 1);
								break;
#endif
#ifndef DEACTIVATE_KEYS
							case HOTSPOT_TYPE_KEYS:
								pkeys ++;
								sfx_play (2, 1);
								break;
#endif
							case HOTSPOT_TYPE_REFILL:
								plife += PLAYER_REFILL;
								if (plife > PLAYER_LIFE) plife = PLAYER_LIFE;
								sfx_play (3, 1);
								break;
#ifdef MAX_AMMO
							case HOTSPOT_TYPE_AMMO:
								sfx_play (2, 1);
								if (MAX_AMMO - pammo > AMMO_REFILL)
									pammo += AMMO_REFILL;
								else
									pammo = MAX_AMMO;
								break;
#endif
						}
						hry = 240;
						hact [n_pant] = 0;
					}
				}
			}
			