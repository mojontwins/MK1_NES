// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

void fire_bullet (void) {
	#ifdef MAX_AMMO
		if (!pammo) return;
		pammo --;
	#endif
	// Creates a new bullet (if possible);
	for (bi = 0; bi < MAX_BULLETS; bi ++) {
		if (bst [bi] == 0) {
			bst [bi] = 1;

			#ifdef PLAYER_TOP_DOWN
				switch (pfacing) {
					case CELL_FACING_LEFT:
						bx [bi] = prx - 4;
						bmx [bi] = -PLAYER_BULLET_SPEED;
						by [bi] = pry + PLAYER_BULLET_Y_OFFSET;
						bmy [bi] = 0;
						break;	
					case CELL_FACING_RIGHT:
						bx [bi] = prx + 12;
						bmx [bi] = PLAYER_BULLET_SPEED;
						by [bi] = pry + PLAYER_BULLET_Y_OFFSET;
						bmy [bi] = 0;
						break;
					case CELL_FACING_DOWN:
						bx [bi] = prx + PLAYER_BULLET_X_OFFSET;
						by [bi] = pry + 12;
						bmy [bi] = PLAYER_BULLET_SPEED;
						bmx [bi] = 0;
						break;
					case CELL_FACING_UP:
						bx [bi] = prx + 8 - PLAYER_BULLET_X_OFFSET;
						by [bi] = pry - 4;
						bmy [bi] = -PLAYER_BULLET_SPEED;
						bmx [bi] = 0;
						break;
				}
			#else
				by [bi] = pry + PLAYER_BULLET_Y_OFFSET;
				
				if (pfacing) {
					// Left
					bx [bi] = prx - 4;
					bmx [bi] = -PLAYER_BULLET_SPEED;
				} else {
					// Right
					bx [bi] = prx + 12;
					bmx [bi] = PLAYER_BULLET_SPEED;
				}
			#endif		
			
			// sound
			sfx_play (5, 0);
			// done
			break;
		}
	}
}

void bullets_move (void) {
	for (bi = 0; bi < MAX_BULLETS; bi ++) {
		if (bst [bi]) {
			if (bmx [bi]) {
				bx [bi] += bmx [bi];

				if (
					bx [bi] < PLAYER_BULLET_SPEED ||
					bx [bi] > 255 - PLAYER_BULLET_SPEED
				) bst [bi] = 0; 
			
			}

			#ifdef PLAYER_TOP_DOWN		
				if (bmy [bi]) {
					by [bi] += bmy [bi];
					if (
						by [bi] < PLAYER_BULLET_SPEED ||
						by [bi] > 191 - PLAYER_BULLET_SPEED
					) bst [bi] = 0; 
				}
			#endif
			
			cx1 = (bx [bi] + 3) >> 4; 
			cy1 = (by [bi] + 3) >> 4;
			cm_two_points ();

			#ifdef BREAKABLE_WALLS
				if (at1 & 16) {
					bst [bi] = 0;
					sfx_play (6, 2);
					break_wall (cx1, cy1 - 1);
				} else 
			#endif			
			
			if (at1 & 8) {
				bst [bi] = 0;
				sfx_play (6, 2);
			}
			
			// Collide with enemies?			
			for (gpjt = 0; gpjt < 3; gpjt ++) {
				#ifdef FIRE_MIN_KILLABLE
					if (en_t [gpjt] >= FIRE_MIN_KILLABLE)
				#else
					if (en_t [gpjt])
				#endif
				{
					#ifdef ENABLE_PURSUERS
						if (en_t [gpjt] != 7 || en_alive [gpjt] == 2)
					#endif
					
					if (collide_in (bx [bi] + 3, by [bi] + 3, en_x [gpjt], en_y [gpjt])) {
						
						en_cttouched [gpjt] = 8;
						bst [bi] = 0;
						sfx_play (6, 2);
						en_life [gpjt] --;						

						if (en_life [gpjt] == 0) {	

							#ifdef ENABLE_PURSUERS
								if (en_t [gpjt] == 7) {
									en_alive [gpjt] = 0;
									en_ct [gpjt] = DEATH_COUNT_EXPRESSION;
									en_life [gpjt] = ENEMIES_LIFE_GAUGE;
								} else
							#endif
							en_t [gpjt] = 0;		

							pkilled ++;												
						}
						break;
					}
				}	
			}
						
			oam_index = oam_spr (
				bx [bi], SPRITE_ADJUST + by [bi], 
				BULLET_PATTERN, BULLET_PALETTE,
				oam_index
			);
		}
	}
}
