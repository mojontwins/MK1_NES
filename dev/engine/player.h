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
	pfacing = FACING_DOWN;
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
#ifdef MAX_AMMO
#ifdef INITIAL_AMMO
	pammo = INITIAL_AMMO
#else
	pammo = MAX_AMMO;
#endif
#endif	
	pfiring = 0;
#ifdef PLAYER_GARGAJO
	pgargajocounter = 0;
#endif	
#ifdef PLAYER_CAN_FIRE
	for (i = 0; i < MAX_BULLETS; i++) bst [i] = 0;
#endif
	pstate = EST_NORMAL;
#ifdef PLAYER_CAN_FIRE
	pkilled = 0;
#endif

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
		oam_meta_spr (prx, pry + SPRITE_ADJUST, OAM_PLAYER, spr_player [psprid]);	
	} else {
		oam_meta_spr (0, 240, OAM_PLAYER, spr_pl_empty);
	}
}

void kill_player (void) {
	render_player ();
	
	plife --;
#ifdef PLAYER_FLICKERS
	pstate = EST_PARP;
	pctstate = 100;	
#else
	pstate = EST_REBOUND;
	pctstate = 16;	
#endif
	sfx_play (4, 0);
#ifdef DIE_AND_RESPAWN
	px = px_safe;
	py = py_safe;
	n_pant = n_pant_safe;
#endif	

#ifdef DIE_AND_RESPAWN
	music_pause (1);
	gpit = 60;
	while (gpit --) {
		ppu_waitnmi ();
	}
	/*px_safe = px;
	py_safe = py;
	n_pant_safe = n_pant;
	*/
	pvx = pvy = pj = 0;
	music_pause (0);
#endif	
}

