// NES MK1 v0.7
// Copyleft Mojon Twins 2013, 2015, 2016

// player.h
// Player movement & stuff

void player_init (void) {
	// Init player data
	
	px = (signed int) (PLAYER_INI_X << 4) << 6;
	py = (signed int) (PLAYER_INI_Y << 4) << 6;
	
	pvx = pvy = 0;

	#ifdef PLAYER_MOGGY_STYLE	
		pfacing = CELL_FACING_DOWN;
	#else
		pfacing = 0;
	#endif	

	pfr = pctfr = 0;
	pj = pctj = 0;
	psprid = 6;

	#ifndef DEACTIVATE_OBJECTS
		pobjs = 0;
	#endif

	#ifndef DEACTIVATE_KEYS
		pkeys = 0;
	#endif

	pgotten = 0;
	pfiring = 0;

	#ifdef PLAYER_GARGAJO
		pgargajocounter = 0;
	#endif	

	#ifdef PLAYER_CAN_FIRE
		gpit = MAX_BULLETS; while (gpit --) bst [gpit] = 0;
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
		px_safe = px;
		py_safe = py;
		n_pant_safe = n_pant;
	#endif

	#ifdef CARRY_ONE_HS_OBJ
		pinv = HS_INV_EMPTY; 
	#endif

	#ifdef CARRY_ONE_FLAG_OBJ
		flags [HS_INV_FLAG] = HS_INV_EMPTY;
	#endif

	#ifdef ENABLE_CONTAINERS
		upd_cont_index = 0;
	#endif
}

void render_player (void) {
	if (pstate == EST_NORMAL || half_life) {
		oam_meta_spr (prx, pry + SPRITE_ADJUST, 4, spr_player [psprid]);	
	} else {
		oam_meta_spr (0, 240, 4, spr_pl_empty);
	}
}

void kill_player (void) {
	render_player ();
	sfx_play (4, 0);
	
	plife --;

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
}

#if defined(PLAYER_PUSH_BOXES) || !defined(DEACTIVATE_KEYS)
	void player_process_tile (x0, y0, x1, y1) {
	
		// Boxes

		#ifdef PLAYER_PUSH_BOXES
			#ifdef FIRE_TO_PUSH
				if (i & PAD_A) {
					pfiring = 1;
			#endif
					if (x0 > 0 && x0 < 15 && y0 > 0 && y0 < 11) {
						if (qtile (x0, y0) == 14 && attr (x1, y1) == 0) {
							pushed_any = 1;
							// Move & paint
							map_set (x0, y0, 0);
							map_set (x1, y1, 14);			
							// Sound
							sfx_play (1, 1);
						}
					}
			#ifdef FIRE_TO_PUSH
				}
			#endif	
		#else
			y1=x1;//Shutup, compiler!
		#endif

			// Bolts

		#ifndef DEACTIVATE_KEYS
			if (qtile (x0, y0) == 15) {
				if (pkeys) {
					// Update screen
					map_set (x0, y0, 0);
					// Clear lock in RAM
					clear_cerrojo ((y0 << 4) | x0);
					// Decrease keys 
					pkeys --;
					// Sound
					sfx_play (1, 1);
				} else {
					no_ct = 100;
				}
			} 
		#endif

	}
#endif

#ifdef PLAYER_CAN_FIRE
	#include "engine/playermods/bullets.h"
#endif

#ifdef PLAYER_GARGAJO
	#include "engine/playermods/gargajo.h"
#endif

#ifndef PLAYER_MOGGY_STYLE
	void player_points_beneath (void) {
		cy1 = cy2 = (pry + 16) >> 4;
		cm_two_points ();
	}
#endif

