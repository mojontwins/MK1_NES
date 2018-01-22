// NES MK1 v0.7
// Copyleft Mojon Twins 2013, 2015, 2016

// enengine.h
// Enemies Engine & stuff

#if defined (PLAYER_KILLS_ENEMIES) || defined (PLAYER_CAN_FIRE) || defined (FANTY_KILLED_BY_TILE)
void enems_kill (unsigned char gpit) {
	en_t [gpit] = 0;

	#ifdef PERSISTENT_DEATHS
		ep_flags [en_offs + gpit] &= 0xFE;
	#endif

	#ifdef ACTIVATE_SCRIPTING
		run_script (2 * MAP_SIZE + 5);
	#endif

		pkilled ++;

	#ifdef COUNT_KILLED_IN_FLAG
		flags [COUNT_KILLED_IN_FLAG] ++;
	#endif
}
#endif

#ifdef ENABLE_HOMING_FANTY
	// Lame but fast and tiny
	// Before the call: copy fanty's coordinates into rdx, rdy
	unsigned char enems_distance (void) {
		rda = delta (prx, rdx); // dx
		rdb = delta (pry, rdy); // dy
		rdc = min (rda, rdb);
		return (rda + rdb - (rdc >> 1) - (rdc >> 2) + (rdc >> 4));
	}
#endif

#ifdef ENABLE_CHAC_CHAC
	void enems_draw_chac_chac (unsigned char a1, unsigned char a2, unsigned char a3) {
		map_set (en_x [gpit], en_y [gpit], a1);
		map_set (en_x [gpit], en_y [gpit] + 1, a2);
		map_set (en_x [gpit], en_y [gpit] + 2, a3);
	}
#endif

#ifdef PERSISTENT_ENEMIES
	void enems_persistent_load (void) {
		gp_gen = (unsigned char *) (c_enems);
		for (ep_it = 0; ep_it < 3 * MAP_SIZE; ep_it ++) {
			// Skip t
			rdt = *gp_gen ++; if (rdt && rdt != 4) BADDIES_COUNT ++;

			// XY1
			rda = *gp_gen ++;
			ep_x [ep_it] = rda & 0xf0;
			ep_y [ep_it] = rda << 4;

			// XY2
			rda = *gp_gen ++;
			rdb = rda & 0xf0;
			rdc = rda << 4;

			// P, here used for speed
			rda = *gp_gen ++;
			ep_mx [ep_it] = add_sign (rdb - ep_x [ep_it], rda);
			ep_my [ep_it] = add_sign (rdc - ep_y [ep_it], rda);		
		}
	}

	void enems_persistent_update (void) {
		if (on_pant != 99) {
			ep_it = on_pant + on_pant + on_pant;
			gpit = 3; while (gpit --) {
				ep_x [ep_it] = en_x [gpit];
				ep_y [ep_it] = en_y [gpit];
				ep_mx [ep_it] = en_mx [gpit] << (1 - en_status [gpit]);
				ep_my [ep_it] = en_my [gpit] << (1 - en_status [gpit]);	
				ep_it ++;		
			}	
		}
	}
#endif

#ifdef PERSISTENT_DEATHS
	void enems_persistent_deaths_load (void) {
		gpit = MAP_SIZE * 3; while (gpit --) {
			ep_flags [gpit] |= 0x01;
		}
	}
#endif

