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

void enems_update_unsigned_char_arrays (void) {
	__asm__ ("ldy %v", gpit);

	__asm__ ("lda %v", _en_t);
	__asm__ ("sta %v, y", en_t);

	__asm__ ("lda %v", _en_s);
	__asm__ ("sta %v, y", en_s);

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
}

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
				_en_t = 0;
				gp_gen += 4;
			} else 
		#endif
		{
			// First get T, then do whatever I need
			_en_t = *gp_gen ++;

			// General...
			en_alive [gpit] = 0;

			// YX1
			rda = *gp_gen ++;
			_en_y1 = rda & 0xf0;
			_en_x1 = rda << 4;

			// YX2
			rda = *gp_gen ++;
			_en_y2 = rda & 0xf0;
			_en_x2 = rda << 4;
		
			// P, here used for speed
			rda = *gp_gen ++;

			#ifdef PERSISTENT_ENEMIES
				// Copy position & direction from ep_*
				_en_x = ep_x [rdc];
				_en_y = ep_y [rdc];
			#else
				// Initialize position & direction from ROM
				_en_x = _en_x1;
				_en_y = _en_y1;
			#endif

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
					_en_ct = 0;
	
					// Linear enems.
					#ifdef ENABLE_PUNCHIES
						if (_en_t >= 16) {
							en_rawv [gpit] = 2;
							_en_s = PUNCHIES_BASE_SPRID + ((_en_t - 16) << 3);
						} else
					#endif					
					#ifdef ENABLE_SHOOTIES
						if (_en_t >= 12) {
							en_rawv [gpit] = 1;
							_en_s = SHOOTIES_BASE_SPRID + ((_en_t - 12) << 3);
						} else
					#endif
					{
						en_rawv [gpit] = 0;
						_en_s = (_en_t - 1) << 3;
					}

					#ifdef PERSISTENT_ENEMIES
						_en_mx = ep_mx [rdc];
						_en_my = ep_my [rdc];
					#else
						_en_mx = ADD_SIGN2 (_en_x2, _en_x1, rda);
						_en_my = ADD_SIGN2 (_en_y2, _en_y1, rda);
					#endif

					// HL conversion		

					if (rda == 1) {
						en_status [gpit] = 1; 
					} else {
						en_status [gpit] = 0;
						#ifndef PERSISTENT_ENEMIES
							_en_mx >>= 1;
							_en_my >>= 1;
						#endif
					}

					// Fix limits so 1 < 2 always.
					if (_en_x1 > _en_x2) { rda = _en_x1; _en_x1 = _en_x2; _en_x2 = rda; }
					if (_en_y1 > _en_y2) { rda = _en_y1; _en_y1 = _en_y2; _en_y2 = rda; }
					
					break;

				#ifdef ENABLE_STEADY_SHOOTERS
					case 5:
						// _en_my = direction (LEFT UP RIGHT DOWN)
						if (_en_x2 > _en_x1) _en_my = 2;
						else if (_en_x2 < _en_x1) _en_my = 0;
						else if (_en_y2 > _en_y1) _en_my = 3;
						else _en_my = 1;
						_en_s = STEADY_SHOOTERS_BASE_SPRID + _en_my;

						// _en_mx = frequency from the attr
						_en_ct = _en_mx = rda;
						break;
				#endif

				#if defined (ENABLE_FANTY) || defined (ENABLE_HOMING_FANTY)
					case 6:
						// Fantys
						enf_x [gpit] = _en_x << 6;
						enf_y [gpit] = _en_y << 6;
						enf_vx [gpit] = enf_vy [gpit] = 0;
						_en_s = FANTY_BASE_SPRID;
						break;
				#endif

				#ifdef ENABLE_PURSUERS		
					case 7:
						// Pursuers
						_en_ct = DEATH_COUNT_EXPRESSION;	
						#ifdef ENABLE_GENERATORS
							en_generator_life [gpit] = GENERATOR_LIFE_GAUGE;
							gen_was_hit [gpit] = 0;
						#endif	
						_en_s = ((TYPE_7_FIXED_SPRITE - 1) << 3);
						break;
				#endif	

				#ifdef ENABLE_SAW
					case 8:
						// Saws
						#ifdef PERSISTENT_ENEMIES
							// Initialize position & direction from ROM
							_en_x = _en_x1;
							_en_y = _en_y1;							
						#endif
						_en_mx = ADD_SIGN2 (_en_x2, _en_x1, rda);
						_en_my = ADD_SIGN2 (_en_y2, _en_y1, rda);

						// emerging sense
						rda = ABS (_en_mx); if (!rda) rda = ABS (_en_my);
						rda --;

						// Sense
						rdb = (_en_x1 != _en_x2) ? 
							SGNC (_en_x2, _en_x1, SAW_V_DISPL) :
							SGNC (_en_y2, _en_y1, SAW_V_DISPL);

						// Store:
						_en_my = rda; // EMERGING SENSE
						_en_mx = rdb; // MOVING SENSE

						en_alive [gpit] = 1;
						_en_ct = SAW_EMERGING_STEPS;

						break;
				#endif		

				#ifdef ENABLE_PEZONS
					case 9:
						// Pezones

						// Initialize
						_en_my = PEZON_WAIT + (rda << 3);	// Speed in colocador defines idle time! (x8)
						_en_mx = _en_my;

						_en_s = PEZONS_BASE_SPRID;
						break;
				#endif

				#ifdef ENABLE_CHAC_CHAC
					case 10:
						// Cuchillas Chac Chac

						_en_my = (rda << 4);	// IDLE_1
						_en_x = _en_x1 >> 4;
						_en_y = (_en_y1 >> 4) - 1;
						_en_mx = _en_my;

						break;
				#endif

				#ifdef ENABLE_MONOCOCOS
					case 11:
						// Monococos
						_en_mx = 0; _en_my = MONOCOCO_BASE_TIME_HIDDEN - (rand8 () & 0x15);
						_en_s = MONOCOCO_BASE_SPRID;
						break;
				#endif	

				#ifdef ENABLE_COMPILED_ENEMS
					case 20:
						_en_ct = 0;
						_en_s = COMPILED_ENEMS_BASE_SPRID;
						en_behptr [gpit] = en_behptrs [rda];
						break;
				#endif

				#ifdef ENABLE_SIMPLE_WARPERS
					case 0xff:
						_en_mx = rda;
						break;
				#endif				
			}

			#if (defined (ENABLE_FANTY) || defined (ENABLE_HOMING_FANTY)) && defined (FANTY_LIFE_GAUGE)
				en_life [gpit] = _en_t == 6 ? FANTY_LIFE_GAUGE : ENEMIES_LIFE_GAUGE;
			#else
				en_life [gpit] = ENEMIES_LIFE_GAUGE;
			#endif
			
			en_cttouched [gpit] = 0;
			en_spr_id [gpit] = _en_s;
		}
		#if defined (PERSISTENT_DEATHS) || defined (PERSISTENT_ENEMIES)
			rdc ++;
		#endif

		enems_update_unsigned_char_arrays ();
	}
}

