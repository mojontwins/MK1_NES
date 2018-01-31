// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// player.h
// Player movement & stuff

#ifdef DIE_AND_RESPAWN
	void player_register_safe_spot (void) {
		px_safe = px;
		py_safe = py;
		n_pant_safe = n_pant;
	}
#endif

void player_init (void) {
	// Init player data

	pvx = pvy = 0;

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

	#ifdef PLAYER_CAN_FIRE
		pkilled = 0;

		#ifdef MAX_AMMO
			#ifdef INITIAL_AMMO
				pammo = INITIAL_AMMO
			#else
				pammo = MAX_AMMO;
			#endif
		#endif	
	#endif

	pstate = EST_NORMAL;

	#ifdef DIE_AND_RESPAWN
		player_register_safe_spot ();
	#endif

	#ifdef CARRY_ONE_HS_OBJECT
		pinv = HS_OBJ_EMPTY; 
	#endif

	#ifdef CARRY_ONE_FLAG_OBJ
		flags [HS_INV_FLAG] = HS_OBJ_EMPTY;
	#endif

	#ifdef ENABLE_CONTAINERS
		upd_cont_index = 0;
	#endif
}

void player_render (void) {
	if (pstate == EST_NORMAL || half_life) {
		oam_meta_spr (prx, pry + SPRITE_ADJUST, 4, spr_player [psprid]);	
	} else {
		oam_meta_spr (0, 240, 4, spr_pl_empty);
	}
}

