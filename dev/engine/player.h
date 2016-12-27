// NES MK1 v0.3
// Copyleft Mojon Twins 2013, 2015

// player.h
// Player movement & stuff

void __fastcall__ player_init (void) {
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
	pobjs = pkeys = 0;
	pgotten = 0;
#ifdef MAX_AMMO
#ifdef INITIAL_AMMO
	pammo = INITIAL_AMMO
#else
	pammo = MAX_AMMO;
#endif
#endif	
	pfiring = 0;
#ifdef PLAYER_CAN_FIRE
	for (i = 0; i < MAX_BULLETS; i++) bst [i] = 0;
#endif
	pstate = EST_NORMAL;
	pkilled = 0;

	px_safe = px;
	py_safe = py;
	n_pant_safe = n_pant;
}

void __fastcall__ kill_player (void) {
	plife --;
#ifdef PLAYER_FLICKERS
	pstate = EST_PARP;
	pctstate = 100;	
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
			no_on = 1;
			no_ct = 100;
			oam_meta_spr ((x0 << 4) + 8, (y0 << 4) - 13 + SPRITE_ADJUST, 160, spr_en_12);	
		}
	} 
#endif
}
#endif

#ifdef PLAYER_CAN_FIRE
void __fastcall__ fire_bullet (void) {
	// Creates a new bullet (if possible);
	for (gpit = 0; gpit < MAX_BULLETS; gpit ++) {
		if (bst [gpit] == 0) {
			bst [gpit] = 1;
#ifdef MAX_AMMO
			if (!pammo) return;
			pammo --;
#endif
#ifdef PLAYER_MOGGY_STYLE
			switch (pfacing) {
				case FACING_LEFT:
					bx [gpit] = prx - 4;
					bmx [gpit] = -PLAYER_BULLET_SPEED;
					by [gpit] = pry + PLAYER_BULLET_Y_OFFSET;
					bmy [gpit] = 0;
					break;	
				case FACING_RIGHT:
					bx [gpit] = prx + 12;
					bmx [gpit] = PLAYER_BULLET_SPEED;
					by [gpit] = pry + PLAYER_BULLET_Y_OFFSET;
					bmy [gpit] = 0;
					break;
				case FACING_DOWN:
					bx [gpit] = prx + PLAYER_BULLET_X_OFFSET;
					by [gpit] = pry + 12;
					bmy [gpit] = PLAYER_BULLET_SPEED;
					bmx [gpit] = 0;
					break;
				case FACING_UP:
					bx [gpit] = prx + 8 - PLAYER_BULLET_X_OFFSET;
					by [gpit] = pry - 4;
					bmy [gpit] = -PLAYER_BULLET_SPEED;
					bmx [gpit] = 0;
					break;
			}
#endif		
			// sound
			sfx_play (5, 0);
			// done
			break;
		}
	}
}