#ifdef ENEMS_MAY_DIE
	void enems_kill () {
		sfx_play (SFX_BREAKB, 1); 
		
		_en_t = 0;

		#ifdef PERSISTENT_DEATHS
			ep_flags [en_offs + gpit] &= 0xFE;
		#endif

		#ifdef ACTIVATE_SCRIPTING
			run_script (2 * MAP_SIZE + 5);
		#endif

		#if defined (ENABLE_STEADY_SHOOTERS) && !(defined (STEADY_SHOOTER_KILLABLE) && defined (STEADY_SHOOTER_COUNT))
			if (_ent != 5)
		#endif
		{
			pkilled ++;
			#ifdef COUNT_KILLED_IN_FLAG
				flags [COUNT_KILLED_IN_FLAG] ++;
			#endif
		}
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
					#ifndef PLAYER_TOP_DOWN
						&& _en_t != 4
					#endif
					#ifdef ENABLE_STEADY_SHOOTERS
						&& _en_t != 5
					#endif
					#ifdef ENABLE_SAW
						&& _en_t != 8 
					#endif
					#ifdef ENABLE_CHAC_CHAC
						&& _en_t != 10
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

					#ifdef ENABLE_STEADY_SHOOTERS
						case 5:
							#include "engine/enemmods/enem_steady_shooter.h"
							break;
					#endif

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

					#ifdef ENABLE_COMPILED_ENEMS
						case 20:
							#include "engine/enemmods/enem_compiled.h"
							break;
					#endif
				}

				// Store corrent sprite frame as calculated
				
				en_spr_id [gpit] = en_spr;
			}

			// Warp player?

			#ifdef ENABLE_SIMPLE_WARPERS
				if (_en_t == 0xff) {
					if (collide ()
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
				#ifdef ENABLE_PURSUERS
					|| (_en_t == 7 && en_alive [gpit] != 2)
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
					#ifndef STEADY_SHOOTER_KILLABLE
						&& _en_t != 5
					#endif	
					#ifdef ENABLE_SAW
						&& _en_t != 8
					#endif
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
						sfx_play (SFX_JUMP, 0);
					} else {
						sfx_play (SFX_STEPON, 1);
						pvy = -PLAYER_VY_JUMP_INITIAL << 1;
					}

					if (pry > _en_y - ENEMS_UPPER_COLLISION_BOUND) { pry = _en_y - ENEMS_UPPER_COLLISION_BOUND; py = pry << FIXBITS; }

					touched = 1;
				} else
			#endif

			if (
				touched == 0 &&
				pstate == EST_NORMAL &&
				collide ()
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
				#ifndef STEADY_SHOOTER_KILLABLE
					|| _en_t == 5
				#endif					
				#ifdef ENABLE_SAW
					|| _en_t == 8
				#endif
			) goto skipdo;

			// Hit enemy

			#if defined (PLAYER_PUNCHES) || defined (PLAYER_KICKS)
				if (phitteract) {
					if (
						phitterx + 7 >= _en_x &&
						phitterx <= _en_x + 7 &&
						phittery + 7 + ENEMS_COLLISION_VSTRETCH_FG >= _en_y &&
						phittery <= _en_y + 12
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
						sfx_play (SFX_ENHIT, 1);
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

		enems_update_unsigned_char_arrays ();

		#if defined (ENABLE_FANTY) || defined (ENABLE_HOMING_FANTY)
			enf_x [gpit] = _enf_x; enf_vx [gpit] = _enf_vx;
		#endif
		#if defined (ENABLE_FANTY) || defined (ENABLE_HOMING_FANTY) || defined (ENABLE_PEZONS)
			enf_y [gpit] = _enf_y; enf_vy [gpit] = _enf_vy;
		#endif

	}	
}