void player_kill (void) {
	pkill = 0;
	sfx_play (4, 0);
	
	if (plife) plife --; else game_over = 1;

	#ifdef PLAYER_FLICKERS
		pstate = EST_PARP;
		pctstate = 100;	
	#else
		pstate = EST_REBOUND;
		pctstate = 16;	
	#endif

	#ifdef DIE_AND_RESPAWN
		px = px_safe;
		py = py_safe;
		n_pant = n_pant_safe;
		music_pause (1);
		delay (60);
		pvx = pvy = pj = 0;
		music_pause (0);
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

	hitv = hith = 0;
	pushed_any = 0;
	pnotsafe = 0;
	ppossee = 0;
	#ifdef ENABLE_SLIPPERY
		pice = 0;
	#endif

	// ********
	// Vertical
	// ********

	#ifdef PLAYER_TOP_DOWN		
		// Controller 

		if (!(i & PAD_UP || i & PAD_DOWN)) {
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
		
		if (i & PAD_UP) {
			pfacingv = CELL_FACING_UP;
			if (pvy > -PLAYER_VX_MAX) {
				pvy -= PLAYER_AX;
			}
		}
		
		if (i & PAD_DOWN) {
			pfacingv = CELL_FACING_DOWN;
			if (pvy < PLAYER_VX_MAX) {
				pvy += PLAYER_AX;
			}
		}

	#else
		// Gravity

		#ifndef PLAYER_SWIMS
			if (!pj) {
				pvy += PLAYER_G;
				if (pvy > PLAYER_VY_FALLING_MAX) pvy = PLAYER_VY_FALLING_MAX; 
			}
		#endif

		// Moving platforms invalidate pvy

		#ifdef PLAYER_CUMULATIVE_JUMP
			if (!pj)
		#endif
			if (pgotten) pvy = 0;			
	#endif

	cx1 = prx >> 4;
	cx2 = (prx + 7) >> 4;

	#ifdef PLAYER_HAS_JETPAC
	    // Controller 

		if (i & PAD_B) {
			pvy -= PLAYER_AY_JETPAC;
			if (pvy < -PLAYER_VY_JETPAC_MAX) pvy = -PLAYER_VY_JETPAC_MAX;
		}
	#endif

	#ifdef PLAYER_SWIMS
		// Controller 

		if (!(i & (PAD_DOWN|PAD_A))) {
			pvy -= PLAYER_AY_SWIM >> 1;
		} else {
			if (i & (PAD_DOWN|PAD_A)) {
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

	#endif

	// Move
	py += pvy;
	if (py < 0) py = 0;
	
	// Collision
	prx = px >> FIXBITS;
	pry = py >> FIXBITS;

	#ifdef PLAYER_TOP_DOWN		
		if (pvy < 0)
	#else
		rds16 = pvy + pgtmy;
		if (rds16 < 0)
	#endif		
		{
			cy1 = cy2 = (pry + PLAYER_COLLISION_TOP) >> 4;
			cm_two_points ();
			if ((at1 & 8) || (at2 & 8)) {
				pvy = 0; pry = ((cy1 + 1) << 4) - PLAYER_COLLISION_TOP; py = pry << FIXBITS;
				pgotten = 0;
				pfiring = 1;
				#ifdef PLAYER_TOP_DOWN
					// Special obstacles
					if (at1 & 2) player_process_tile (at1, cx1, cy1, cx1, cy1 - 1);
					if (at2 & 2) player_process_tile (at2, cx2, cy1, cx2, cy1 - 1);
				#endif
			} else if ((at1 & 1) || (at2 & 1)) {
				hitv = 1;
			}
	#ifdef ENABLE_QUICKSANDS
			else if ((at1 & 2) || (at2 & 2)) {
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
				pry + 4 < (cy1 << 4) &&
				((at1 & 12) || (at2 & 12))
			)
	 		#endif
			{
				pvy = 0; pry = ((cy1 - 1) << 4);py = pry << FIXBITS;
				pgotten = 0;
				pfiring = 1;
				ppossee = 1;
				
				#ifdef PLAYER_TOP_DOWN
					if (at1 & 2) player_process_tile (at1, cx1, cy1, cx1, cy1 + 1);
					if (at2 & 2) player_process_tile (at2, cx2, cy1, cx2, cy1 + 1);			
				#endif

				#ifdef ENABLE_SLIPPERY
					pice = (at1 & 64) || (at2 & 64);
				#endif

				#ifdef ENABLE_CONVEYORS
					if (at1 & 32) { if (at1 & 1) pgtmx = PLAYER_VX_CONVEYORS; else pgtmx = -PLAYER_VX_CONVEYORS; pgotten = 1; }
					if (at2 & 32) { if (at2 & 1) pgtmx = PLAYER_VX_CONVEYORS; else pgtmx = -PLAYER_VX_CONVEYORS; pgotten = 1; }
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
					if ((at1 & 2) || (at2 & 2)) {
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
		if (i & PAD_A) {
			if (!pjb) {
				pjb = 1;
				if (!pj) {
					if (pgotten || ppossee || hitv) {
						sfx_play (7, 0);
						pj = 1; pctj = 0; pvy = -PLAYER_VY_JUMP_INITIAL;
						
						#ifdef DIE_AND_RESPAWN
							if (!(pgotten || hitv || pnotsafe)) {
								player_register_safe_spot ();
							}
						#endif	
					}
				} 
			}
			if (pj) {
				if (pctj < PLAYER_AY_JUMP) pvy -= (PLAYER_AY_JUMP - (pctj));
				if (pvy < -PLAYER_VY_JUMP_MAX) pvy = -PLAYER_VY_JUMP_MAX;
				pctj ++; if (pctj == 16) pj = 0;	
			}
		} else {
			pj = 0; pjb = 0;
		}
	#endif

	// **********
	// Horizontal
	// **********
	
	// Poll pad
	if (!(i & PAD_LEFT || i & PAD_RIGHT)) {
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

	if (i & PAD_LEFT) {
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
	
	if (i & PAD_RIGHT) {
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
	else prx = px >> FIXBITS;
	
	// Collision

	cy1 = (pry + PLAYER_COLLISION_TOP) >> 4;
	cy2 = (pry + 15) >> 4;

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
		cm_two_points ();
		if ((at1 & 8) || (at2 & 8)) {
			pvx = 0; prx = rda; px = prx << FIXBITS; pfiring = 1;

			// Special obstacles
			if (at1 & 2) player_process_tile (at1, cx1, cy1, rdm, cy1);
			if (at2 & 2) player_process_tile (at2, cx1, cy2, rdm, cy2);
		} else {
			hith = ((at1 & 1) || (at2 & 1));
		}
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

	// *************
	// Killing tiles
	// *************
	
	phit = 0;
	if (hitv) { phit = 1; pvy = ADD_SIGN (-pvy, PLAYER_V_REBOUND); } 
	#ifndef NO_HORIZONTAL_EVIL_TILE	
		if (hith) { phit = 1; pvx = ADD_SIGN (-pvx, PLAYER_V_REBOUND); }
	#endif	
	if (pstate != EST_PARP) if (phit) pkill = 1;

	// **************
	// B Button stuff
	// **************

	// (fire bullets, run scripting w/animation, do containers)

	#if (defined (ACTIVATE_SCRIPTING) && defined (FIRE_SCRIPT_WITH_ANIMATION)) || defined (ENABLE_CONTAINERS) || defined (PLAYER_CAN_FIRE)
		if (b_button) {
			#ifdef PLAYER_CAN_FIRE				
				fire_bullet ();
			#endif		
			
			#ifdef ENABLE_CONTAINERS
				containers_do ();
			#endif
			
			#if defined (ACTIVATE_SCRIPTING) && defined (FIRE_SCRIPT_WITH_ANIMATION)
				if (ppossee) {
					pvx = pvy = 0;
					#ifdef ENABLE_CONTAINERS
						upd_cont_index = 0;
					#endif								
					use_ct = 1;
				}
			#endif
		} 
	#endif

	// **********
	// Calc frame
	// **********

	// You may (will) need to tinker with this for your game.

	#ifdef PLAYER_TOP_DOWN

		// Frame selection for top-down view games

		if (pvx != 0 || pvy != 0) {
			pctfr ++;
			if (pctfr == 4) {
				pctfr = 0;
				pfr = !pfr;
				psprid = pfacing + pfr;
			}
		}

	#else

		// Frame selection for side view games

		#ifdef PLAYER_SWIMS
			if (i && (rdx != prx || rdy != pry)) {
				if (pvx) {
					psprid = CELL_SWIM_CYCLE + ((prx >> 3) & 3);
				} else {
					psprid = CELL_SWIM_CYCLE + ((pry >> 3) & 3);
				}
			} else psprid = CELL_SWIM_CYCLE + 1;
		#else
			if (ppossee || pgotten) {

				// On floor

				if (pvx > PLAYER_VX_MIN || pvx < -PLAYER_VX_MIN) {
					psprid = CELL_WALK_CYCLE + ((prx >> 3) & 3);
				} else {
					psprid = CELL_IDLE;
				}
			} else {
				psprid = CELL_AIRBORNE;
				/*
				if (pvy < PLAYER_VY_FALLING_MIN)
					psprid = CELL_ASCENDING;
				else
					psprid = CELL_DESCENDING;	
				*/
			}
		#endif

		psprid += pfacing;
	#endif

	prx_old = prx;
	pry_old = pry;
}