void player_move (void) {
	i = pad_poll (0);
	wall = 0;
	hitv = hith = 0;
	pushed_any = 0;
	//ppossee = 0;

	// ********
	// Vertical
	// ********

	#ifdef PLAYER_MOGGY_STYLE		
		// Poll pad
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
			if (pvy > -player_vx_max) {
				pvy -= PLAYER_AX;
			}
		}
		
		if (i & PAD_DOWN) {
			pfacingv = CELL_FACING_DOWN;
			if (pvy < PLAYER_VX_MAX) {
			if (pvy < player_vx_max) {
				pvy += PLAYER_AX;
			}
		}
	#else
		// gravity

		if (!pj) {
			pvy += PLAYER_G;
			if (pvy > PLAYER_VY_FALLING_MAX) pvy = PLAYER_VY_FALLING_MAX; 
		}

		#ifdef PLAYER_CUMULATIVE_JUMP
			if (!pj)
		#endif
			if (pgotten) pvy = 0;			
	#endif

	cx1 = prx >> 4;
	cx2 = (prx + 7) >> 4;

	#ifdef PLAYER_HAS_JETPAC
	    // **********************************
		// thrust! PAD_B, change when needed.
		// **********************************
		if (i & PAD_B) {
			pvy -= PLAYER_AY_JETPAC;
			if (pvy < -PLAYER_VY_JETPAC_MAX) pvy = -PLAYER_VY_JETPAC_MAX;
		}
	#endif

	// Move
	py += pvy;
	if (py < 0) py = 0;
	
	// Collision
	prx = px >> 6;
	pry = py >> 6;
		
	#ifdef PLAYER_MOGGY_STYLE		
		if (pvy < 0)
	#else
		rds16 = pvy + pgtmy;
		if (rds16 < 0)
	#endif		
		{
			cy1 = cy2 = (pry + 8) >> 4;
			cm_two_points ();
			if ((at1 & 8) || (at2 & 8)) {
				pvy = 0; pry = ((cy1 + 1) << 4) - 8; py = pry << 6;
				pgotten = 0;
				wall = WTOP;
			} else if ((at1 & 1) || (at2 & 1)) {
				hitv = 1;
			}
	#ifdef ENABLE_QUICKSANDS
			else if ((at1 & 2) || (at2 & 2)) {
				if (pctj > 2) pj = 0;
			}
	#endif		

	#ifdef PLAYER_MOGGY_STYLE			
		} else if (pvy > 0)
	#else
		} else if (rds16 > 0)
	#endif		
		{
			cy1 = cy2 = (pry + 15) >> 4; 
			cm_two_points (); 
	 		if (((pry - 1) & 15) < 8 && ((at1 & 12) || (at2 & 12))) {
				pvy = 0; pry = ((cy1 - 1) << 4);py = pry << 6;
				pgotten = 0;
				wall = WBOTTOM;
			} else if ((at1 & 1) || (at2 & 1)) {
				hitv = 1;
			}
	#ifdef ENABLE_QUICKSANDS		
			else {
				if ((at1 & 2) || (at2 & 2)) pvy = QUICKSANDS_SINK_VY;
			}
	#endif
		}
	
	player_points_beneath ();
	ppossee = ((at1 & 14) || (at2 & 14));
	#ifdef ENABLE_SLIPPERY
		pice = (ppossee && ((at1 & 64) || (at2 & 64)));
	#endif	

	// Conveyors
	#ifdef ENABLE_CONVEYORS	
		if (ppossee) {
			pconvd1 = at1 & 1; pconvd2 = at2 & 1; 
			if (at1 & 32) { if (pconvd1) pgtmx = 64; else pgtmx = -64; pgotten = 1; }
			if (at2 & 32) { if (pconvd2) pgtmx = 64; else pgtmx = -64; pgotten = 1; }
		}
	#endif

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
							if (!(pgotten || hitv)) {
								px_safe = px;
								py_safe = py;
								n_pant_safe = n_pant;
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
		#ifdef PLAYER_MOGGY_STYLE		
			pfacingh = 0xff;
		#endif
		
		if (pvx > 0) {

			#ifdef ENABLE_SLIPPERY
				pvx -= pice ? ICE_RX : PLAYER_RX;
			#else			
				pvx -= PLAYER_RX;
			#endif			
			
			if (pvx < 0) pvx = 0;
		} else if (pvx < 0) {

			#ifdef ENABLE_SLIPPERY
				pvx += pice ? ICE_RX : PLAYER_RX;
			#else
				pvx += PLAYER_RX;
			#endif

			if (pvx > 0) pvx = 0;
		}
	}

	if (i & PAD_LEFT) {
		#ifdef PLAYER_MOGGY_STYLE		
			pfacingh = CELL_FACING_LEFT;
		#else
			pfacing = CELL_FACING_LEFT;		
		#endif
		
		if (pvx > -PLAYER_VX_MAX) {
			
			#ifdef ENABLE_SLIPPERY
				pvx -= pice ? ICE_AX : PLAYER_AX;
			#else
				pvx -= PLAYER_AX;
			#endif
		}
	}
	
	if (i & PAD_RIGHT) {
		#ifdef PLAYER_MOGGY_STYLE		
			pfacingh = CELL_FACING_RIGHT;
		#else
			pfacing = CELL_FACING_RIGHT;
		#endif		

		if (pvx < PLAYER_VX_MAX) {
		
			#ifdef ENABLE_SLIPPERY
				pvx += pice ? ICE_AX : PLAYER_AX;
			#else
				pvx += PLAYER_AX;
			#endif
		}
	}
	
	// Move
	px += pvx;
	if (px < 0) px = 0;
	if (px > 15360) px = 15360;

	#ifndef PLAYER_MOGGY_STYLE	
		if (pgotten) px += pgtmx;
	#endif
	
	// Collision
	prx = px >> 6;

	cy1 = (pry + 8) >> 4;
	cy2 = (pry + 15) >> 4;

	rds16 = pvx + pgtmx;
	if (rds16) 	{
		if (rds16 < 0) {
			cx1 = cx2 = prx >> 4;
			rda = (cx1 + 1) << 4;
			wall = WLEFT;
		} else {
			cx1 = cx2 = (prx + 7) >> 4;
			rda = ((cx1 - 1) << 4) + 8;
			wall = WRIGHT;
		}
		cm_two_points ();
		if ((at1 & 8) || (at2 & 8)) {
			pvx = 0; prx = rda; px = prx << 6;
		} else {
			wall = 0;
			hith = ((at1 & 1) || (at2 & 1));
		}
	}

	// Facing
	#ifdef PLAYER_MOGGY_STYLE	
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

	prx = px >> 6;
	pry = py >> 6;
	
	// *************
	// Killing tiles
	// *************
	
	phit = 0;
	if (hitv) { phit = 1; pvy = add_sign (-pvy, PLAYER_V_REBOUND); } 
	#ifndef NO_HORIZONTAL_EVIL_TILE	
		if (hith) { phit = 1; pvx = add_sign (-pvx, PLAYER_V_REBOUND); }
	#endif	
	if (pstate != EST_PARP) if (phit) kill_player ();

	// ************************************************
	// Tile type 10 operations (push boxes, open locks)
	// ************************************************
	
	#if defined(PLAYER_PUSH_BOXES) || !defined(DEACTIVATE_KEYS)
		ptx1 = (prx + 8) >> 4;
		pty1 = (pry + 8) >> 4;
		
		#ifdef PLAYER_MOGGY_STYLE
			if (wall == WTOP) {
				// interact up			

				#ifdef BOUNDING_BOX_8_BOTTOM
					pty1 = (pry + 7) >> 4;
				#endif

				if (attr (ptx1, pty1) == 10) player_process_tile (ptx1, pty1, ptx1, pty1 - 1);
			} else if (wall == WBOTTOM) {
				// interact down

				#ifdef BOUNDING_BOX_8_BOTTOM
					pty1 = (pry + 16) >> 4;
				#endif		
				
				if (attr (ptx1, pty1) == 10) player_process_tile (ptx1, pty1, ptx1, pty1 + 1);
			} else
		#endif	
		
		if (wall == WLEFT) {		
			// interact left
			
			#ifdef BOUNDING_BOX_8_BOTTOM
				ptx1 = (prx + 3) >> 4;
			#endif		
			
			if (attr (ptx1, pty1) == 10) player_process_tile (ptx1, pty1, ptx1 - 1, pty1);
		} else if (wall == WRIGHT) {
			// interact right
	
			#ifdef BOUNDING_BOX_8_BOTTOM
				ptx1 = (prx + 12) >> 4;
			#endif		
			
			if (attr (ptx1, pty1) == 10) player_process_tile (ptx1, pty1, ptx1 + 1, pty1);
		}
	#endif	

	// **************
	// B Button stuff
	// **************

	// (fire bullets, run scripting w/animation, do containers)

	#if (defined (ACTIVATE_SCRIPTING) && defined (FIRE_SCRIPT_WITH_ANIMATION)) || defined (ENABLE_CONTAINERS) || defined (PLAYER_CAN_FIRE)
		if (i & PAD_B) {
			if (!pfiring) {
				pfiring = 1;
				#ifdef FIRE_TO_PUSH
					if (!pushed_any)
				#endif
				{
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
			}
		} else pfiring = 0;
	#endif

	// *************
	// Shoot Gargajo
	// *************

	#ifdef PLAYER_GARGAJO

		if ((i & PAD_B) && !pgargajocounter) {
			if (pfiring < GARGAJO_L_MAX) {
				gp_addr = 0x2004 + (LINE_OF_TEXT << 5) + (pfiring >> 1);
				update_list [update_index ++] = MSB (gp_addr);
				update_list [update_index ++] = LSB (gp_addr ++);
				update_list [update_index ++] = 9;
				pfiring ++;
			}
		}

		if (!(i & PAD_B)) {
			// Have we just stopped pressing fire?
			if (pfiring) {
				// Shoot new gargajo
				if (pfiring < GARGAJO_L_MIN) pfiring = GARGAJO_L_MIN;
				fire_gargajo (pfiring);
				pfiring = 0;
				pgargajocounter = GARGAJO_RELOAD;
				clean_gauge ();
			}
		}
	#endif

	// **********
	// Calc frame
	// **********

	// You may (will) need to tinker with this for your game.

	#ifdef PLAYER_MOGGY_STYLE

		// Frame selection for top-down view games

		if (pvx != 0 || pvy != 0) {
			pctfr ++;
			if (pctfr == 4) {
				pctfr = 0;
				pfr = !pfr;
				psprid = player_frames [pfacing + pfr];
			}
		}

	#else

		// Frame selection for side view games

		if (ppossee || pgotten) {

			// On floor

			if (pvx > PLAYER_VX_MIN || pvx < -PLAYER_VX_MIN) {
				psprid = CELL_WALK_CYCLE + ((prx >> 3) & 3);
			} else {
				psprid = CELL_IDLE;
			}
		} else {
			psprid = CELL_AIRBORNE;
		}

		psprid += pfacing;
	#endif
}
