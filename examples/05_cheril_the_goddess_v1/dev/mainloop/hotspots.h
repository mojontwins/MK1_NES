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
							oam_meta_spr (hrx, hry + SPRITE_ADJUST, 176, spr_hs [hrt - 1]);
						} else if (hrt >= HS_INV_MIN + HS_USE_OFFS && hrt <= HS_INV_MAX + HS_USE_OFFS) {				
							if (pinv == hrt - HS_USE_OFFS) {
								if (pinv == 7) {
									// Using key in bolt
									hact [n_pant] = 0;
									sfx_play (1, 1);
									fx_flash ();
									map_set (7, 8, 0);
									oam_meta_spr (0, 240, 176, spr_empty);
									hry = 240;
								} else {
									music_stop ();
									use_ct = 1; pctfr = 0;
								}
							} else gpjt = 1;
						} else if (hrt == 18 && !tt_ct) {
							if (ppodewwwr) {
								tt_ct = 1;
								music_stop ();
								sfx_play (2, 1);
								c_pal_bg_backup = c_pal_bg;
								c_pal_bg = (unsigned char *) mypal_reds;
								fx_flash ();
							} else gpjt = 1;
						} else if (hrt == 19) {
							if (ppodewwwr < 3) {
								ppodewwwr = 3;
								sfx_play (3, 1);
								fx_flash ();
								guay_ct = 32;
							} else gpjt = 1;
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
						oam_meta_spr (0, 240, 176, spr_empty);
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
			