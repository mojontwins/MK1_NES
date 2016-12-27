// NES MK1 v0.3
// Copyleft Mojon Twins 2013, 2015

// enengine.h
// Enemies Engine & stuff

#ifdef PERSISTENT_ENEMIES
// Copy to RAM addresses and initialize statuses (alive = 1)
void __fastcall__ persistent_enems_load (void) {
	gp_gen = (unsigned char *) (c_enems);
	for (ep_it = 0; ep_it < 3 * MAP_W * MAP_H; ep_it ++) {
		ep_x [ep_it] = *gp_gen ++;
		ep_y [ep_it] = *gp_gen;
		gp_gen += 3;
		ep_mx [ep_it] = (*gp_gen ++) - 8;
		ep_my [ep_it] = (*gp_gen ++) - 8;
		ep_t [ep_it] = (*gp_gen ++);
	}
}

void __fastcall__ persistent_update (void) {
	if (on_pant != 99) {
		ep_it = on_pant * 3;
		for (gpit = 0; gpit < 3; gpit ++) {
			ep_x [ep_it] = en_x [gpit];
			ep_y [ep_it] = en_y [gpit];
			ep_mx [ep_it] = en_mx [gpit] << (!en_status [gpit]);
			ep_my [ep_it] = en_my [gpit] << (!en_status [gpit]);			
			ep_t [ep_it] = en_t [gpit];
			ep_it ++;
		}	
	}
}
#endif

void __fastcall__ enems_load (void) {
	// Loads enems from n_pant
	
#ifdef PERSISTENT_ENEMIES
	ep_it = n_pant * 3;
#endif
	gp_gen = (unsigned char *) (c_enems) + n_pant * 21;
	for (gpit = 0; gpit < 3; gpit ++) {
		oam_meta_spr (
			0, 240, 
			(gpit << 4), 
			spr_empty
		);

		en_x1 [gpit] = *gp_gen ++;
		en_y1 [gpit] = *gp_gen ++;
		en_x2 [gpit] = *gp_gen ++;
		en_y2 [gpit] = *gp_gen ++;

		en_t [gpit] = ep_t [ep_it];
		
#ifdef ENABLE_SAW
		if (en_t [gpit] == 8) {

			en_mx [gpit] = (*gp_gen ++) - 8;
			en_my [gpit] = (*gp_gen ++) - 8;
			en_x [gpit] = en_x1 [gpit];
			en_y [gpit] = en_y1 [gpit];

			// emerging sense
			rda = abs (en_mx [gpit]); if (!rda) rda = abs (en_my [gpit]);
			rda --;
			//rda = add_sign (rda, SAW_V_EM);

			// Sense
			if (en_x1 [gpit] == en_x2 [gpit]) {
				rdb = sgnc (en_y2 [gpit], en_y1 [gpit], SAW_V_DISPL);
			} else {
				rdb = sgnc (en_x2 [gpit], en_x1 [gpit], SAW_V_DISPL);
			}

			// Store:
			en_my [gpit] = rda; // EMERGING SENSE
			en_mx [gpit] = rdb; // MOVING SENSE
			
			en_alive [gpit] = 1;
			en_ct [gpit] = SAW_EMERGING_STEPS;

			gp_gen ++;
		} else
#endif
		{
			en_mx [gpit] = ep_mx [ep_it];
			en_my [gpit] = ep_my [ep_it];

#ifdef ENABLE_FANTY
			if (en_t [gpit] == 6) {
				en_x [gpit] = en_x1 [gpit];
				en_y [gpit] = en_y1 [gpit];
			} else
#endif		
			{
				en_x [gpit] = ep_x [ep_it];
				en_y [gpit] = ep_y [ep_it];
			}

			gp_gen += 3;
		}
		
		ep_it ++;

#if defined(PLAYER_CAN_FIRE)
		en_life [gpit] = ENEMIES_LIFE_GAUGE;
#elif defined(PLAYER_KILLS_ENEMIES)
		en_life [gpit] = 1;
#endif		
		en_status [gpit] = 0;
		
#ifdef ENABLE_PURSUERS		
		if (en_t [gpit] == 7) {
			en_s [gpit] = TYPE_7_FIXED_SPRITE - 1;
			en_alive [gpit] = 0;
			en_ct [gpit] = DEATH_COUNT_EXPRESSION;
		} else 
#endif
#ifdef ENABLE_FANTY
		if (en_t [gpit] == 6) {
			enf_x [gpit] = en_x [gpit] << 6;
			enf_y [gpit] = en_y [gpit] << 6;
			enf_vx [gpit] = enf_vy [gpit] = 0;
		} else
#endif
		{
			en_s [gpit] = en_t [gpit] - 1;
			if (en_mx [gpit] == 1 || en_my [gpit] == 1) {
				en_status [gpit] = 1; 
			} else {
				en_status [gpit] = 0;
				en_mx [gpit] >>= 1;
				en_my [gpit] >>= 1;
			}
		}
		
		en_touched [gpit] = 0;		
	}
}

