// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// Simple multidirectional fixed point cocos

void cocos_init (void) {
	coco_it = COCOS_MAX; while (coco_it --) {
		coco_slots [coco_it] = coco_it;
		coco_on [coco_it] = 0;
	}
	coco_slots_i = COCOS_MAX;
}

void cocos_shoot (void) {
	// Create a coco @ (rdx, rdy)
	
	// Shoot towards player
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
		if (coco_x [coco_it] < 0 || coco_x [coco_it] > 256<<6 || coco_y [coco_it] < 0 || coco_y [coco_it] > 192<<6) {
			cocos_destroy ();
			continue;
		}

		rdx = coco_x [coco_it] >> 6;
		rdy = coco_y [coco_it] >> 6;

		// Render
		oam_index = oam_spr (rdx, rdy + SPRITE_ADJUST, COCO_PATTERN, COCO_PALETTE, oam_index);

		// Collide w/player
		if (pstate == EST_NORMAL && rdx >= prx + 1 && rdx <= prx + 7 && rdy + 7 >= pry && rdy <= pry + 12) {
			pkill = 1;
			cocos_destroy ();
		}		
	}
}
