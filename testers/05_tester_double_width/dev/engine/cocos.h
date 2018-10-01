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
	// Create a coco @ (COCO_RDX, rdy), shoot towards player

	void cocos_shoot_aimed (void) {		
		rdct = distance ();

	#ifdef COCO_FAIR_D
		if (rdct > COCO_FAIR_D && coco_slots_i) 
	#else
		if (coco_slots_i)
	#endif
		{
			-- coco_slots_i; coco_it = coco_slots [coco_slots_i];

			coco_x [coco_it] = COCO_RDX << 6;
			coco_y [coco_it] = rdy << 6;

			// Apply formula. Looks awkward but it's optimized for space and shitty compiler
			rds16 = COCO_V * rda / rdct; coco_vx [coco_it] = ADD_SIGN2 (px, coco_x [coco_it], rds16);
			rds16 = COCO_V * rdb / rdct; coco_vy [coco_it] = ADD_SIGN2 (py, coco_y [coco_it], rds16);

			coco_on [coco_it] = 1;

			sfx_play (SFX_COCO, 2);
		}	
	}
#endif

#ifdef COCOS_ENABLE_LINEAR
	// Create a coco @ (COCO_RDX, rdy), direction rda. LEFT UP RIGHT DOWN

	void cocos_shoot_linear (void) {
		if (coco_slots_i == 0) return;

		-- coco_slots_i; coco_it = coco_slots [coco_slots_i];

		coco_x [coco_it] = COCO_RDX << 6;
		coco_y [coco_it] = rdy << 6;

		coco_vx [coco_it] = coco_dx [rda];
		coco_vy [coco_it] = coco_dy [rda];

		coco_on [coco_it] = 1;

		sfx_play (SFX_COCO, 2);
	}
#endif

void cocos_destroy (void) {
	coco_on [coco_it] = 0;
	coco_slots [coco_slots_i] = coco_it; coco_slots_i ++;
	sfx_play (SFX_DUMMY1, 2);
}

void cocos_do (void) {
	coco_it = COCOS_MAX; while (coco_it --) if (coco_on [coco_it]) {
		#ifdef DOUBLE_WIDTH
			COCO_RDX = coco_x [coco_it] >> FIXBITS;
		#endif
		if (
			#ifdef DOUBLE_WIDTH
				COCO_RDX < scroll_x + 8 ||
				COCO_RDX > scroll_x + 248 ||
			#else
				coco_x [coco_it] < coco_vx [coco_it] ||
				coco_x [coco_it] > (248<<FIXBITS) - coco_vx [coco_it] ||
			#endif

			coco_y [coco_it] < coco_vy [coco_it] ||
			coco_y [coco_it] > (200<<FIXBITS) - coco_vy [coco_it]
		) {
			cocos_destroy ();
			continue;
		}

		// Move
		coco_x [coco_it] += coco_vx [coco_it];
		coco_y [coco_it] += coco_vy [coco_it];

		COCO_RDX = coco_x [coco_it] >> FIXBITS;
		rdy      = coco_y [coco_it] >> FIXBITS;

		// Render
		oam_index = oam_spr (
			#ifdef DOUBLE_WIDTH
				COCO_RDX - scroll_x,
			#else
				COCO_RDX, 
			#endif
			rdy + SPRITE_ADJUST, 
			COCO_PATTERN, 
			COCO_PALETTE, 
			oam_index
		);

		#ifdef COCO_COLLIDES
			rdm = map_attr [((COCO_RDX + 4) >> 4) | ((rdy + 4 - 16) & 0xf0)];
			if (rdm & 8) {
				cocos_destroy (); continue;
			}
		#endif

		// Collide w/player
		if (pflickering == 0 && 
			COCO_RDX + 7 >= prx && 
			COCO_RDX <= prx + 7 && 
			rdy + 7 + PLAYER_COLLISION_VSTRETCH_FG >= pry && 
			rdy <= pry + 12
		) {
			en_sg_2 = 1;
			#include "my/on_player_coco.h"
			pkill = !!en_sg_2;
			cocos_destroy ();
		}		
	}
}