void __fastcall__ enems_move (void) {
#ifndef PLAYER_MOGGY_STYLE	
	pgotten = 0;
	pgtmx = 0;
	pgtmy = 0;
#endif
	
	// Updates sprites
	touched = 0;
	for (gpit = 0; gpit < 3; gpit ++) {
#if defined(PLAYER_CAN_FIRE) || defined(PLAYER_KILLS_ENEMIES)
		if (en_touched [gpit]) {
			en_cttouched [gpit] --;
			if (!en_cttouched [gpit]) {
				en_touched [gpit] = 0;
			} else {
				oam_meta_spr (en_x [gpit], en_y [gpit] + SPRITE_ADJUST, ENEMS_OAM_BASE + (gpit << 4), spr_en_11);
				continue;
			}
		}
#endif
		
		if (en_t [gpit]) {

#ifdef BOUNDING_BOX_8_BOTTOM
			// Gotten preliminary:
			gpjt = (prx + 11 >= en_x [gpit] && prx <= en_x [gpit] + 11);
#endif		

			if (en_mx [gpit] != 0) {
				en_fr = ((en_x [gpit]) >> 4) & 1;
			} else {
				en_fr = ((en_y [gpit]) >> 4) & 1;
			}

			switch (en_t [gpit]) {
				case 1:
				case 2:
				case 3:
				case 4:
					#include "engine/enemmods/enem_linear.h"
					break;
#ifdef ENABLE_FANTY					
				case 6:
					#include "engine/enemmods/enem_fanty.h"
					break;
#endif
#ifdef ENABLE_PURSUERS					
				case 7:					
					#include "engine/enemmods/enem_pursuers.h"
					break;
#endif
#ifdef ENABLE_SAW					
				case 8:
					#include "engine/enemmods/enem_saw.h"
					break;
#endif					
			}

#ifndef PLAYER_MOGGY_STYLE
			// My nemesis: movable platforms
			// This time coded in a SMARTER way...!
			if (en_t [gpit] == 4 && gpjt && !pgotten && !pj) {
				// Horizontal moving platforms
				if (en_mx [gpit]) {
					if (pry + 16 >= en_y [gpit] && pry + 12 <= en_y [gpit]) {
						pgotten = 1;
						pgtmx = en_mx [gpit] << (6 - en_status [gpit]);
						py = (en_y [gpit] - 16) << 6; pry = py >> 6;
					}
				}
				
				// Vertical moving platforms
				if (
					(en_my [gpit] < 0 && pry + 17 >= en_y [gpit] && pry + 12 <= en_y [gpit]) ||
					(en_my [gpit] > 0 && pry + 16 + en_my [gpit] >= en_y [gpit] && pry + 12 <= en_y [gpit])
				) {
					pgotten = 1;
					pgtmy = en_my [gpit] << (6 - en_status [gpit]);
					py = (en_y [gpit] - 16) << 6; pry = py >> 6;
					pvy = 0;
				}
			}
#endif

#ifdef PLAYER_KILLS_ENEMIES
#ifdef PLAYER_MIN_KILLABLE
			if (en_t [gpit] >= PLAYER_MIN_KILLABLE && en_t [gpit] != 8 && en_t [gpit] != 4) {
#else
			if (en_t [gpit] != 8 && en_t [gpit] != 4 && res_on) {
#endif			
				// Step over enemy
				if (collide (prx, pry, en_x [gpit], en_y [gpit] - 4) && pry + 2 < en_y [gpit] && pvy > 0 && !pgotten && !ppossee) {
					en_touched [gpit] = 1;
					en_cttouched [gpit] = 8;
					sfx_play (6, 2);
					en_life [gpit] --;
					if (!pad_poll (0) & PAD_B) {
						pvy = -PLAYER_VY_JUMP_INITIAL << 1;
					} else {
						pj = 1; 
						pctj = 0;
						sfx_play (7, 0);
						pvy = -PLAYER_VY_JUMP_INITIAL;
					}
					if (en_life [gpit] == 0) {
						en_t [gpit] = 0;
						pkilled ++;
					}
					touched = 1;
					continue;
				}
			}
#endif

			// Collide <-> player
#ifdef ENABLE_SAW
			if (!touched && pstate == EST_NORMAL && collide (prx, pry, en_x [gpit], en_y [gpit]) && (!res_on || en_t [gpit] == 8)) 
#else
			if (!touched && pstate == EST_NORMAL && collide (prx, pry, en_x [gpit], en_y [gpit]) && !res_on) 
#endif
			{

#ifdef ENABLE_PURSUERS	
				if (en_t [gpit] != 7 || en_alive [gpit] == 2) 
#endif
#ifndef PLAYER_MOGGY_STYE
				if (en_t [gpit] != 4)
#endif				
#ifdef ENABLE_SAW
				if (en_t [gpit] != 8 || en_alive [gpit])
#endif				
				{
					touched = 1;
					kill_player ();
				}
			}
		} else {
			oam_meta_spr (0, 240, ENEMS_OAM_BASE + (gpit << 4), spr_empty);
		}
	}	
}
