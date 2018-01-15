					switch (en_alive [gpit]) {
						case 0:
							// IDLE
							if (en_ct [gpit]) {
								en_ct [gpit] --;
								en_y [gpit] = 240;
								oam_meta_spr (0, 240, ENEMS_OAM_BASE + (gpit << 4), spr_empty);
							} else {
								en_alive [gpit] = 1;
								en_x [gpit] = en_x1 [gpit];
								en_y [gpit] = en_y1 [gpit];
								en_rawv [gpit] = 1 << (rand8 () % 5);
								if (en_rawv [gpit] > 4) en_rawv [gpit] = 1;
								if (en_rawv [gpit] == 1) en_status [gpit] = 1; else en_rawv [gpit] >>= 1;
								en_ct [gpit] = 50 + (rand8 () & 31);
								oam_meta_spr (en_x [gpit], en_y [gpit] + SPRITE_ADJUST, ENEMS_OAM_BASE + (gpit << 4), spr_en_0A);
							}
							break;
						case 1:
							// Appearing
							if (en_ct [gpit]) {
								en_ct [gpit] --;
							} else {
								en_alive [gpit] = 2;
							}
							break;
						case 2:
							// Pursuing
							if (pstate == EST_NORMAL && (!en_status [gpit] || half_life)) {
								en_mx [gpit] = add_sign (((prx >> 2) << 2) - en_x [gpit], en_rawv [gpit]);
								en_my [gpit] = add_sign (((pry >> 2) << 2) - en_y [gpit], en_rawv [gpit]);

								en_y [gpit] += en_my [gpit];
#ifdef WALLS_STOP_ENEMIES
								// Collision detection
#ifdef BOUNDING_BOX_8_BOTTOM
								etx1 = (en_x [gpit] + 4) >> 4;
								etx2 = (en_x [gpit] + 11) >> 4;
#else
								etx1 = en_x [gpit] >> 4;
								etx2 = (en_x [gpit] + 15) >> 4;
#endif								
								if (en_my [gpit] < 0) {
#ifdef BOUNDING_BOX_8_BOTTOM
									ety1 = (en_y [gpit] + 8) >> 4;
#else																		
									ety1 = en_y [gpit] >> 4;
#endif									
									if (attr (etx1, ety1) || attr (etx2, ety1)) 
#ifdef BOUNDING_BOX_8_BOTTOM
										en_y [gpit] = ((ety1 + 1) << 4) - 8;
#else									
										en_y [gpit] = (ety1 + 1) << 4;
#endif										
								} else if (en_my [gpit] > 0) {
									ety1 = (en_y [gpit] + 15) >> 4;
									if (attr (etx1, ety1) || attr (etx2, ety1))
										en_y [gpit] = (ety1 - 1) << 4;
								}
#endif							
								en_x [gpit] += en_mx [gpit];
#ifdef WALLS_STOP_ENEMIES
								// Collision detection
#ifdef BOUNDING_BOX_8_BOTTOM
								ety1 = (en_y [gpit] + 8) >> 4;
								ety2 = (en_y [gpit] + 15) >> 4;
#else								
								ety1 = en_y [gpit] >> 4;
								ety2 = (en_y [gpit] + 15) >> 4;
#endif								
								if (en_mx [gpit] < 0) {
#ifdef BOUNDING_BOX_8_BOTTOM									
									etx1 = (en_x [gpit] + 4) >> 4;
#else
									etx1 = en_x [gpit] >> 4;
#endif									
									if (attr (etx1, ety1) || attr (etx1, ety2))
#ifdef BOUNDING_BOX_8_BOTTOM
										en_x [gpit] = ((etx1 + 1) << 4) - 4;
#else									
										en_x [gpit] = (etx1 + 1) << 4;	
#endif										
								} else if (en_mx [gpit] > 0) {
#ifdef BOUNDING_BOX_8_BOTTOM
									etx1 = (en_x [gpit] + 11) >> 4;
#else									
									etx1 = (en_x [gpit] + 15) >> 4;
#endif									
									if (attr (etx1, ety1) || attr (etx1, ety2))
#ifdef BOUNDING_BOX_8_BOTTOM
										en_x [gpit] = ((etx1 - 1) << 4) + 4;
#else									
										en_x [gpit] = (etx1 - 1) << 4;
#endif										
								}
#endif
	
							}
							oam_meta_spr (en_x [gpit], en_y [gpit] + SPRITE_ADJUST, ENEMS_OAM_BASE + (gpit << 4), spr_enems [(en_s [gpit] << 1) + en_fr]);
							break;
					}					
#ifdef ENABLE_GENERATORS
					// Paint generator
					if (en_generator_life [gpit]) {
						oam_meta_spr (en_x1 [gpit], en_y1 [gpit] + SPRITE_ADJUST, (gpit << 4), spr_enems [GENERATOR_BASE_SPRITE + (gen_was_hit [gpit] > 0)]);
						if (gen_was_hit [gpit]) gen_was_hit [gpit] --;
					}
#endif
