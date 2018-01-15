// NES MK1 v0.4
// Copyleft Mojon Twins 2013, 2015

// enengine.h
// Enemies Engine & stuff

#ifdef PERSISTENT_ENEMIES
// Copy to RAM addresses and initialize statuses (alive = 1)
void __fastcall__ persistent_enems_load (void) {
	/*
	gp_gen = (unsigned char *) (c_enems);
	for (ep_it = 0; ep_it < 3 * MAP_W * MAP_H; ep_it ++) {
		ep_x [ep_it] = *gp_gen ++;
		ep_y [ep_it] = *gp_gen;
		gp_gen += 3;
		ep_mx [ep_it] = (*gp_gen ++) - 8;
		ep_my [ep_it] = (*gp_gen ++) - 8;
		ep_t [ep_it] = (*gp_gen ++);
	}
	*/
	// TODO! Rewrite when needed again.
}

void __fastcall__ persistent_update (void) {
	/*
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
	*/
	// TODO! Rewrite when needed again.
}
#endif

#ifdef PERSISTENT_DEATHS
void persistent_deaths_load (void) {
	gpit = MAP_W * MAP_H * 3; while (gpit --) {
		ep_flags [gpit] |= 0x01;
	}
}
#endif

void __fastcall__ enems_load (void) {
	// Loads enems from n_pant
	// TODO: Rewrite for PERSISTENT_ENEMIES when needed.

	// Read 3 enemies from enems ROM pool and populate my arrays properly.

	// Each screen holds 3 * 4 bytes of enemies, that's 12 bytes per screen.
	// 12 = 4 + 8 so you know the drill...
	gp_gen = (unsigned char *) (c_enems + (n_pant << 2) + (n_pant << 3));

	// Notice that enemies are write backwards. Take in account in the future
#ifdef PERSISTENT_DEATHS	
	rdc = n_pant + n_pant + n_pant;
#endif	
	gpit = 3; while (gpit --) {
#ifdef PERSISTENT_DEATHS	
		// Fast hack. If enemy is dead, change for type 0 and skip data.
		if (!ep_flags [rdc + gpit] & 1) {
			en_t [gpit] = 0;
			gp_gen += 4;
		} else 
#endif
		{
			// First get T, then do whatever I need
			en_t [gpit] = *gp_gen ++;

			// General...

			// XY1
			rda = *gp_gen ++;
			en_x [gpit] = en_x1 [gpit] = rda & 0xf0;
			en_y [gpit] = en_y1 [gpit] = rda << 4;

			// XY2
			rda = *gp_gen ++;
			en_x2 [gpit] = rda & 0xf0;
			en_y2 [gpit] = rda << 4;

			// P, here used for speed
			rda = *gp_gen ++;
			en_mx [gpit] = add_sign (en_x2 [gpit] - en_x1 [gpit], rda);
			en_my [gpit] = add_sign (en_y2 [gpit] - en_y1 [gpit], rda);


			switch (en_t [gpit]) {
				case 1:
				case 2:
				case 3:
				case 4:
					// Linear enems.			

					// HL conversion
					en_s [gpit] = en_t [gpit] - 1;
					if (rda) {
						en_status [gpit] = 1; 
					} else {
						en_status [gpit] = 0;
						en_mx [gpit] >>= 1;
						en_my [gpit] >>= 1;
					}

					break;
	#ifdef ENABLE_HOMING_FANTY				
				case 5:
					// Homing fantys
					break;
	#endif
	#ifdef ENABLE_FANTY				
				case 6:
					// Fantys
					enf_x [gpit] = en_x [gpit] << 6;
					enf_y [gpit] = en_y [gpit] << 6;
					enf_vx [gpit] = enf_vy [gpit] = 0;
					break;
	#endif
	#ifdef ENABLE_PURSUERS		
				case 7:
					// Pursuers

					en_s [gpit] = TYPE_7_FIXED_SPRITE - 1;
					en_alive [gpit] = 0;
					en_ct [gpit] = DEATH_COUNT_EXPRESSION;	
	#ifdef ENABLE_GENERATORS
					en_generator_life [gpit] = GENERATOR_LIFE_GAUGE;
					gen_was_hit [gpit] = 0;
	#endif	
					break;
	#endif	
	#ifdef ENABLE_SAWS		
				case 8:
					// Saws

					// emerging sense
					rda = abs (en_mx [gpit]); if (!rda) rda = abs (en_my [gpit]);
					rda --;

					// Sense
					rdb = en_mx [gpit] ? 
						sgnc (en_x2 [gpit], en_x1 [gpit], SAW_V_DISPL) :
						sgnc (en_y2 [gpit], en_y1 [gpit], SAW_V_DISPL);

					// Store:
					en_my [gpit] = rda; // EMERGING SENSE
					en_mx [gpit] = rdb; // MOVING SENSE

					en_alive [gpit] = 1;
					en_ct [gpit] = SAW_EMERGING_STEPS;
					break;
	#endif				
	#ifdef ENABLE_PEZONS
				case 9:
					// Pezones
					en_my [gpit] = PEZON_WAIT + ((rand8 () & 15) << 1);
					en_alive [gpit] = 0;
					en_mx [gpit] = en_my [gpit];
					break;
	#endif
			}

	#if defined(PLAYER_CAN_FIRE)
	#if defined (ENABLE_FANTY)
			en_life [gpit] = en_t [gpit] == 6 ? FANTY_LIFE_GAUGE : ENEMIES_LIFE_GAUGE;
	#else
			en_life [gpit] = ENEMIES_LIFE_GAUGE;
	#endif
	#elif defined(PLAYER_KILLS_ENEMIES)
			en_life [gpit] = 1;
	#endif		
			en_status [gpit] = 0;
			en_cttouched [gpit] = 0;			
		}
	}
}