void enems_load (void) {
	// Loads enems from n_pant

	// Read 3 enemies from enems ROM pool and populate my arrays properly.
	// If persistent enemies on: x, y, mx, my read from RAM pool.
	// If persistent deaths on: read ep_flags and modify en_t accordingly.

	// Each screen holds 3 * 4 bytes of enemies, that's 12 bytes per screen.
	// 12 = 4 + 8 so you know the drill...
	
	gp_gen = (unsigned char *) (c_enems + (n_pant << 2) + (n_pant << 3));

	// Notice that enemies are writen backwards. Take in account in the future

#if defined (PERSISTENT_DEATHS) || defined (PERSISTENT_ENEMIES)
	en_offs = rdc = n_pant + n_pant + n_pant;// + 3;
#endif	

	gpit = 3; while (gpit --) {
		
/*
#if defined (PERSISTENT_DEATHS) || defined (PERSISTENT_ENEMIES)
		-- rdc;
#endif
*/
#ifdef PERSISTENT_DEATHS	
		// Fast hack. If enemy is dead, change for type 0 and skip data.
		if (!ep_flags [rdc] & 1) {
			en_t [gpit] = 0;
			gp_gen += 4;
		} else 
#endif
		{
			// First get T, then do whatever I need
			en_t [gpit] = *gp_gen ++;

			// General...

			// YX1
			rda = *gp_gen ++;
			en_y1 [gpit] = rda & 0xf0;
			en_x1 [gpit] = rda << 4;

			// YX2
			rda = *gp_gen ++;
			en_y2 [gpit] = rda & 0xf0;
			en_x2 [gpit] = rda << 4;
		
			// P, here used for speed
			rda = *gp_gen ++;

#ifdef PERSISTENT_ENEMIES
			// Copy position & direction from ep_*
			en_x [gpit] = ep_x [rdc];
			en_y [gpit] = ep_y [rdc];
			en_mx [gpit] = ep_mx [rdc];
			en_my [gpit] = ep_my [rdc];
#else
			// Initialize position & direction from ROM
			en_x [gpit] = en_x1 [gpit];
			en_y [gpit] = en_y1 [gpit];
			en_mx [gpit] = add_sign (en_x2 [gpit] - en_x1 [gpit], rda);
			en_my [gpit] = add_sign (en_y2 [gpit] - en_y1 [gpit], rda);
#endif

			switch (en_t [gpit]) {
				case 1:
				case 2:
				case 3:
				case 4:
					// Linear enems.			

					// HL conversion
					en_s [gpit] = (en_t [gpit] - 1) << 2;
					if (rda == 1) {
						en_status [gpit] = 1; 
					} else {
						en_status [gpit] = 0;
						en_mx [gpit] >>= 1;
						en_my [gpit] >>= 1;
					}

					// Fix limits so 1 < 2 always.
					if (en_x1 [gpit] > en_x2 [gpit]) { rda = en_x1 [gpit]; en_x1 [gpit] = en_x2 [gpit]; en_x2 [gpit] = rda; }
					if (en_y1 [gpit] > en_y2 [gpit]) { rda = en_y1 [gpit]; en_y1 [gpit] = en_y2 [gpit]; en_y2 [gpit] = rda; }
					
					break;
	#ifdef ENABLE_FANTY				
				case 6:
					// Fantys
					enf_x [gpit] = en_x [gpit] << 6;
					enf_y [gpit] = en_y [gpit] << 6;
					enf_vx [gpit] = enf_vy [gpit] = 0;
					break;
	#endif

	#ifdef ENABLE_HOMING_FANTY				
				case 6:
					// Fantys
					enf_x [gpit] = en_x [gpit] << 6;
					enf_y [gpit] = en_y [gpit] << 6;
					enf_vx [gpit] = enf_vy [gpit] = 0;
					// State idle
					en_alive [gpit] = 0; 
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

	#ifdef ENABLE_SAW
				case 8:
					// Saws

					// emerging sense
					rda = abs (en_mx [gpit]); if (!rda) rda = abs (en_my [gpit]);
					rda --;

					// Sense
					rdb = (en_x1 [gpit] != en_x2 [gpit]) ? 
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

					// Initialize
					en_my [gpit] = PEZON_WAIT + (rda << 3);	// Speed in colocador defines idle time! (x8)
					en_alive [gpit] = 0;
					en_mx [gpit] = en_my [gpit];
					break;
	#endif

	#ifdef ENABLE_CHAC_CHAC
				case 10:
					// Cuchillas Chac Chac

					en_my [gpit] = (rda << 4);	// IDLE_1
					en_x [gpit] = en_x1 [gpit] >> 4;
					en_y [gpit] = en_y1 [gpit] >> 4;
					en_alive [gpit] = 0;
					en_mx [gpit] = en_my [gpit];

					break;
	#endif

	#ifdef ENABLE_MONOCOCO
				case 11:
					// Monococos
					en_mx [gpit] = 0; en_my [gpit] = MONOCOCO_BASE_TIME_HIDDEN - (rand8 () & 0x15);
					break;
	#endif					
			}

	#if defined(PLAYER_CAN_FIRE)
		#if defined (ENABLE_FANTY) || defined (ENABLE_HOMING_FANTY)
				en_life [gpit] = en_t [gpit] == 6 ? FANTY_LIFE_GAUGE : ENEMIES_LIFE_GAUGE;
		#else
				en_life [gpit] = ENEMIES_LIFE_GAUGE;
		#endif
	#elif defined(PLAYER_KILLS_ENEMIES)
			en_life [gpit] = 1;
	#endif		
			en_cttouched [gpit] = 0;
		}
#if defined (PERSISTENT_DEATHS) || defined (PERSISTENT_ENEMIES)
		rdc ++;
#endif
	}
}