#if defined(PLAYER_PUSH_BOXES) || !defined(DEACTIVATE_KEYS)
void process_tile (x0, y0, x1, y1) {
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
#ifndef DEACTIVATE_KEYS
	if (qtile (x0, y0) == 15) {
		if (pkeys) {
			// Update screen
			map_set (x0, y0, 0);
			// Clear lock in RAM
			clear_cerrojo ((x0 << 4) + (y0 & 15));
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
		//pfacingv = 0xff;
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
		//pfacingv = FACING_UP;
		if (pvy > -PLAYER_VX_MAX) {
		//if (pvy > -player_vx_max) {
			pvy -= PLAYER_AX;
		}
	}
	
	if (i & PAD_DOWN) {
		//pfacingv = FACING_DOWN;
		if (pvy < PLAYER_VX_MAX) {
		//if (pvy < player_vx_max) {
			pvy += PLAYER_AX;
		}
	}
#else
	// gravity
	if (!pj) {
		if (pvy < PLAYER_VY_FALLING_MAX) { 
			pvy += PLAYER_G;
		} else {
			pvy = PLAYER_VY_FALLING_MAX;
		}
	}

#ifdef PLAYER_CUMULATIVE_JUMP
	if (!pj)
#endif
		if (pgotten) pvy = 0;			
#endif

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
	
	ptx1 = (prx + 4) >> 4;
	ptx2 = (prx + 11) >> 4;
#ifdef PLAYER_MOGGY_STYLE		
	if (pvy < 0)
#else
	if (pvy + pgtmy < 0)
#endif		
	{
		pty1 = (pry + 8) >> 4;
		if ((attr (ptx1, pty1) & 8) || (attr (ptx2, pty1) & 8)) {
			pvy = 0;
			pry = ((pty1 + 1) << 4) - 8;
			py = pry << 6;
			wall = WTOP;
			pgotten = 0;
		} 
#ifdef PLAYER_MOGGY_STYLE			
	} else if (pvy > 0)
#else
	} else if (pvy + pgtmy > 0)
#endif		
	{
		pty1 = (pry + 15) >> 4;
#ifndef PLAYER_MOGGY_STYLE
 		if (((pry - 1) & 15) < 8)		
#endif		
		if ((attr (ptx1, pty1) & 12) || (attr (ptx2, pty1) & 12)) {
			pvy = 0;
			pry = ((pty1 - 1) << 4);
			py = pry << 6;
			wall = WBOTTOM;
			pgotten = 0;
		}
	}
	if (pvy != 0) hitv = (attr (ptx1, pty1) & 1) || (attr (ptx2, pty1) & 1);
	
	pty1 = (pry + 16) >> 4;
	ppossee = (attr (ptx1, pty1) & 12 || attr (ptx2, pty1) & 12);

#ifdef PLAYER_HAS_JUMP
	// *******************************
	// Jump: PAD_A, change when needed
	// *******************************
	if (i & PAD_A) {
		if (!pjb) {
			pjb = 1;
			if (!pj) {
				if (pgotten || ppossee || hitv) {
					pj = 1; 
					pctj = 0;
					sfx_play (7, 0);
					pvy = -PLAYER_VY_JUMP_INITIAL;
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

#ifdef PLAYER_BOOTEE
	if (!pj) {
		if (pgotten || ppossee || hitv) {
			pj = 1; 
			pctj = 0;
			sfx_play (7, 0);
			pvy = -PLAYER_VY_JUMP_INITIAL;
#ifdef DIE_AND_RESPAWN
			if (!(pgotten || hitv)) {
				px_safe = px;
				py_safe = py;
				n_pant_safe = n_pant;
			}
#endif	
		}
	} 	

	if (pj) {
		if (pctj < PLAYER_AY_JUMP) pvy -= (PLAYER_AY_JUMP - (pctj));
		if (pvy < -PLAYER_VY_JUMP_MAX) pvy = -PLAYER_VY_JUMP_MAX;
		pctj ++; if (pctj == 16) pj = 0;	
	}

	if (i & PAD_DOWN) {
		if (pvy < 0) pvy += PLAYER_AY_UNTHRUST;
	}
#endif

	// **********
	// Horizontal
	// **********
	
#ifdef ENABLE_CONVEYORS	
	// Conveyors
	if (ppossee) {
		pry = py >> 6;
		ptx1 = (prx + 8) >> 4;
		pty1 = (pry + 16) >> 4;
		gpit = attr (ptx1, pty1);
		if (gpit & 32) {
			pgotten = 1; 
			pgtmy = 0;
			if (gpit & 1) {
				pgtmx = 64;
			} else {
				pgtmx = -64;
			}
		}
	}
#endif	
	
	// Poll pad
	if (!(i & PAD_LEFT || i & PAD_RIGHT)) {
#ifdef PLAYER_MOGGY_STYLE		
		pfacingh = 0xff;
#endif
		if (pvx > 0) {
			pvx -= PLAYER_RX;
			if (pvx < 0)
				pvx = 0;
		} else if (pvx < 0) {
			pvx += PLAYER_RX;
			if (pvx > 0)
				pvx = 0;
		}
	}

	psprint = ((i & PAD_B) && ppossee);
	if (!psprint && ppossee) {
		if (pvx < -PLAYER_VX_MAX) pvx = -PLAYER_VX_MAX;
		//if (pvx < -player_vx_max) pvx = -player_vx_max;
		if (pvx > PLAYER_VX_MAX) pvx = PLAYER_VX_MAX;
		//if (pvx < -player_vx_max) pvx = -player_vx_max;
	}
	
	if (i & PAD_LEFT) {
#ifdef PLAYER_MOGGY_STYLE		
		pfacingh = FACING_LEFT;
#endif
		if (psprint) {
			if (pvx > -PLAYER_VX_SPRINT_MAX) {
				pvx -= PLAYER_AX_SPRINT;
			}
		} else if (pvx > -PLAYER_VX_MAX) {
		//} else if (pvx > -player_vx_max) {
			pvx -= PLAYER_AX;			
		}
#ifndef PLAYER_MOGGY_STYLE			
		pfacing = 16;
#endif		
	}
	
	if (i & PAD_RIGHT) {
#ifdef PLAYER_MOGGY_STYLE		
		pfacingh = FACING_RIGHT;
#endif		
		if (psprint) {
			if (pvx < PLAYER_VX_SPRINT_MAX) {
				pvx += PLAYER_AX_SPRINT;
			}
		} else if (pvx < PLAYER_VX_MAX) {
		//} else if (pvx < player_vx_max) {
			pvx += PLAYER_AX;
		}
#ifndef PLAYER_MOGGY_STYLE			
		pfacing = 0;
#endif			
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
	//pry = py >> 6;

#ifdef BOUNDING_BOX_8_BOTTOM
	pty1 = (pry + 8) >> 4;
	pty2 = (pry + 15) >> 4;
#ifdef PLAYER_MOGGY_STYLE	
	if (pvx < 0) {
#else
	if (pvx + pgtmx < 0) {
#endif
		ptx1 = (prx + 4) >> 4;
		if ((attr (ptx1, pty1) & 8) || (attr (ptx1, pty2) & 8)) {
			pvx = 0;
			prx = ((ptx1 + 1) << 4) - 4;
			px = prx << 6;
			wall = WLEFT;
		}
#ifdef PLAYER_MOGGY_STYLE	
	} else if (pvx > 0) {
#else
	} else if (pvx + pgtmx > 0) {
#endif		
		ptx1 = (prx + 11) >> 4;
		if ((attr (ptx1, pty1) & 8) || (attr (ptx1, pty2) & 8)) {
			pvx = 0;
			prx = ((ptx1 - 1) << 4) + 4;
			px = prx << 6;
			wall = WRIGHT;
		}
	}
#ifndef NO_HORIZONTAL_EVIL_TILE		
	if (pvx != 0) hith = (attr (ptx1, pty1) & 1) || (attr (ptx1, pty2) & 1);
#endif	
#endif	

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
		if (attr (ptx1, pty1) == 10) process_tile (ptx1, pty1, ptx1, pty1 - 1);
	} else if (wall == WBOTTOM) {
		// interact down
#ifdef BOUNDING_BOX_8_BOTTOM
		pty1 = (pry + 16) >> 4;
#endif		
		if (attr (ptx1, pty1) == 10) process_tile (ptx1, pty1, ptx1, pty1 + 1);
	} else
#endif	
	if (wall == WLEFT) {		
		// interact left
#ifdef BOUNDING_BOX_8_BOTTOM
		ptx1 = (prx + 3) >> 4;
#endif		
		if (attr (ptx1, pty1) == 10) process_tile (ptx1, pty1, ptx1 - 1, pty1);
	} else if (wall == WRIGHT) {
		// interact right
#ifdef BOUNDING_BOX_8_BOTTOM
		ptx1 = (prx + 12) >> 4;
#endif		
		if (attr (ptx1, pty1) == 10) process_tile (ptx1, pty1, ptx1 + 1, pty1);
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
					upd_cont_index = 0;
					use_ct = 1;
				}
#endif
			}	
		}
	} else pfiring = 0;
#endif

//


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

	// Basic, v.1.0
#ifdef PLAYER_MOGGY_STYLE
	if (pvx != 0 || pvy != 0) {
		pctfr ++;
		if (pctfr == 4) {
			pctfr = 0;
			pfr = !pfr;
			psprid = player_frames [pfacing + pfr];
		}
	}
#elif defined (PLAYER_BOOTEE)
#ifdef PLAYER_BOUNCES	
	if (pstate == EST_REBOUND) {
		psprid = 8;
	} else 
#endif	
	{
#ifdef PLAYER_GARGAJO
		if (pfiring) {
			psprid = 6;
		} else if (pgargajocounter) {
			psprid = 7;
			pgargajocounter --;
		} else
#endif
		{
			if (i & PAD_DOWN) 
				psprid = 5;
			else if (pvy < PLAYER_VY_AN1)
				psprid = 2;
			else if (pvy < PLAYER_VY_AN2)
				psprid = 1;
			else if (pvy < PLAYER_VY_AN3)
				psprid = 0;
			else if (pvy < PLAYER_VY_AN4)
				psprid = 3;
			else
				psprid = 4;
		}
	}

	psprid += pfacing;
	// TODO: Extend
#else
	// Extended 16, v.2.0
	// facing = 0 || 8
	// frame = 0 standing, 1 2 3 4 running, 5 jumping, 6 7 falling
	if (ppossee || pgotten) {
		if (pvx > PLAYER_VX_MIN || pvx < -PLAYER_VX_MIN) {
			psprid = pfacing + 1 + ((prx >> 3) & 3);
		} else {
			psprid = pfacing;
		}
	} else {
		if (pvy >= PLAYER_VY_FALLING_MAX >> 1) psprid = pfacing + 6 + ((pry >> 4) & 1);
		psprid = pfacing + 15;
	}
#endif
}
