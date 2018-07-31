// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// player.h
// Player movement & stuff

#ifdef DIE_AND_RESPAWN
	void player_register_safe_spot (void) {
		px_safe = px;
		py_safe = py;
		n_pant_safe = n_pant;
	}
#endif

void player_stop (void) {
	pvx = pvy = 0;
}

void player_init (void) {
	// Init player data

	if (!warp_to_level) player_stop ();

	#ifdef PLAYER_TOP_DOWN	
		pfacing = CELL_FACING_DOWN;
	#else
		pfacing = 0;
	#endif	

	pfr = pctfr = 0;
	pj = pctj = 0;
	psprid = 0;

	#ifndef DEACTIVATE_OBJECTS
		pobjs = 0;
	#endif

	#ifndef DEACTIVATE_KEYS
		pkeys = 0;
	#endif

	pgotten = 0;
	pfiring = 0;

	#ifdef ENEMS_MAY_DIE
		pkilled = 0;
	#endif

	#ifdef PLAYER_CAN_FIRE
		#ifdef MAX_AMMO
			#ifdef INITIAL_AMMO
				pammo = INITIAL_AMMO
			#else
				pammo = MAX_AMMO;
			#endif
		#endif
		#ifdef PLAYER_CHARGE_AND_FIRE
			pfiregauge = pfiregauge = 0;
		#endif
	#endif

	pflickering = pbouncing = 0;

	#ifdef DIE_AND_RESPAWN
		player_register_safe_spot ();
	#endif

	#ifdef CARRY_ONE_HS_OBJECT
		pinv = HS_OBJ_EMPTY; 
	#endif

	// Default engine type
	#ifndef PLAYER_TOP_DOWN
		#ifdef PLAYER_HAS_JUMP
			vertical_engine_type = ENGINE_TYPE_JUMP;
		#elif defined (PLAYER_HAS_JETPAC)
			vertical_engine_type = ENGINE_TYPE_JET_PAC;
		#elif defined (PLAYER_AUTO_JUMP)
			vertical_engine_type = ENGINE_TYPE_AUTO_JUMP;
		#elif defined (PLAYER_SWIMS)
			vertical_engine_type = ENGINE_TYPE_SWIM;
		#endif
	#endif

	#ifdef ENABLE_TRAMPOLINES
		ptrampoline = 0;
	#endif

	#ifdef PLAYER_SPINS	
		pspin = 0;
	#endif	
}

void player_render (void) {
	if (0 == pflickering || half_life) 
		oam_index = oam_meta_spr (
			prx, pry + SPRITE_ADJUST, 
			oam_index, 
			spr_player [psprid]
		);
}

void player_to_pixels (void) {
	prx = px >> FIXBITS;
	pry = py >> FIXBITS;
}

void player_kill (void) {
	player_render ();
	update_cycle ();

	pkill = phit = 0;
	sfx_play (SFX_PHIT, 0);
	
	if (plife) -- plife; else game_over = 1;

	#ifdef PLAYER_FLICKERS
		pflickering = PLAYER_FLICKERS;
	#endif

	#ifdef PLAYER_BOUNCES
		pbouncing = PLAYER_BOUNCES;
	#endif

	#ifdef ENABLE_USE_ANIM
		use_ct = 0;
	#endif

	#ifdef DIE_AND_RESPAWN
		music_pause (1);
		delay (60);
		
		#ifdef DIE_AND_REINIT
			level_reset = 1;
		#else
			px = px_safe; 
			py = py_safe; 
			player_to_pixels ();
			n_pant = n_pant_safe;		
			player_stop ();
			music_pause (0);
		#endif

		// May be necessary to find a proper cell later on
		#if defined (ENABLE_BREAKABLE)
			pmayneedrelocation = 1;
		#endif
	#endif	

	#ifdef DIE_AND_REENTER
		on_pant = 0xff;
	#endif
}

#if defined(PLAYER_PUSH_BOXES) || !defined(DEACTIVATE_KEYS)
	#include "engine/playermods/process_tile.h"
#endif

#ifdef PLAYER_CAN_FIRE
	#include "engine/playermods/bullets.h"
#endif