void enems_move (void) {
#ifndef PLAYER_MOGGY_STYLE	
	pgotten = pgtmx = pgtmy = 0;
#endif
	
	// Updates sprites
	touched = 0;
	en_initial ++; if (en_initial >= 3) en_initial = 0;
	gpit = en_initial;
	gpjt = 3; while (gpjt --) {
		gpit += 2; if (gpit > 2) gpit -=3;
#if defined(PLAYER_CAN_FIRE) || defined(PLAYER_KILLS_ENEMIES) || defined (FANTY_KILLED_BY_TILE)
		if (en_cttouched [gpit]) {
			en_cttouched [gpit] --;
			oam_index = oam_meta_spr (
				en_x [gpit], en_y [gpit] + SPRITE_ADJUST, 
				oam_index, 
				spr_enems [SPRITE_BADDIE_DYING]
			);
			continue;
		} 
#endif
		
		if (en_t [gpit]) {

			// Gotten preliminary:
			pregotten = (prx + 7 >= en_x [gpit] && prx <= en_x [gpit] + 15);

			// Select frame upon screen position:
			en_fr = ((((en_mx [gpit]) ? en_x [gpit] : en_y [gpit]) + 8) >> 4) & 1;

			// Means don't render (can/will be overwritten):
			en_spr = 0xff;	

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

#ifdef ENABLE_HOMING_FANTY
				case 6:
					#include "engine/enemmods/enem_homing_fanty.h"
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

#ifdef ENABLE_CHAC_CHAC
				case 10:
					#include "engine/enemmods/enem_chac_chac.h"
					break;
#endif

#ifdef ENABLE_MONOCOCOS
				case 11:
					#include "engine/enemmods/enem_monococo.h"
					break;
#endif					
			}

			// Render enemy metasprite en_spr
			if (en_spr != 0xff) {
				oam_index = oam_meta_spr (
					en_x [gpit], en_y [gpit] + SPRITE_ADJUST, 
					oam_index, 
					spr_enems [en_spr]
				);
			}

#ifndef PLAYER_MOGGY_STYLE
			// Movable platforms

			if (en_t [gpit] == 4 && pregotten && !pgotten && !pj) {
				
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

			// Step over enemy

#if defined (PLAYER_HAS_JUMP) && (defined (PLAYER_KILLS_ENEMIES) || defined (PLAYER_SAFE_LANDING))
			if (
	#ifdef PLAYER_MIN_KILLABLE
				(en_t [gpit] >= PLAYER_MIN_KILLABLE) &&
	#endif
	#ifdef ENABLE_SAW
				en_t [gpit] != 8 &&
	#endif 
				en_t [gpit] != 4
			) {
				if (
					pregotten &&
					pry + 15 >= en_y [gpit] && 
					pry + 8 < en_y [gpit] && 
					pvy > 0 && 
					!pgotten && 
					!ppossee
				) {
#ifdef PLAYER_SAFE_LANDING
					if (en_my [gpit] < 0) en_my [gpit] = -en_my [gpit];					
#else
					en_cttouched [gpit] = 8;
					sfx_play (6, 2);
					en_life [gpit] --;
#endif					

#ifndef PLAYER_BOOTEE
					if (!(pad_poll (0) & PAD_B)) {
						pvy = -PLAYER_VY_JUMP_INITIAL << 1;
					} else 
#endif
					{
						pj = 1; pctj = 0; pvy = -PLAYER_VY_JUMP_INITIAL;
						sfx_play (7, 0);
					}


#ifndef PLAYER_SAFE_LANDING					
					if (en_life [gpit] == 0) enems_kill (gpit);
#endif					
					touched = 1;
					continue;
				}
			}
#endif

			// Collide <-> player

			if (
#if defined (ENABLE_RESONATORS) && defined (ENABLE_SAW)
				(!res_on || en_t [gpit] == 8) &&
#endif				
				!touched &&
				pstate == EST_NORMAL && 
				collide (prx, pry, en_x [gpit], en_y [gpit])
			) {

#ifdef ENABLE_PURSUERS	
				if (en_t [gpit] != 7 || en_alive [gpit] == 2) 
#endif
#ifndef PLAYER_MOGGY_STYE
				if (en_t [gpit] != 4)
#endif				
#ifdef ENABLE_SAW
				if (en_t [gpit] != 8 || en_alive [gpit])
#endif
#ifdef ENABLE_CHAC_CHAC
				if (en_t [gpit] != 10)
#endif
#ifdef ENABLE_MONOCOCOS
				if (en_t [gpit] != 11 || en_mx [gpit] == 2)
#endif
				{
					touched = 1;

#ifdef PLAYER_BOUNCES
					pvx = add_sign (en_mx [gpit], PLAYER_V_REBOUND); en_mx [gpit] = add_sign (en_x [gpit] - prx, abs (en_mx [gpit]));
					pvy = add_sign (en_my [gpit], PLAYER_V_REBOUND); if (!en_mx [gpit]) en_my [gpit] = add_sign (en_y [gpit] - pry, abs (en_my [gpit]));
#endif
					
#ifdef PLAYER_CAN_FIRE
					en_life [gpit] --; if (en_life [gpit] == 0)
					{
	#ifdef ENABLE_PURSUERS
						if (en_t [gpit] == 7) {
							en_alive [gpit] = 0;
							en_ct [gpit] = DEATH_COUNT_EXPRESSION;
							en_life [gpit] = ENEMIES_LIFE_GAUGE;
						} else 
	#endif
						{
							enems_kill (gpit);
						}
					}					
#endif				
					kill_player ();
				}
			}
		} 
	}	
}
