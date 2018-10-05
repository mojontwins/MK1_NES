// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

void fire_bullet (void) {
	#ifdef MAX_AMMO
		if (!pammo) return;
		-- pammo;
	#endif

	// Creates a new bullet (if possible):

	if (b_slots_i == 0) return;
	-- b_slots_i; bi = b_slots [b_slots_i];

	#ifdef PLAYER_BULLET_LIFE
		bst [bi] = PLAYER_BULLET_LIFE;
	#else
		bst [bi] = 1;
	#endif

	#ifdef PLAYER_FIRE_RELOAD
		pfirereload = PLAYER_FIRE_RELOAD;
	#endif
	
	switch (pfacing) {
		case CELL_FACING_LEFT:
			bx [bi] = prx - 8;
			bmx [bi] = -PLAYER_BULLET_SPEED;
			by [bi] = pry + PLAYER_BULLET_Y_OFFSET;
			bmy [bi] = 0;
			break;	
		case CELL_FACING_RIGHT:
			bx [bi] = prx + 8;
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
				bx [bi] = prx - PLAYER_BULLET_X_OFFSET;
				by [bi] = pry - 4;
				bmy [bi] = -PLAYER_BULLET_SPEED;
				bmx [bi] = 0;
				break;	
		#endif
	}	
	
	// Diagonals
	#ifdef PLAYER_CAN_FIRE_8_WAY
		#ifdef PLAYER_TOP_DOWN
		if (pfacing == CELL_FACING_LEFT || pfacing == CELL_FACING_RIGHT)
		#endif
		{
			if (pad0 & PAD_UP) {
				#ifndef PLAYER_TOP_DOWN
					if (0 == (pad0 & (PAD_LEFT|PAD_RIGHT))) bmx [bi] = 0;
				#endif
				bmy [bi] = -PLAYER_BULLET_SPEED;
				bx [bi] = prx;
			} else if (pad0 & PAD_DOWN) {
				#ifndef PLAYER_TOP_DOWN
					if (0 == (pad0 & (PAD_LEFT|PAD_RIGHT))) bmx [bi] = 0;
				#endif
				bmy [bi] = PLAYER_BULLET_SPEED;
				bx [bi] = prx;
			}
		}
		#ifdef PLAYER_TOP_DOWN
		else {
			if (pad0 & PAD_LEFT) {
				bmx [bi] = -PLAYER_BULLET_SPEED;
			} else if (pad0 & PAD_RIGHT) {
				bmx [bi] = PLAYER_BULLET_SPEED;
			}
		}
		#endif
	#endif

	sfx_play (SFX_BULLET, 2);
}

void bullets_destroy (void) {
	bst [bi] = 0;
	b_slots [b_slots_i] = bi; ++ b_slots_i;
	sfx_play (SFX_DUMMY1, 2);
}

void bullets_move (void) {
	for (bi = 0; bi < MAX_BULLETS; bi ++) {
		if (bst [bi]) {
			bx [bi] += bmx [bi];
			by [bi] += bmy [bi];

			#ifdef PLAYER_BULLET_FLICKERS
			if (bst [bi] > PLAYER_BULLET_FLICKERS || half_life)
			#endif
			oam_index = oam_spr (
				bx [bi], SPRITE_ADJUST + by [bi], 
				BULLET_PATTERN, BULLET_PALETTE,
				oam_index
			);

			cx1 = ((bx [bi] + 4) >> 4);
			cy1 = ((by [bi] + 4 - 16) >> 4);
			rdm = map_attr [COORDS (cx1, cy1)];

			#ifdef PLAYER_BULLET_LIFE
				-- bst [bi]; 
				if (bst [bi] == 0) bullets_destroy (); 
				else
			#endif
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
				by [bi] > 207 - PLAYER_BULLET_SPEED ||
				(rdm & 8)
			) bullets_destroy (); 
		}
	}
}
