// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Simple multidirectional fixed point cocos

void cocos_init (void) {
	coco_it = COCOS_MAX; while (coco_it --) {
		coco_slots [coco_it] = coco_it;
		coco_on [coco_it] = 0;
	}
	coco_slots_i = COCOS_MAX;
}

#ifdef COCOS_ENABLE_AIMED
	// Create a coco @ (rdx, rdy), shoot towards player

	void cocos_shoot_aimed (void) {		
		rdct = distance ();

		if (rdct > COCO_FAIR_D && coco_slots_i) {
			coco_slots_i --; coco_it = coco_slots [coco_slots_i];

			coco_x [coco_it] = rdx << 6;
			coco_y [coco_it] = rdy << 6;

			// Apply formula. Looks awkward but it's optimized for space and shitty compiler
			rds16 = COCO_V * rda / rdct; coco_vx [coco_it] = ADD_SIGN2 (px, coco_x [coco_it], rds16);
			rds16 = COCO_V * rdb / rdct; coco_vy [coco_it] = ADD_SIGN2 (py, coco_y [coco_it], rds16);

			coco_on [coco_it] = 1;
		}	
	}
#endif

#ifdef COCOS_ENABLE_LINEAR
	// Create a coco @ (rdx, rdy), direction rda.

	void cocos_shoot_linear (void) {
		if (coco_slots_i == 0) return;

		coco_slots_i --; coco_it = coco_slots [coco_slots_i];

		coco_x [coco_it] = rdx << 6;
		coco_y [coco_it] = rdy << 6;

		coco_vx [coco_it] = coco_dx [rda];
		coco_vy [coco_it] = coco_dy [rda];

		coco_on [coco_it] = 1;
	}
#endif

void cocos_destroy (void) {
	coco_on [coco_it] = 0;
	coco_slots [coco_slots_i] = coco_it; coco_slots_i ++;
}

void cocos_do (void) {
	coco_it = COCOS_MAX; while (coco_it --) if (coco_on [coco_it]) {
		// Move
		coco_x [coco_it] += coco_vx [coco_it];
		coco_y [coco_it] += coco_vy [coco_it];

		// Out of bounds
		if (coco_x [coco_it] < 0 || coco_x [coco_it] > 248<<FIXBITS || coco_y [coco_it] < 16<<FIXBITS || coco_y [coco_it] > 200<<FIXBITS) {
			cocos_destroy ();
			continue;
		}

		rdx = coco_x [coco_it] >> 6;
		rdy = coco_y [coco_it] >> 6;

		// Render
		oam_index = oam_spr (rdx, rdy + SPRITE_ADJUST, COCO_PATTERN, COCO_PALETTE, oam_index);

		#ifdef COCO_COLLIDES
			rdm = map_attr [((rdx + 4) >> 4) | ((rdy + 4 - 16) & 0xf0)];
			if (rdm & 8) {
				cocos_destroy (); continue;
			}
		#endif

		// Collide w/player
		if (pstate == EST_NORMAL && 
			rdx + 7 >= prx && rdx <= prx + 7 && 
			#ifdef TALL_PLAYER
				rdy + 7 >= pry - 16 + PLAYER_COLLISION_TOP &&
			#else			
				rdy + 7 >= pry && 
			#endif
			rdy <= pry + 12
		) {
			pkill = 1;
			cocos_destroy ();
		}		
	}
}