void enems_move (void) {
#ifndef PLAYER_MOGGY_STYLE	
	pgotten = 0;
	pgtmx = 0;
	pgtmy = 0;
#endif
	
	// Updates sprites
	touched = 0;
	for (gpit = 0; gpit < 3; gpit ++) {
#if defined(PLAYER_CAN_FIRE) || defined(PLAYER_KILLS_ENEMIES)
		if (en_cttouched [gpit]) {
			en_cttouched [gpit] --;
			oam_meta_spr (en_x [gpit], en_y [gpit] + SPRITE_ADJUST, ENEMS_OAM_BASE + (gpit << 4), spr_en_0A);
			continue;
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
#ifdef ENABLE_PEZONS
				case 9:
					#include "engine/enemmods/enem_pezon.h"
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
			if (en_t [gpit] != 8 && en_t [gpit] != 4) {
#endif			
				// Step over enemy
				if (collide (prx, pry, en_x [gpit], en_y [gpit] - 4) && pry + 2 < en_y [gpit] && pvy > 0 && !pgotten && !ppossee) {
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
						kill_enemy (gpit);
						pkilled ++;
					}
					touched = 1;
					continue;
				}
			}
#endif

			// Collide <-> player
#ifdef ENABLE_SAW
			if (!touched && pstate == EST_NORMAL && collide (prx, pry, en_x [gpit], en_y [gpit]) && en_t [gpit] == 8)
#else
			if (!touched && pstate == EST_NORMAL && collide (prx, pry, en_x [gpit], en_y [gpit])) 
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

#ifdef PLAYER_BOUNCES
					// Bounce! In each axis, apply enemy's speed as is speed 1 = 128 and so forth
					pvx = add_sign (en_mx [gpit], PLAYER_V_REBOUND); en_mx [gpit] = -en_mx [gpit];
					pvy = add_sign (en_my [gpit], PLAYER_V_REBOUND); if (!en_mx [gpit]) en_my [gpit] = -en_my [gpit];

#endif

#ifdef PLAYER_CAN_FIRE
					en_life [gpjt] --;	
					if (en_life [gpjt] == 0) {	
#ifdef ENABLE_PURSUERS
						if (en_t [gpit] == 7) {
							en_alive [gpjt] = 0;
							en_ct [gpjt] = DEATH_COUNT_EXPRESSION;
							en_life [gpjt] = ENEMIES_LIFE_GAUGE;
						} else 
#endif
						{
							kill_enemy (gpjt);
							pkilled ++;		
						}
					}
					
#endif
					if (!pstate) kill_player ();
				}
			}
		} else {
			oam_meta_spr (0, 240, ENEMS_OAM_BASE + (gpit << 4), spr_empty);
#if defined (ENABLE_SAW) || defined (ENABLE_PEZONS)	|| defined (ENABLE_GENERATORS)
			oam_meta_spr (0, 240, (gpit << 4), spr_empty);
#endif			
		}
	}	
}
