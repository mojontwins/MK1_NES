					if (!en_status [gpit] || half_life) {
						ptx1 = en_x [gpit];
						pty1 = en_y [gpit];
						en_x [gpit] += en_mx [gpit];
						en_y [gpit] += en_my [gpit];

#ifdef WALLS_STOP_ENEMIES
						// Collision. Acts as "everything is a wall". Refine if needed for future games!
						
						en_colly = 0;
						if (en_my [gpit]) {
							etx1 = en_x [gpit] >> 4;
							etx2 = (en_x [gpit] + 15) >> 4;
							if (en_my [gpit] < 0) {
								ety1 = en_y [gpit] >> 4;	
							} else /*if (en_my [gpit] > 0)*/ {
								ety1 = (en_y [gpit] + 15) >> 4;
							}						
							if ((attr (etx1, ety1) & 8) + (attr (etx2, ety1) & 8)) {
								en_colly = 1;
								en_y [gpit] = pty1;
							}				
						}
						
						en_collx = 0;
						if (en_mx [gpit]) {
							ety1 = en_y [gpit] >> 4;
							ety2 = (en_y [gpit] + 15) >> 4;
							if (en_mx [gpit] < 0) {
								etx1 = en_x [gpit] >> 4;
							} else /*if (en_mx [gpit] > 0)*/ {
								etx1 = (en_x [gpit] + 15) >> 4;
							}
							if ((attr (etx1, ety1) & 8) + (attr (etx1, ety2) & 8)) {
								en_collx = 1;
								en_x [gpit] = ptx1;
							}
						}
#endif
						
#ifdef WALLS_STOP_ENEMIES
						if (en_x [gpit] == en_x1 [gpit] || en_x [gpit] == en_x2 [gpit] || en_collx) en_mx [gpit] = -en_mx [gpit];
						if (en_y [gpit] == en_y1 [gpit] || en_y [gpit] == en_y2 [gpit] || en_colly) en_my [gpit] = -en_my [gpit];
#else
						if (en_x [gpit] == en_x1 [gpit] || en_x [gpit] == en_x2 [gpit]) en_mx [gpit] = -en_mx [gpit];
						if (en_y [gpit] == en_y1 [gpit] || en_y [gpit] == en_y2 [gpit]) en_my [gpit] = -en_my [gpit];						
#endif						
					}
					en_facing = (en_mx [gpit] > 0 || en_my [gpit] > 0) ? 0 : 2;
					oam_meta_spr (
						en_x [gpit], en_y [gpit] + SPRITE_ADJUST, 
						OAM_ENEMS + (gpit << 4), 
						spr_enems [en_s [gpit] + en_fr + en_facing]);