void player_move (void) {
	if (pflickering) -- pflickering;
	if (pbouncing) -- pbouncing;

	#if defined (PLAYER_PUNCHES) || defined (PLAYER_KICKS)
		if (pfrozen) {
			-- pfrozen; 
			if (pfrozen == 0) {
			#ifdef PLAYER_PUNCHES
				ppunching = 0;
			#endif
			#ifdef PLAYER_KICKS
				pkicking = 0;
			#endif
			} else return;
		}
	#endif

	#ifdef ENABLE_USE_ANIM
		if (use_ct) {
			if (use_sub_ct) -- use_sub_ct; else {
				if (use_ct < USE_ANIM_MAX_FRAMES) {
					++ use_ct;
					use_sub_ct = (use_ct == USE_ANIM_MAX_FRAMES) ? 50 : USE_ANIM_FRAMES_PER_STEP;
					#ifdef ACTIVATE_SCRIPTING
						if (
							use_type == USE_TYPE_SCRIPTING && 
							fire_script_success == 0 && 
							use_ct == USE_ANIM_MAX_FRAMES
						) ++ use_ct;
					#endif
				} else use_ct = 0;
			}
			// Invalidate pad input
			a_button = b_button = pad0 = 0;
		}
	#endif

	hitv = hith = 0;
	pnotsafe = 0;
	#ifdef ENABLE_SLIPPERY
		pice = 0;
	#endif

	// ******************
	// Initial detections
	// ******************

	#ifdef NEEDS_INITIAL_DETECTION
		cx1 = prx >> 4; cx2 = (prx + 7) >> 4;
		cy1 = cy2 = (pry + 15) >> 4;
		cm_two_points ();
	#endif

	#ifdef ENABLE_LADDERS
		rdb = ponladder;
		ponladder = (!pj && at1 == 32 && at2 == 32);
	#endif

	#ifdef ENABLE_PROPELLERS
		rda = pfloating;
		pfloating = (at1 == 64 || at2 == 64);
		if (rda != pfloating) sfx_play (SFX_FLOAT, 0);
	#endif

	#ifdef ENABLE_SPRINGS
		if (springs_on && cy1 < 12) {
			if (cy1) -- cy1;
			_t = SPRING_SPIKE_TILE; 
			#ifdef SPRINGS_NEED_POSSEE
			if (ppossee)
			#endif
			{
				if (QTILE (cx1, cy1 + 1) == SPRING_TILE && QTILE (cx1, cy1) != SPRING_SPIKE_TILE) { _x = cx1; _y = cy1; map_set (); sfx_play (SFX_SPRING, 1);}
				if (QTILE (cx2, cy1 + 1) == SPRING_TILE && QTILE (cx1, cy1) != SPRING_SPIKE_TILE) { _x = cx2; _y = cy1; map_set (); sfx_play (SFX_SPRING, 1);}
			}
		}
	#endif

	#ifdef ENABLE_TRAMPOLINES
		if (at1 == 66 || at2 == 66) {
			a_button = 1; ptrampoline = 1;
			#ifdef PLAYER_SPINS	
				pspin = 0;
			#endif
		}
	#endif

	oppossee = ppossee;
	ppossee = 0;

	// ********
	// Vertical
	// ********

	#ifdef PLAYER_TOP_DOWN		
		// Controller 

		if (!(pad0 & PAD_UP || pad0 & PAD_DOWN)) {
			pfacingv = 0xff;
			if (pvy > 0) {
				pvy -= PLAYER_RX;
				if (pvy < 0)
					pvy = 0;
			} else if (pvy < 0) {
				pvy += PLAYER_RX;
				if (pvy > 0)
					pvy = 0;
			}
		}
		
		if (pad0 & PAD_UP) {
			pfacingv = CELL_FACING_UP;
			if (pvy > -PLAYER_VX_MAX) {
				pvy -= PLAYER_AX;
			}
		}
		
		if (pad0 & PAD_DOWN) {
			pfacingv = CELL_FACING_DOWN;
			if (pvy < PLAYER_VX_MAX) {
				pvy += PLAYER_AX;
			}
		}

	#else
		// Gravity

		#ifdef ENABLE_LADDERS
			// Special cases: in and out the ladder.

			if (ponladder == 0) {
				if (pad0 & PAD_DOWN) {
					cy1 = cy2 = (pry + 16) >> 4;
					cm_two_points ();
					ponladder = (!pj && at1 == 32 && at2 == 32);
				}

				if ((pad0 & PAD_UP) && rdb) pvy = 0;
			}

			if (ponladder) {
				if (pad0 & PAD_UP) {
					pvy = -PLAYER_VY_LADDERS;
				} else if (pad0 & PAD_DOWN) {
					pvy = PLAYER_VY_LADDERS;
				} else pvy = 0;

				cy1 = cy2 = (pry + 4) >> 4;
				cm_two_points ();
				phalfladder = (at1 != 32) && (at2 != 32);
			} else
		#endif

		#ifdef PLAYER_CAN_FLOAT 
			if (pfloating) {
				pvy -= PLAYER_AY_FLOAT; 
				if (pvy < -PLAYER_VY_FLOAT_MAX) pvy = -PLAYER_VY_FLOAT_MAX;
			} else
		#endif

		if (vertical_engine_type != ENGINE_TYPE_SWIM) {
			#ifdef PLAYER_JUMP_TYPE_MK2
				if (
					!pgotten
				) {
					if (!pj) {
						pvy += PLAYER_G;
					} else pvy += PLAYER_G_MK2_JUMPING;
					if (pvy > PLAYER_VY_FALLING_MAX) pvy = PLAYER_VY_FALLING_MAX; 
				}
			#else
				if (!pj) {
					pvy += PLAYER_G;
					if (pvy > PLAYER_VY_FALLING_MAX) pvy = PLAYER_VY_FALLING_MAX; 
				}
			#endif
		}

		// Moving platforms invalidate pvy

		if (pgotten) pvy = 0;			
	#endif

	#ifdef PLAYER_HAS_JETPAC
	    // Controller 

		if (vertical_engine_type == ENGINE_TYPE_JET_PAC) {
			if (pad0 & PAD_A) {
				pvy -= PLAYER_AY_JETPAC;
				if (pvy < -PLAYER_VY_JETPAC_MAX) pvy = -PLAYER_VY_JETPAC_MAX;
			}
		}
	#endif

	#ifdef PLAYER_SWIMS
		// Controller 

		if (vertical_engine_type == ENGINE_TYPE_SWIM) {
			if (!(pad0 & (PAD_DOWN|PAD_A))) {
				pvy -= PLAYER_AY_SWIM >> 1;
			} else {
				if (pad0 & (PAD_DOWN|PAD_A)) {
					pvy += PLAYER_AY_SWIM;
				}
		
				// Limit
				if (pvy > PLAYER_VY_SWIM_MAX) {
					pvy = PLAYER_VY_SWIM_MAX;
				}
			}
			if (pvy < -PLAYER_VY_SWIM_MAX) {
				pvy = -PLAYER_VY_SWIM_MAX;
			}
		}
	#endif

	// Move
	py += pvy;
	if (py < 0) py = 0;
	
	// Collision
	player_to_pixels ();

	cx1 = prx >> 4;
	cx2 = (prx + 7) >> 4;

	#ifdef PLAYER_TOP_DOWN		
		if (pvy < 0)
	#else
		rds16 = pvy + pgtmy;
		if (rds16 < 0)
	#endif		
		{
			cy1 = cy2 = (pry - PLAYER_COLLISION_VSTRETCH_BG) >> 4;			
			cm_two_points ();
			if ((at1 & 8) || (at2 & 8)) {
				pry = ((cy1 + 1) << 4) + PLAYER_COLLISION_VSTRETCH_BG;
				pvy = 0; py = pry << FIXBITS;
				pgotten = 0;
				pfiring = 1;
				#if defined (PLAYER_TOP_DOWN) && (defined(PLAYER_PUSH_BOXES) || !defined(DEACTIVATE_KEYS))
					// Special obstacles
					                if (at1 & 2) player_process_tile (at1, cx1, cy1, cx1, cy1 - 1);
					if (cx1 != cx2) if (at2 & 2) player_process_tile (at2, cx2, cy1, cx2, cy1 - 1);
				#endif
			} else if ((at1 & 1) || (at2 & 1)) {
				hitv = 1;
			}
	#ifdef ENABLE_QUICKSANDS
			else if ((at1 == 2) || (at2 == 2)) {
				if (pctj > 2) pj = 0;
			}
	#endif		

	#ifdef PLAYER_TOP_DOWN			
		} else if (pvy > 0)
	#else
		} else if (rds16 > 0)
	#endif		
		{
			cy1 = cy2 = (pry + 16) >> 4; 
			cm_two_points (); 
			#ifdef PLAYER_TOP_DOWN
			if ((at1 & 8) || (at2 & 8)) 
			#else
	 		if (
				pry < ((cy1 - 1) << 4) + 4 && 
				(
					(at1 & 12) || (at2 & 12)
					#ifdef ENABLE_LADDERS
						|| (!ponladder && ((at1 & 32) && at2 & 32))
					#endif					
				)
			)
	 		#endif
			{
				pvy = 0; pry = ((cy1 - 1) << 4);py = pry << FIXBITS;
				pgotten = 0;
				pfiring = 1;
				ppossee = 1;
				
				#if defined (PLAYER_TOP_DOWN) && (defined(PLAYER_PUSH_BOXES) || !defined(DEACTIVATE_KEYS))
					                if (at1 & 2) player_process_tile (at1, cx1, cy1, cx1, cy1 + 1);
					if (cx1 != cx2) if (at2 & 2) player_process_tile (at2, cx2, cy1, cx2, cy1 + 1);			
				#endif

				#ifdef ENABLE_SLIPPERY
					pice = (at1 & 64) || (at2 & 64);
				#endif

				#ifdef ENABLE_CONVEYORS
					cfx = 0;
					                if ((at1 & 40) == 40) { if (at1 & 1) cfx = pgtmx = PLAYER_VX_CONVEYORS; else cfx = pgtmx = -PLAYER_VX_CONVEYORS; pgotten = 1; } 
					if (cx1 != cx2) if ((at2 & 40) == 40) { if (at2 & 1) cfx = pgtmx = PLAYER_VX_CONVEYORS; else cfx = pgtmx = -PLAYER_VX_CONVEYORS; pgotten = 1; } 
				#endif

				#if defined (ENABLE_BREAKABLE) && defined (BREAKABLE_WALKABLE)
					if (at1 & 16) { breakable_break (cx1, cy1 - 1); pnotsafe = 1; }
					if (cx1 != cx2 && (at2 & 16)) { breakable_break (cx2, cy1 - 1); pnotsafe = 1; }
				#endif

				if ((at1 & 1) || (at2 & 1)) pnotsafe = 1; 
			} else if ((at1 & 1) || (at2 & 1)) {
				hitv = 1;
			}
			#ifdef ENABLE_QUICKSANDS		
				else {
					if ((at1 == 2) || (at2 == 2)) {
						pvy = PLAYER_VY_SINKING;
						ppossee = 1;
					}
				}
			#endif
		}
	
	#ifdef PLAYER_HAS_JUMP
		// *******************************
		// Jump: PAD_A, change when needed
		// *******************************

		if (vertical_engine_type == ENGINE_TYPE_JUMP) {
			#ifdef PLAYER_JUMP_TYPE_MK2

				if (
					a_button 
					&& !pj
					&& (
						pgotten || ppossee || hitv
						#ifdef ENABLE_LADDERS
							|| ponladder
						#endif					
					)
				) {
					jump_start ();
					
					#ifdef DIE_AND_RESPAWN
						if (!(pgotten || hitv || pnotsafe)) {
							player_register_safe_spot ();
						}
					#endif

					#ifdef PLAYER_SPINS
						#ifdef ENABLE_TRAMPOLINES
							if (!ptrampoline)
						#endif
						pspin = 1;
					#endif
				}

				if (pj) {
					#ifdef ENABLE_TRAMPOLINES
						if (ptrampoline) {
							++ pctj; if (pctj == PLAYER_VY_MK2_TRAMPOLINE_A_STEPS)
							{ pj = 0; ptrampoline = 0; }
						} else 
					#endif
					if (pad0 & PAD_A) {
						++ pctj; if (pctj == PLAYER_VY_MK2_JUMP_A_STEPS) pj = 0;
					} else {
						pj = 0; if (pvy < -PLAYER_VY_MK2_JUMP_RELEASE) pvy = -PLAYER_VY_MK2_JUMP_RELEASE;
					}
				}

			#else
				
				if (
					a_button 
					&& !pj
					&& (
						pgotten || ppossee || hitv
						#ifdef ENABLE_LADDERS
							|| ponladder
						#endif
					)
				) {
					jump_start ();
					
					#ifdef DIE_AND_RESPAWN
						if (!(pgotten || hitv || pnotsafe)) {
							player_register_safe_spot ();
						}
					#endif	

					#ifdef PLAYER_SPINS
						#ifdef ENABLE_TRAMPOLINES
							if (!ptrampoline)
						#endif
						pspin = 1;
					#endif
				}
				
				#ifdef ENABLE_TRAMPOLINES
				if (pj && ptrampoline) {

					++ pctj; if (pctj == 32) pj = 0;
					if (pctj < PLAYER_AY_JUMP) pvy -= (PLAYER_AY_JUMP - (pctj));
				} else
				#endif
				if (pad0 & PAD_A) {
					if (pj) {
						if (pctj < PLAYER_AY_JUMP) pvy -= (32 - (pctj));
						if (pvy < -PLAYER_VY_JUMP_MAX) pvy = -PLAYER_VY_JUMP_MAX;
						++ pctj; if (pctj == 16) pj = 0;	
					}
				} else {
					pj = 0; 
				}
			#endif
		}
	#endif

	#ifdef PLAYER_AUTO_JUMP
		if (vertical_engine_type == ENGINE_TYPE_AUTO_JUMP) {
			if (
				!pj
				&& (
					pgotten || ppossee || hitv
					#ifdef ENABLE_LADDERS
						|| ponladder
					#endif
				)
			) {
				jump_start ();

				#ifdef DIE_AND_RESPAWN
					if (!(pgotten || hitv || pnotsafe)) {
						player_register_safe_spot ();
					}
				#endif	
			}

			if (pj) {
				if (pctj < PLAYER_AY_JUMP) pvy -= (PLAYER_AY_JUMP - (pctj));
				if (pvy < -PLAYER_VY_JUMP_MAX) pvy = -PLAYER_VY_JUMP_MAX;
				++ pctj; if (pctj == 16) pj = 0;	
			}
			
			if (pad0 & PAD_DOWN) {
				if (pvy < 0) pvy += PLAYER_AY_UNTHRUST;
			}
		}
	#endif

	#ifdef PLAYER_SPINS
		if (pad0 & PAD_DOWN) {
			if (ppossee && ABS (pvx) > PLAYER_VX_MIN) {
				pspin = 1; sfx_play (SFX_DUMMY2, 0);
			}
		}
	#endif

	// **********
	// Horizontal
	// **********
	
	// Poll pad
	if (!(pad0 & PAD_LEFT || pad0 & PAD_RIGHT)) {
		#ifdef PLAYER_TOP_DOWN		
			pfacingh = 0xff;
		#endif
		
		if (pvx > 0) {

			#ifdef ENABLE_SLIPPERY
				pvx -= pice ? PLAYER_RX_ICE : PLAYER_RX;
			#else			
				pvx -= PLAYER_RX;
			#endif			
			
			if (pvx < 0) pvx = 0;
		} else if (pvx < 0) {

			#ifdef ENABLE_SLIPPERY
				pvx += pice ? PLAYER_RX_ICE : PLAYER_RX;
			#else
				pvx += PLAYER_RX;
			#endif

			if (pvx > 0) pvx = 0;
		}
	}

	if (pad0 & PAD_LEFT) {
		#ifdef PLAYER_TOP_DOWN		
			pfacingh = CELL_FACING_LEFT;
		#else
			pfacing = CELL_FACING_LEFT;		
		#endif
		
		if (pvx > -PLAYER_VX_MAX) {
			
			#ifdef ENABLE_SLIPPERY
				pvx -= pice ? PLAYER_AX_ICE : PLAYER_AX;
			#else
				pvx -= PLAYER_AX;
			#endif
		}
	}
	
	if (pad0 & PAD_RIGHT) {
		#ifdef PLAYER_TOP_DOWN		
			pfacingh = CELL_FACING_RIGHT;
		#else
			pfacing = CELL_FACING_RIGHT;
		#endif		

		if (pvx < PLAYER_VX_MAX) {
		
			#ifdef ENABLE_SLIPPERY
				pvx += pice ? PLAYER_AX_ICE : PLAYER_AX;
			#else
				pvx += PLAYER_AX;
			#endif
		}
	}
	
	// Move
	px += pvx;
	#ifndef PLAYER_TOP_DOWN	
		if (pgotten) px += pgtmx;
	#endif
		
	if (px < (4<<FIXBITS)) prx = 4;
	else if (px > (244<<FIXBITS)) prx = 244; 
	else player_to_pixels ();
	
	// Collision

	#if PLAYER_COLLISION_VSTRETCH_BG <= 0
		cy1 = (pry - PLAYER_COLLISION_VSTRETCH_BG) >> 4;
		cy2 = (pry + 15) >> 4;
	#endif

	rds16 = pvx + pgtmx;
	if (rds16) 	{
		if (rds16 < 0) {
			cx1 = cx2 = prx >> 4; 
			rda = (cx1 + 1) << 4;
			rdm = cx1 - 1;
		} else {
			cx1 = cx2 = (prx + 8) >> 4;
			rda = ((cx1 - 1) << 4) + 8;
			rdm = cx1 + 1;
		}
		#if PLAYER_COLLISION_VSTRETCH_BG > 0
			cm_three_points ();
			if ((at1 & 8) || (at2 & 8) || (at3 & 8)) {
				pvx = 0; prx = rda; px = prx << FIXBITS; pfiring = 1;

				// Special obstacles
				#if (defined(PLAYER_PUSH_BOXES) || !defined(DEACTIVATE_KEYS))
					                if (at2 & 2) player_process_tile (at2, cx1, cy2, rdm, cy2);
					if (cy2 != cy3) if (at3 & 2) player_process_tile (at3, cx1, cy3, rdm, cy3);
				#endif				
			} else {
				hith = ((at1 & 1) || (at2 & 1) || (at3 & 1));
			}
		#else
			cm_two_points ();
			if ((at1 & 8) || (at2 & 8)) {
				pvx = 0; prx = rda; px = prx << FIXBITS; pfiring = 1;

				// Special obstacles
				#if (defined(PLAYER_PUSH_BOXES) || !defined(DEACTIVATE_KEYS))
					                if (at1 & 2) player_process_tile (at1, cx1, cy1, rdm, cy1);
					if (cy1 != cy2) if (at2 & 2) player_process_tile (at2, cx1, cy2, rdm, cy2);
				#endif				
			} else {
				hith = ((at1 & 1) || (at2 & 1));
			}
		#endif
	}

	// Facing
	#ifdef PLAYER_TOP_DOWN	
		#ifdef TOP_OVER_SIDE
			if (pfacingv != 0xff) {
				pfacing = pfacingv; 
			} else if (pfacingh != 0xff) {
				pfacing = pfacingh;
			}
		#else
			if (pfacingh != 0xff) {
				pfacing = pfacingh; 
			} else if (pfacingv != 0xff) {
				pfacing = pfacingv;
			}
		#endif
	#endif

	#ifdef PLAYER_SPINS
		if ((!pvx && (ppossee || pgotten) && !pj) || (ppossee && !oppossee)) pspin = 0;
	#endif

	// *************
	// Killing tiles
	// *************

	phit = 0;
	
	#ifdef NO_HORIZONTAL_EVIL_TILE
		if (hitv || hith) { phit = 1; pvy = ADD_SIGN (-pvy, PLAYER_V_REBOUND); } 
	#else
		if (hitv) { phit = 1; pvy = ADD_SIGN (-pvy, PLAYER_V_REBOUND); } 
		if (hith) { phit = 1; pvx = ADD_SIGN (-pvx, PLAYER_V_REBOUND); }
	#endif

	#if defined (ENABLE_CHAC_CHAC) || defined (ENABLE_TILE_CHAC_CHAC)
		cx1 = cx2 = (prx + 4) >> 4;
		cy1 = pry >> 4; cy2 = (pry + 15) >> 4;
		cm_two_points ();
		if ((at1 & 1) || (at2 & 1)) phit = 1;
	#endif

	if (!pflickering && !pbouncing) if (phit) { 
		player_to_pixels ();
		en_sg_2 = 1;

		#include "my/on_player_spike.h"

		if (en_sg_2)
			pkill = 1; 
		#ifdef PLAYER_SPINS
			pspin = 0;
		#endif
	}

	// ***********************
	// Center point detections
	// ***********************

	#ifdef NEEDS_CENTER_DETECTION
		cx1 = cx2 = (prx + 4) >> 4;
		cy1 = cy2 = (pry + 8) >> 4;
		cm_two_points ();
	#endif

	#ifdef ENABLE_TILE_GET
		if (cy1 && at1 == 34) {
			-- cy1;
			
			_x = cx1; _y = cy1; _t = 0; map_set ();
			sfx_play (SFX_RING, 2);
			
			#include "my/on_tile_got.h"

			#ifdef PERSISTENT_TILE_GET
				tile_got [(cy1 << 1) | (cx1 > 7)] |= bits [cx1 & 7];
			#endif
		}
	#endif

	// **************
	// B Button stuff
	// **************

	// (fire bullets, run scripting w/animation, do interactives)

	#ifdef PLAYER_CHARGE_AND_FIRE
		#ifdef PLAYER_FIRE_RELOAD
			if (pfirereload) -- pfirereload; 
			else
		#endif
		if (!b_button && (pad0 & PAD_B)) {
			if (pfiregauge < PLAYER_CHARGE_MAX) ++ pfiregauge;
		}

		if (0 == (pad0 & PAD_B)) {
			if (pfiregauge >= PLAYER_CHARGE_MIN) fire_bullet ();
			pfiregauge = 0;
		}
	#endif

	#if defined (ACTIVATE_SCRIPTING) || defined (ENABLE_INTERACTIVES) || defined (PLAYER_CAN_FIRE) || defined (PLAYER_PUNCHES)
		if (
			b_button
			#ifdef ENABLE_LADDERS
				&& !ponladder
			#endif
		) {
			#ifdef ACTIVATE_SCRIPTING
				#include "engine/playermods/scripting.h"
			#endif

			#if defined (PLAYER_CAN_FIRE) && !defined (PLAYER_CHARGE_AND_FIRE)
				#ifdef PLAYER_FIRE_RELOAD
					if (pfirereload) -- pfirereload; 
					else
				#endif
			
				if (b_button) fire_bullet ();
			#endif		
			
			#ifdef PLAYER_PUNCHES
				if (ppossee && ppunching == 0) { ppunching = 16; phitteract = 1; sfx_play (SFX_HITTER, 0); }
			#endif

			#ifdef PLAYER_KICKS
				if (!ppossee && pkicking == 0) { pkicking = 16; phitteract = 1; sfx_play (SFX_HITTER, 0); }
			#endif
		} 
	#endif

	#ifdef PLAYER_PUNCHES
		if (ppunching) {
			-- ppunching; if (ppunching < 12) phitteract = 0;
			phitterx = pfacing ? prx - PLAYER_PUNCH_OFFS_X : prx + PLAYER_PUNCH_OFFS_X;
			phittery = pry + PLAYER_PUNCH_OFFS_Y;
		} 
	#endif

	#ifdef PLAYER_KICKS
		if (pkicking) {
			-- pkicking; if (pkicking < 12) phitteract = 0;
			phitterx = pfacing ? prx - PLAYER_KICK_OFFS_X : prx + PLAYER_KICK_OFFS_X;
			phittery = pry + PLAYER_KICK_OFFS_Y;
			if (ppossee) pkicking = 0;
		} 
	#endif

	#if defined (ENABLE_BREAKABLE) && (defined (PLAYER_PUNCHES) || defined (PLAYER_KICKS))
		if (phitteract) {
			cx1 = (phitterx + 4) >> 4;
			cy1 = (phittery + 4 - 16) >> 4;
			if (ATTR(cx1, cy1) & 16) {
				breakable_break (cx1, cy1);
				pfrozen = PLAYER_FROZEN_FRAMES;
				phitteract = 0;
			}
		}
	#endif

	// **********
	// Calc frame
	// **********
	#include "my/player_frame_selector.h"

	prx_old = prx;
	pry_old = pry;
}
