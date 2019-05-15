// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// general.h
// General functions, vars & buffers

void cm_two_points (void) {
	// Calculates at1 & at2 from cx1, cy1 & cx2, cy2
	if (cy1 > 12 || cy2 > 12) { at1 = at2 = 0; return; }
	at1 = MAP_ATTR (COORDS (cx1, cy1 ? cy1 - 1 : 0));
	at2 = MAP_ATTR (COORDS (cx2, cy2 ? cy2 - 1 : 0));
}

#if PLAYER_COLLISION_VSTRETCH_BG > 0
void cm_three_points (void) {
	// Always vertical, upon pry and pre-calculated cx1.
	cy1 = (pry - PLAYER_COLLISION_VSTRETCH_BG) >> 4;
	if (cy1 <= 12) at1 = MAP_ATTR (COORDS (cx1, cy1 ? cy1 - 1 : 0));
	cy2 = pry >> 4;
	if (cy2 <= 12) at2 = MAP_ATTR (COORDS (cx1, cy2 ? cy2 - 1 : 0));
	cy3 = (pry + 15) >> 4;
	if (cy3 <= 12) at3 = MAP_ATTR (COORDS (cx1, cy3 ? cy3 - 1 : 0));
}
#endif

unsigned char collide (void) {

	// Player bounding box is:
	//     prx to prx + 7
	//     pry - PLAYER_COLLISION_VSTRETCH_FG to pry + 13
	// Enemy bounding box is:
	//     _en_x + 4 to _en_x + 11
	//     _en_y - ENEMS_COLLISION_VSTRETCH_FG to _en_y + 13

	return (
		prx + 3 >= EN_X_ABSOLUTE && 
		prx <= EN_X_ABSOLUTE + 11 && 
		pry + 13 + ENEMS_COLLISION_VSTRETCH_FG >= _en_y &&
		pry <= _en_y + 13 + PLAYER_COLLISION_VSTRETCH_FG
	);

}

signed int add_sign (signed int sign, signed int value) {
	return sign == 0 ? 0 : sign < 0 ? -value : value;
}

#ifdef ACTIVATE_SCRIPTING
void run_fire_script (void) {
	fire_script_success = 0;
	run_script (2 * MAP_SIZE + 2);
	run_script ((n_pant << 1) + 1);
	#ifdef ENABLE_PUSHED_SCRIPT
		just_pushed = 0;
	#endif
	#ifdef ENABLE_INTERACTIVES
		just_interacted = 0;
	#endif
}
#endif

signed int saturate (signed int v, signed int max) {
	return v >= 0 ? (v > max ? max : v) : (v < -max ? -max : v);
}

void pad_read (void) {
	// Thanks for this, Nicole & nesdev!
	// https://forums.nesdev.com/viewtopic.php?p=179315#p179315
	pad_this_frame = pad0;
	pad0 = pad_poll (0);			// Read pads here.
	pad_this_frame = (pad_this_frame ^ pad0) & pad0;
}

#if defined (ENABLE_HOMING_FANTY) || defined (ENABLE_COCOS)
	// Lame but fast and tiny
	// Before the call: copy objects's coordinates into rdx, rdy
	unsigned char distance (void) {
		rda = DELTA (prx, rdx); // dx
		rdb = DELTA (pry, rdy); // dy
		rdc = MIN (rda, rdb);
		return (rda + rdb - (rdc >> 1) - (rdc >> 2) + (rdc >> 4));
	}
#endif

#ifndef PLAYER_TOP_DOWN
	void jump_start (void) {
		pj = 1; pctj = 0; 
		#ifdef ENABLE_TRAMPOLINES
		if (ptrampoline) {
			sfx_play (SFX_TRAMPOLINE, 0);
			#ifdef PLAYER_JUMP_TYPE_MK2
				pvy = -PLAYER_VY_MK2_JUMP_INITIAL_TRAMPOLINE;
			#else
				pvy = -PLAYER_VY_JUMP_INITIAL_TRAMPOLINE;
			#endif
		} else
		#endif
		{
			sfx_play (SFX_JUMP, 0);
			#ifdef PLAYER_JUMP_TYPE_MK2
				pvy = -PLAYER_VY_MK2_JUMP_INITIAL;
			#else
				pvy = -PLAYER_VY_JUMP_INITIAL;
			#endif
		}
	}
#endif

void update_cycle (void) {
	#if defined (DOUBLE_WIDTH) && defined (NO_SPLIT)
		scroll (scroll_x, SCROLL_Y);
	#endif
	oam_hide_rest (oam_index);
	#ifdef DEBUG
		ppu_mask (0x1e);
	#endif
	ppu_waitnmi ();
	
	clear_update_list ();
	oam_index = 4;
	#if defined (DOUBLE_WIDTH) && !defined (NO_SPLIT)
		split (scroll_x, SCROLL_Y);
	#endif

	#ifdef DEBUG
		//ppu_mask (0x1f);
	#endif
}

#ifdef DOUBLE_WIDTH
	void scroll_to (void) {
		// Fast scroll to rds16, assume multiple of 8
		scroll_x &= 0xfff8;	// Make multiple of 8
		while (scroll_x != rds16) {
			if (scroll_x < rds16) scroll_x += 8;
			else scroll_x -= 8;
			update_cycle ();
		}
	}

	void calc_scroll_pos (void) {
		#ifdef SINGLE_SCREEN_SUPPORT
			if (scr_single) { scroll_x = 0; return; } 
		#endif
			
		scroll_x = prx - 124;
		if (scroll_x < 0) scroll_x = 0;
		else if (scroll_x > 256) scroll_x = 256;	
	}

	void calc_en_x_absolute (void) {
		#if defined (ENABLE_FANTY) || defined (ENABLE_HOMING_FANTY) || defined (ENABLE_TIMED_FANTY) || defined (ENABLE_BOIOIONG)
			if (_en_t == 6 || _en_t == 13) {
				// Fanties and boioiongs are absolute
				EN_X_ABSOLUTE = _enf_x >> FIXBITS;
			} else 
		#endif
		#if defined (ENABLE_PRECALC_FANTY) || defined (ENABLE_PRECALC_HOMING_FANTY) || defined (ENABLE_PRECALC_TIMED_FANTY)
			if (_en_t == 6) {
				EN_X_ABSOLUTE = FANTY_ENX;
			} else
		#endif
		{
			// Other types are relative
			EN_X_ABSOLUTE = en_x_offs + _en_x;
		}
		on_screen = (EN_X_ABSOLUTE >= scroll_x && EN_X_ABSOLUTE < scroll_x + 240);
	}
#endif