void __fastcall__ bullets_move (void) {
	for (gpit = 0; gpit < MAX_BULLETS; gpit ++) {
		if (bst [gpit]) {
			if (bmx [gpit] != 0) {
				bx [gpit] += bmx [gpit];
				if (
					bx [gpit] < PLAYER_BULLET_SPEED ||
					bx [gpit] > 255 - PLAYER_BULLET_SPEED
				) {
					bst [gpit] = 0; 
				}
			}
			if (bmy [gpit] != 0) {
				by [gpit] += bmy [gpit];
				if (
					by [gpit] < PLAYER_BULLET_SPEED ||
					by [gpit] > 191 - PLAYER_BULLET_SPEED
				) {
					bst [gpit] = 0; 
				}
			}
			
			btx = (bx [gpit] + 3) >> 4; bty = (by [gpit] + 3) >> 4;

#ifdef BREAKABLE_WALLS
			// Break wall
			if (attr (btx, bty) & 16) {
				break_wall (btx, bty);
				bst [gpit] = 0;
				sfx_play (6, 2);
			}
#endif			
			
			if (attr (btx, bty) > 7) {
				bst [gpit] = 0;
				sfx_play (6, 2);
			}
			
			// Collide with enemies?			
			for (gpjt = 0; gpjt < 3; gpjt ++) {
#ifdef FIRE_MIN_KILLABLE
				if (en_t [gpjt] >= FIRE_MIN_KILLABLE) {
#else
				if (en_t [gpjt]) {
#endif
#ifdef ENABLE_PURSUERS
					if (en_t [gpjt] != 7 || en_alive [gpjt] == 2)
#endif
					if (collide_in (bx [gpit] + 3, by [gpit] + 3, en_x [gpjt], en_y [gpjt])) {
						en_touched [gpjt] = 1;
						en_cttouched [gpjt] = 8;
						bst [gpit] = 0;
						sfx_play (6, 2);
						en_life [gpjt] --;						
						if (en_life [gpjt] == 0) {	
#ifdef ENABLE_PURSUERS
							if (en_t [gpjt] == 7) {
								en_alive [gpjt] = 0;
								en_ct [gpjt] = DEATH_COUNT_EXPRESSION;
								en_life [gpjt] = ENEMIES_LIFE_GAUGE;
							} else
#endif
							en_t [gpjt] = 0;		
							pkilled ++;												
						}
						break;
					}
				}	
			}
						
			oam_spr (bx [gpit], SPRITE_ADJUST + by [gpit], 152, 1, 112 + (gpit << 2));
		}
		if (!bst [gpit]) {			
			oam_spr (0, 240, 132, 1, 152 + (gpit << 2));
		}
	}
}
#endif

#ifdef PLAYER_MOGGY_STYLE
const unsigned char player_frames [] = {0, 1, 2, 3, 4, 5, 6, 7};
#else
const unsigned char player_frames [] = {
	4, 5, 6, 5, 
	0, 1, 2, 1,
	7, 3
};
#endif

void __fastcall__ player_move (void) {
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
		pfacingv = FACING_UP;
		if (pvy > -PLAYER_VX_MAX) {
			pvy -= PLAYER_AX;
		}
	}
	
	if (i & PAD_DOWN) {
		pfacingv = FACING_DOWN;
		if (pvy < PLAYER_VX_MAX) {
			pvy += PLAYER_AX;
		}
	}
#else
#ifdef ENABLE_PROPELLERS
	if (ppropelled) {
		pvy -= PROPELLER_AY;
		if (pvy < -PROPELLER_VY_MAX) pvy = -PROPELLER_VY_MAX;
	} else
#endif
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

	// **********
	// Horizontal
	// **********
	
#ifdef ENABLE_CONVEYORS	
	// Conveyors
	if (ppossee) {
		pry = py >> 6;
		ptx1 = (prx + 4) >> 4;
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
		ptx1 = (prx + 11) >> 4;
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
		pfacingh = 0xff;
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
		if (pvx > PLAYER_VX_MAX) pvx = PLAYER_VX_MAX;
	}
	
	if (i & PAD_LEFT) {
		pfacingh = FACING_LEFT;
		if (psprint) {
			if (pvx > -PLAYER_VX_SPRINT_MAX) {
				pvx -= PLAYER_AX_SPRINT;
			}
		} else if (pvx > -PLAYER_VX_MAX) {
			pvx -= PLAYER_AX;			
		}
#ifndef PLAYER_MOGGY_STYLE			
		pfacing = 8;
#endif		
	}
	
	if (i & PAD_RIGHT) {
		pfacingh = FACING_RIGHT;
		if (psprint) {
			if (pvx < PLAYER_VX_SPRINT_MAX) {
				pvx += PLAYER_AX_SPRINT;
			}
		} else if (pvx < PLAYER_VX_MAX) {
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
	if (pvx != 0) hitv = (attr (ptx1, pty1) & 1) || (attr (ptx1, pty2) & 1);
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
	
	if (hitv) {
		phit = 1;
#ifdef FULL_BOUNCE
		pvy = add_sign (-pvy, PLAYER_VX_MAX);
#endif
#ifdef DOUBLE_BOUNCE
		pvy = add_sign (-pvy, PLAYER_VX_MAX << 1);
#endif		
	} else if (hith) {
		phit = 1;
#ifdef FULL_BOUNCE
		pvx = add_sign (-pvx, PLAYER_VX_MAX);
#endif
#ifdef DOUBLE_BOUNCE
		pvx = add_sign (-pvx, PLAYER_VX_MAX << 1);
#endif		
	}
	if (pstate != EST_PARP) {
		if (phit) {
			kill_player ();
		}
	}

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

	// ************
	// Fire bullets
	// ************
	
#ifdef PLAYER_CAN_FIRE
#ifdef FIRE_TO_PUSH
	if ((i & PAD_A) && !pfiring && !pushed_any) {
#else
	if ((i & PAD_A) && !pfiring) {
#endif
		pfiring = 1;
		fire_bullet ();
	}
	
	if (!(i & PAD_A)) pfiring = 0;
#endif	
	
	// Calc frame
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
#else
	// Extended 16, v.2.0
	// facing = 0 || 8
	// frame = 0 standing, 1 2 3 4 running, 5 jumping, 6 7 falling
	if (ppossee || pgotten) {
		if (pvx > PLAYER_VX_MIN || pvx < -PLAYER_VX_MIN) {
			psprid = pfacing + 1 + ((prx >> 3) & 3);
			/*
			pctfr ++;
			if (pctfr == 4) {
				pctfr = 0;
				pfr = (pfr + 1) & 3;
				psprid = pfacing + 1 + pfr;
			}
			*/
		} else {
			psprid = pfacing;
		}
	} else {
		if (pvy >= PLAYER_VY_FALLING_MAX >> 1) psprid = pfacing + 6 + ((pry >> 4) & 1);
		psprid = pfacing + 5;
	}
#endif
}

void __fastcall__ render_player (void) {
	if (pstate == EST_NORMAL || half_life) {
		oam_meta_spr (prx, pry + SPRITE_ADJUST, 128, spr_player [psprid]);	
	} else {
		oam_meta_spr (0, 240, 128, spr_pl_empty);
	}
}
