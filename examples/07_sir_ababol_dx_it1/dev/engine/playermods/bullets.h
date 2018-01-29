// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

void fire_bullet (void) {
	#ifdef MAX_AMMO
		if (!pammo) return;
		pammo --;
	#endif

	// Creates a new bullet (if possible):

	if (b_slots_i == 0) return;
	b_slots_i --; bi = b_slots [b_slots_i];

	bst [bi] = 1;
	
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
		#ifdef PLAYER_TOP_DOWN
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
		#endif
	}	
}

void bullets_destroy (void) {
	bst [bi] = 0;
	b_slots [b_slots_i] = bi; b_slots_i ++;
}

void bullets_move (void) {
	for (bi = 0; bi < MAX_BULLETS; bi ++) {
		if (bst [bi]) {
			bx [bi] += bmx [bi];
			by [bi] += bmy [bi];

			oam_index = oam_spr (
				bx [bi], SPRITE_ADJUST + by [bi], 
				BULLET_PATTERN, BULLET_PALETTE,
				oam_index
			);

			cx1 = ((bx [bi] + 4) >> 4);
			cy1 = ((by [bi] + 4 - 16) >> 4);
			rdm = map_attr [COORDS (cx1, cy1)];

			#ifdef ENABLE_BREAKABLE
				if (rdm & 16) {
					breakable_break (cx1, cy1);
					bullets_destroy ();
				} else
			#endif
			if (
				bx [bi] < PLAYER_BULLET_SPEED ||
				bx [bi] > 255 - PLAYER_BULLET_SPEED ||
				by [bi] < PLAYER_BULLET_SPEED ||
				by [bi] > 191 - PLAYER_BULLET_SPEED ||
				(rdm & 8)
			) bullets_destroy (); 
			
			// Collide with enemies?			
			for (gpjt = 0; gpjt < 3; gpjt ++) {
				if (
				#ifdef FIRE_MIN_KILLABLE
					en_t [gpjt] >= FIRE_MIN_KILLABLE
				#else
					en_t [gpjt]
				#endif
				#ifndef PLAYER_TOP_DOWN
					&& en_t [gpjt] != 4
				#endif
				#ifdef ENABLE_SAW
					&& en_t [gpjt] != 8
				#endif
				#ifdef ENABLE_MONOCOCOS
					&& (en_t [gpit] != 11 || en_mx [gpit] == 2)
				#endif
				) {
					#ifdef ENABLE_PURSUERS
						if (en_t [gpjt] != 7 || en_alive [gpjt] == 2)
					#endif
					
					if (collide_in (bx [bi] + 3, by [bi] + 3, en_x [gpjt], en_y [gpjt])) {
						sfx_play (6, 2);
						bullets_destroy ();

						en_cttouched [gpjt] = 8;
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
			
		}
	}
}
