// NES MK1 v2.0
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
		// On screen?
		#ifdef DOUBLE_WIDTH
			if (COCO_RDX < scroll_x || COCO_RDX >= scroll_x + 248) return;
		#endif

		#ifdef COCOS_ROUGH_AIM
			if (coco_slots_i) {
				-- coco_slots_i; coco_it = coco_slots [coco_slots_i];

				coco_x [coco_it] = COCO_RDX << 6;
				coco_y [coco_it] = rdy << 6;

				// First prune: 8 directions.

				if (ROUGHLY_EQUAL (COCO_RDX, prx, 16)) {
					coco_vx [coco_it] = 0;
				} else if (COCO_RDX < prx) {
					coco_vx [coco_it] = COCO_V;
				} else {
					coco_vx [coco_it] = -COCO_V;
				}

				if (ROUGHLY_EQUAL (rdy, pry, 16)) {
					coco_vy [coco_it] = 0;
				} else if (rdy < pry) {
					coco_vy [coco_it] = COCO_V;
				} else {
					coco_vy [coco_it] = -COCO_V;
				}

				// Second prune: 16 directions

				// rda = dx; rdb = dy
				rda = DELTA (COCO_RDX, prx);
				rdb = DELTA (rdy, pry);
				if (ROUGHLY_EQUAL (rda, rdb, 8)) {

				} else if (rda > rdb) {
					coco_vy [coco_it] >>= 1;
				} else {
					coco_vx [coco_it] >>= 1;
				}

				coco_on [coco_it] = 1;
				sfx_play (SFX_COCO, 2);
			}


		#else
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
		#endif
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
	coco_slots [coco_slots_i] = coco_it; ++ coco_slots_i;
	sfx_play (SFX_DUMMY1, 2);
}

void cocos_do (void) {
	coco_it = COCOS_MAX; while (coco_it) {
		-- coco_it;
		if (coco_on [coco_it]) {

			// Move
			coco_x [coco_it] += coco_vx [coco_it];
			coco_y [coco_it] += coco_vy [coco_it];

			COCO_RDX = coco_x [coco_it] >> FIXBITS;
			rdy      = coco_y [coco_it] >> FIXBITS;
			
			// Out of bounds?
			if (
				#ifdef DOUBLE_WIDTH
					COCO_RDX < scroll_x ||
					COCO_RDX > scroll_x + 248 ||
				#else
					// Optimization: if rdx < 0, it will wrap around,
					/*COCO_RDX < 0 ||*/
					COCO_RDX > 248 ||
				#endif

				// Optimization: if rdy < 0, it will wrap around,
				// so this is more or less the same as doing
				// coco_y [coco_it] < 0 || coco_y [coco_it] > (200<<FIXBITS)
				rdy > 208
			) {
				cocos_destroy (); continue;
			}

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
				#ifdef DOUBLE_WIDTH
					gpint = COCO_RDX + 4;
					rdm = MAP_ATTR ((gpint & 0x100 ? 192 : 0) + ((gpint & 0xff) >> 4) | ((rdy + 4 - 16) & 0xf0));
				#else
					rdm = MAP_ATTR (((COCO_RDX + 4) >> 4) | ((rdy + 4 - 16) & 0xf0));
				#endif
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
				pkill = en_sg_2;
				cocos_destroy ();
			}
		}
	}
}
