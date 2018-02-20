// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// enengine.h
// Enemies Engine & stuff

#ifdef ENABLE_CHAC_CHAC
	void enems_draw_chac_chac (unsigned char a1, unsigned char a2, unsigned char a3) {
		_x = en_x [gpit]; _y = en_y [gpit]    ; _t = a1; map_set ();
		_x = en_x [gpit]; _y = en_y [gpit] + 1; _t = a2; map_set ();
		_x = en_x [gpit]; _y = en_y [gpit] + 2; _t = a3; map_set ();
	}
#endif

#ifdef PERSISTENT_ENEMIES
	void enems_persistent_load (void) {
		gp_gen = (unsigned char *) (c_enems);
		for (gpjt = 0; gpjt < 3 * MAP_SIZE; gpjt ++) {
			// Skip t
			rdt = *gp_gen ++; 

			// YX1
			rda = *gp_gen ++;
			ep_y [gpjt] = rda & 0xf0;
			ep_x [gpjt] = rda << 4;

			// YX2
			rda = *gp_gen ++;
			rdc = rda & 0xf0;
			rdb = rda << 4;

			// P, here used for speed
			rda = *gp_gen ++;
			if (rda > 1) rda >>= 1;	// Store converted!
			ep_mx [gpjt] = ADD_SIGN2 (rdb, ep_x [gpjt], rda);
			ep_my [gpjt] = ADD_SIGN2 (rdc, ep_y [gpjt], rda);		
		}
	}

	void enems_persistent_update (void) {
		if (on_pant != 99) {
			gpjt = on_pant + on_pant + on_pant;
			for (gpit = 0; gpit < 3; gpit ++) {
				__asm__ ("ldx %v", gpit);
				__asm__ ("ldy %v", gpjt);

				__asm__ ("lda %v,x", en_x);
				__asm__ ("sta %v,y", ep_x);

				__asm__ ("lda %v,x", en_y);
				__asm__ ("sta %v,y", ep_y);

				__asm__ ("lda %v,x", en_mx);
				__asm__ ("sta %v,y", ep_mx);

				__asm__ ("lda %v,x", en_my);
				__asm__ ("sta %v,y", ep_my);

				gpjt ++;		
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

	#if defined (PERSISTENT_DEATHS) || defined (PERSISTENT_ENEMIES)
		en_offs = rdc = n_pant + n_pant + n_pant;// + 3;
	#endif

	//gpit = 3; while (gpit --) {
	for (gpit = 0; gpit < 3; gpit ++) {
		
		#ifdef PERSISTENT_DEATHS	
			// Fast hack. If enemy is dead, change for type 0 and skip data.
			if (!(ep_flags [rdc] & 1)) {
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
			#else
				// Initialize position & direction from ROM
				en_x [gpit] = en_x1 [gpit];
				en_y [gpit] = en_y1 [gpit];
			#endif

			switch (en_t [gpit]) {
				case 1:
				case 2:
				case 3:
				case 4:
				#ifdef ENABLE_SHOOTIES
					case 12:
					case 13:
					case 14:
					case 15:
				#endif
				#ifdef ENABLE_PUNCHIES
					case 16:
					case 17:
					case 18:
					case 19:
				#endif
					en_ct [gpit] = 0;
	
					// Linear enems.
					#ifdef ENABLE_PUNCHIES
						if (en_t [gpit] >= 16) {
							en_rawv [gpit] = 2;
							en_s [gpit] = PUNCHIES_BASE_SPRID + ((en_t [gpit] - 16) << 3);
						} else
					#endif					
					#ifdef ENABLE_SHOOTIES
						if (en_t [gpit] >= 12) {
							en_rawv [gpit] = 1;
							en_s [gpit] = SHOOTIES_BASE_SPRID + ((en_t [gpit] - 12) << 3);
						} else
					#endif
					{
						en_rawv [gpit] = 0;
						en_s [gpit] = (en_t [gpit] - 1) << 3;
					}

					#ifdef PERSISTENT_ENEMIES
						en_mx [gpit] = ep_mx [rdc];
						en_my [gpit] = ep_my [rdc];
					#else
						en_mx [gpit] = ADD_SIGN2 (en_x2 [gpit], en_x1 [gpit], rda);
						en_my [gpit] = ADD_SIGN2 (en_y2 [gpit], en_y1 [gpit], rda);
					#endif

					// HL conversion		
					if (rda == 1) {
						en_status [gpit] = 1; 
					} else {
						en_status [gpit] = 0;
						#ifndef PERSISTENT_ENEMIES
							en_mx [gpit] >>= 1;
							en_my [gpit] >>= 1;
						#endif
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
						en_s [gpit] = FANTY_BASE_SPRID;
						break;
				#endif

				#ifdef ENABLE_HOMING_FANTY				
					case 6:
						// Fantys
						enf_x [gpit] = en_x [gpit] << 6;
						enf_y [gpit] = en_y [gpit] << 6;
						enf_vx [gpit] = enf_vy [gpit] = 0;
						en_s [gpit] = FANTY_BASE_SPRID;
						// State idle
						en_alive [gpit] = 0; 
						break;
				#endif	

				#ifdef ENABLE_PURSUERS		
					case 7:
						// Pursuers
						en_alive [gpit] = 0;
						en_ct [gpit] = DEATH_COUNT_EXPRESSION;	
						#ifdef ENABLE_GENERATORS
							en_generator_life [gpit] = GENERATOR_LIFE_GAUGE;
							gen_was_hit [gpit] = 0;
						#endif	
						en_s [gpit] = ((TYPE_7_FIXED_SPRITE - 1) << 3);
						break;
				#endif	

				#ifdef ENABLE_SAW
					case 8:
						// Saws
						#ifdef PERSISTENT_ENEMIES
							// Initialize position & direction from ROM
							en_x [gpit] = en_x1 [gpit];
							en_y [gpit] = en_y1 [gpit];
							en_mx [gpit] = ADD_SIGN2 (en_x2 [gpit], en_x1 [gpit], rda);
							en_my [gpit] = ADD_SIGN2 (en_y2 [gpit], en_y1 [gpit], rda);
						#endif

						// emerging sense
						rda = ABS (en_mx [gpit]); if (!rda) rda = ABS (en_my [gpit]);
						rda --;

						// Sense
						rdb = (en_x1 [gpit] != en_x2 [gpit]) ? 
							SGNC (en_x2 [gpit], en_x1 [gpit], SAW_V_DISPL) :
							SGNC (en_y2 [gpit], en_y1 [gpit], SAW_V_DISPL);

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

						en_s [gpit] = PEZONS_BASE_SPRID;
						break;
				#endif

				#ifdef ENABLE_CHAC_CHAC
					case 10:
						// Cuchillas Chac Chac

						en_my [gpit] = (rda << 4);	// IDLE_1
						en_x [gpit] = en_x1 [gpit] >> 4;
						en_y [gpit] = (en_y1 [gpit] >> 4) - 1;
						en_alive [gpit] = 0;
						en_mx [gpit] = en_my [gpit];

						break;
				#endif

				#ifdef ENABLE_MONOCOCOS
					case 11:
						// Monococos
						en_mx [gpit] = 0; en_my [gpit] = MONOCOCO_BASE_TIME_HIDDEN - (rand8 () & 0x15);
						en_s [gpit] = MONOCOCO_BASE_SPRID;
						break;
				#endif	

				#ifdef ENABLE_SIMPLE_WARPERS
					case 0xff:
						en_mx [gpit] = rda;
						break;
				#endif				
			}

			#if (defined (ENABLE_FANTY) || defined (ENABLE_HOMING_FANTY)) && defined (FANTY_LIFE_GAUGE)
				en_life [gpit] = en_t [gpit] == 6 ? FANTY_LIFE_GAUGE : ENEMIES_LIFE_GAUGE;
			#else
				en_life [gpit] = ENEMIES_LIFE_GAUGE;
			#endif
			
			en_cttouched [gpit] = 0;
			en_spr_id [gpit] = en_s [gpit];
		}
		#if defined (PERSISTENT_DEATHS) || defined (PERSISTENT_ENEMIES)
			rdc ++;
		#endif
	}
}

#ifdef ENEMS_MAY_DIE
	void enems_kill () {
		_en_t = 0;

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

	void enems_hit (void) {
		_en_facing = ((_en_x < prx) ? 0 : 4);
		en_cttouched [gpit] = ENEMS_TOUCHED_FRAMES;
		en_life [gpit] --; 

		if (en_life [gpit] == 0) {
			#ifdef ENABLE_PURSUERS
				if (_en_t == 7) {
					en_alive [gpit] = 0;
					_en_ct = DEATH_COUNT_EXPRESSION;
					en_life [gpit] = ENEMIES_LIFE_GAUGE;
				} else 
			#endif
			{
				enems_kill ();
			}
		}
	}
#endif

void enems_move (void) {
#ifndef PLAYER_TOP_DOWN	
	pgotten = pgtmx = pgtmy = 0;
#endif
	
	// Updates sprites
	touched = 0;
	en_initial ++; if (en_initial >= 3) en_initial = 0;
	gpit = en_initial;
	gpjt = 3; while (gpjt --) {
		gpit += 2; if (gpit > 2) gpit -=3;
		
		// Copy arrays -> temporal vars in ZP

		__asm__ ("ldy %v", gpit);

		__asm__ ("lda %v, y", en_t);
		__asm__ ("sta %v", _en_t);

		__asm__ ("lda %v, y", en_s);
		__asm__ ("sta %v", _en_s);

		__asm__ ("lda %v, y", en_x);
		__asm__ ("sta %v", _en_x);

		__asm__ ("lda %v, y", en_y);
		__asm__ ("sta %v", _en_y);

		__asm__ ("lda %v, y", en_x1);
		__asm__ ("sta %v", _en_x1);

		__asm__ ("lda %v, y", en_x2);
		__asm__ ("sta %v", _en_x2);

		__asm__ ("lda %v, y", en_y1);
		__asm__ ("sta %v", _en_y1);

		__asm__ ("lda %v, y", en_y2);
		__asm__ ("sta %v", _en_y2);

		__asm__ ("lda %v, y", en_mx);
		__asm__ ("sta %v", _en_mx);

		__asm__ ("lda %v, y", en_my);
		__asm__ ("sta %v", _en_my);

		__asm__ ("lda %v, y", en_ct);
		__asm__ ("sta %v", _en_ct);

		__asm__ ("lda %v, y", en_facing);
		__asm__ ("sta %v", _en_facing);

		#if defined (ENABLE_FANTY) || defined (ENABLE_HOMING_FANTY)
			_enf_x = enf_x [gpit]; _enf_vx = enf_vx [gpit];
		#endif
		#if defined (ENABLE_FANTY) || defined (ENABLE_HOMING_FANTY) || defined (ENABLE_PEZONS)
			_enf_y = enf_y [gpit]; _enf_vy = enf_vy [gpit];
		#endif		
		
		// Clear selected sprite

		en_spr = 0xff;

		// "touched" state control

		#ifdef ENEMS_MAY_DIE
			if (en_cttouched [gpit]) {
				en_cttouched [gpit] --;
				#ifdef ENEMS_FLICKER
					if (half_life) {
						#ifdef ENEMS_ENABLE_DYING_FRAME
							rda = _en_s + _en_facing + 3;
							if (spr_enems [rda]) en_spr = rda; 
							else
						#endif
						en_spr = en_spr_id [gpit];	
					} 
				#else
					oam_index = oam_meta_spr (
						_en_x, _en_y + SPRITE_ADJUST, 
						oam_index, 
						spr_enems [ENEMS_EXPLODING_CELL]
					);
					#ifndef ENEMS_EXPLODING_CELLS_HIDES
						en_spr = en_spr_id [gpit];
					#endif
				#endif
			} 
		#endif
		
		// Enemy is considered active if...

		if (_en_t && en_cttouched [gpit] == 0) {

			// Gotten preliminary:
			pregotten = (prx + 7 >= _en_x && prx <= _en_x + 15);

			// Select frame upon screen position:
			en_fr = ((((_en_mx) ? _en_x : _en_y)+4) >> 3) & 1;

			// Means don't render (can/will be overwritten):
			
			#ifdef ENABLE_RESONATORS
				if (res_on 
					#ifdef ENABLE_SAW
						&& _en_t != 8 
					#endif
					#ifdef ENABLE_CHAC_CHAC
						&& _en_t != 10
					#endif
					#ifndef PLAYER_TOP_DOWN
						&& _en_t != 4
					#endif
				) {
					en_spr = en_spr_id [gpit];
				} else
			#endif
			{
				switch (_en_t) {
					case 1:
					case 2:
					case 3:
					case 4:
					#ifdef ENABLE_SHOOTIES
						case 12:
						case 13:
						case 14:
						case 15:
					#endif
					#ifdef ENABLE_PUNCHIES
						case 16:
						case 17:
						case 18:
						case 19:
					#endif
						#include "engine/enemmods/enem_linear.h"
						#ifdef ENABLE_SHOOTIES
							#include "engine/enemmods/enem_shooty.h"
						#endif				
						#ifdef ENABLE_PUNCHIES
							#include "engine/enemmods/enem_punchy.h"
						#endif
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

					#ifdef ENABLE_SIMPLE_WARPERS
						case 0xff:
							en_spr = SIMPLE_WARPERS_BASE_SPRID;
							break;
					#endif
				}

				// Store corrent sprite frame as calculated
				
				en_spr_id [gpit] = en_spr;
			}

			// Warp player?

			#ifdef ENABLE_SIMPLE_WARPERS
				if (_en_t == 0xff) {
					if (collide (prx, pry, _en_x, _en_y)
					#ifdef SIMPLE_WARPERS_FIRE_BUTTON
						&& (pad_this_frame & PAD_B)
					#endif
					) {
						n_pant = _en_mx; on_pant = 0xff;
						prx = _en_x2; px = prx << FIXBITS;
						pry = _en_y2; py = pry << FIXBITS;

						#if defined (SIMPLE_WARPERS_FIRE_BUTTON) && (defined (PLAYER_PUNCHES) || defined (PLAYER_KICKS))
							phitteract = 0;
							#ifdef PLAYER_PUNCHES
								ppunching = 0;
							#endif
							#ifdef PLAYER_KICKS
								pkicking = 0;
							#endif
						#endif

						b_button = 0;
						break;
					}
					goto skipdo;
				}
			#endif

			// Moving platforms

			#ifndef PLAYER_TOP_DOWN
				if (_en_t == 4 && pregotten && !pgotten && !pj) {
					
					// Horizontal moving platforms
					
					if (_en_mx) {
						if (pry + 16 >= _en_y && pry + 12 <= _en_y) {
							pgotten = 1;
							pgtmx = _en_mx << (6 - en_status [gpit]);
							py = (_en_y - 16) << 6; pry = py >> 6;
						}
					}
					
					// Vertical moving platforms
					
					if (
						(_en_my < 0 && pry + 17 >= _en_y && pry + 12 <= _en_y) ||
						(_en_my > 0 && pry + 16 + _en_my >= _en_y && pry + 12 <= _en_y)
					) {
						pgotten = 1;
						pgtmy = _en_my << (6 - en_status [gpit]);
						py = (_en_y - 16) << 6; pry = py >> 6;
						pvy = 0;
					}
				}
			#endif

			// Is enemy collidable? If not, exit

			if (
					_en_t == 0	// General condition.
				#ifndef PLAYER_TOP_DOWN				
					|| _en_t == 4
				#endif
				#ifdef ENABLE_SAW
					|| (_en_t == 8 && en_alive [gpit] == 0)
				#endif
				#ifdef ENABLE_PEZONS
					|| (_en_t == 9 && en_alive [gpit] == 0)
				#endif
				#ifdef ENABLE_CHAC_CHAC
					|| _en_t == 10
				#endif
				#ifdef ENABLE_MONOCOCOS
					|| (_en_t == 11 && _en_mx != 2)
				#endif
			) goto skipdo;

			// Collide with player (includes step over enemy)

			// Step over enemy?
			#if defined (PLAYER_HAS_JUMP) && (defined (PLAYER_STEPS_ON_ENEMS) || defined (PLAYER_SAFE_LANDING))
				if (
					pregotten && pry < _en_y && pry + 15 > _en_y && pvy > 0 &&
					pgotten == 0 &&	ppossee == 0
					#ifdef PLAYER_MIN_KILLABLE
						&& _en_t >= PLAYER_MIN_KILLABLE
					#endif
					#ifdef ENABLE_SAW
						&& _en_t != 8
					#endif
					// CUSTOM {
						&& has_boots
					// } END_OF_CUSTOM
				) {
					#ifdef PLAYER_SAFE_LANDING
						if (_en_my < 0) _en_my = -_en_my;
					#else
						#ifdef ENABLE_RESONATORS
							if (res_on)
						#endif
						enems_hit ();
					#endif

					if (i & PAD_A) {
						pj = 1; pctj = 0; pvy = -PLAYER_VY_JUMP_INITIAL;
						sfx_play (7, 0);
					} else {
						pvy = -PLAYER_VY_JUMP_INITIAL << 1;
					}

					if (pry > _en_y - ENEMS_UPPER_COLLISION_BOUND) { pry = _en_y - ENEMS_UPPER_COLLISION_BOUND; py = pry << FIXBITS; }

					touched = 1;
				} else
			#endif

			if (
				touched == 0 &&
				pstate == EST_NORMAL &&
				collide (prx, pry, _en_x, _en_y)
			) {
				#ifdef PLAYER_BOUNCES
					pvx = ADD_SIGN (_en_mx, PLAYER_V_REBOUND); _en_mx = ADD_SIGN (_en_x - prx, ABS (_en_mx));
					pvy = ADD_SIGN (_en_my, PLAYER_V_REBOUND); if (!_en_mx) _en_my = ADD_SIGN (_en_y - pry, ABS (_en_my));
				#endif

				#if defined ENEMIES_SUFFER_ON_PLAYER_COLLISION
					enems_hit ();
				#endif

				#ifdef ENABLE_RESONATORS
					if (
						res_on == 0 
						#ifdef ENABLE_SAW
						|| _en_t == 8
						#endif
					)
				#endif
				{
					pkill = 1;
					touched = 1;
				}
			}

			// Is enemy killable? If not, exit

			if (
				touched
				#ifdef PLAYER_MIN_KILLABLE
					|| _en_t < PLAYER_MIN_KILLABLE
				#endif
				#ifdef ENABLE_SAW
					|| _en_t == 8
				#endif
			) goto skipdo;

			// Hit enemy

			#if defined (PLAYER_PUNCHES) || defined (PLAYER_KICKS)
				if (phitteract) {
					if (
						_en_x + 7 >= phitterx && _en_x <= phitterx + 7 &&
						#ifdef TALL_COLLISION
							phittery + 15 >= _en_y &&
						#else
							phittery + 7 >= _en_y &&
						#endif					
						phittery <= _en_y + 7
					) {
						enems_hit ();
						phitteract = 0;
						pfrozen = PLAYER_FROZEN_FRAMES;
					}
				} 
			#endif

			#ifdef PLAYER_CAN_FIRE
				// Bullets
				bi = MAX_BULLETS; while (bi --) if (bst [bi]) {
					#ifdef ENABLE_PURSUERS
						if (_en_t != 7 || en_alive [gpit] == 2)
					#endif
					
					if (collide_in (bx [bi] + 3, by [bi] + 3, _en_x, _en_y)) {
						sfx_play (6, 2);
						bullets_destroy ();
						enems_hit ();
						break;
					}
				}
			#endif
		} 

skipdo: 
		// Render enemy metasprite en_spr

		if (en_spr != 0xff) {
			oam_index = oam_meta_spr (
				_en_x, _en_y + SPRITE_ADJUST, 
				oam_index, 
				spr_enems [en_spr]
			);
		}

		// Update arrays

		__asm__ ("ldy %v", gpit);

		__asm__ ("lda %v", _en_t);
		__asm__ ("sta %v, y", en_t);

		__asm__ ("lda %v", _en_x);
		__asm__ ("sta %v, y", en_x);

		__asm__ ("lda %v", _en_y);
		__asm__ ("sta %v, y", en_y);

		__asm__ ("lda %v", _en_x1);
		__asm__ ("sta %v, y", en_x1);

		__asm__ ("lda %v", _en_x2);
		__asm__ ("sta %v, y", en_x2);

		__asm__ ("lda %v", _en_y1);
		__asm__ ("sta %v, y", en_y1);

		__asm__ ("lda %v", _en_y2);
		__asm__ ("sta %v, y", en_y2);

		__asm__ ("lda %v", _en_mx);
		__asm__ ("sta %v, y", en_mx);

		__asm__ ("lda %v", _en_my);
		__asm__ ("sta %v, y", en_my);

		__asm__ ("lda %v", _en_ct);
		__asm__ ("sta %v, y", en_ct);

		__asm__ ("lda %v", _en_facing);
		__asm__ ("sta %v, y", en_facing);

		#if defined (ENABLE_FANTY) || defined (ENABLE_HOMING_FANTY)
			enf_x [gpit] = _enf_x; enf_vx [gpit] = _enf_vx;
		#endif
		#if defined (ENABLE_FANTY) || defined (ENABLE_HOMING_FANTY) || defined (ENABLE_PEZONS)
			enf_y [gpit] = _enf_y; enf_vy [gpit] = _enf_vy;
		#endif

	}	
}
