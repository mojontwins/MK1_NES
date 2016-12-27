			// Hotspot interaction
			if (hrt) {
				/*
				if (hrt == HOTSPOT_TYPE_RESONATOR) {
					if (res_on == 0) {
						if (prx + 12 >= hrx && prx <= hrx + 12 && pry + 15 >= hry && pry + 8 <= hry && pvy >= PLAYER_G << 1) {
							pvy = -PLAYER_VY_JUMP_MAX;

							// res_on, start ct...
							res_on = 1; 
							res_ct = 9;
							res_subct = 50;

							// Black and white, blueish.
							//ppu_mask (0x9f);
							pal_bg (mypal_bw);

							// Change tile to "active"
							update_list_tile (hrx >> 3, TOP_ADJUST + (hry >> 3), 20);
						}
					}
				} else */if (collide_in (prx + 8, pry + 8, hrx, hry)) {
					map_set (hrx >> 4, hry >> 4, 0);
					switch (hrt) {
#ifndef DEACTIVATE_OBJECTS
						case HOTSPOT_TYPE_OBJECTS:
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
						case case HOTSPOT_TYPE_AMMO:
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
			