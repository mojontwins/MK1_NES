// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// general.h
// General functions, vars & buffers

void cm_two_points (void) {
	// Calculates at1 & at2 from cx1, cy1 & cx2, cy2
	if (cy1 > 12 || cy2 > 12) { at1 = at2 = 0; return; }
	caux = cy1 ? cy1 - 1 : 0;
	at1 = map_attr [COORDS (cx1, caux)];
	caux = cy2 ? cy2 - 1 : 0;
	at2 = map_attr [COORDS (cx2, caux)];
}

#ifdef TALL_PLAYER
void cm_three_points (void) {
	// Always vertical, upon pry and pre-calculated cx1.
	cy1 = (pry + (PLAYER_COLLISION_TOP - 16)) >> 4;
	if (cy1 <= 12) {
		caux = cy1 ? cy1 - 1 : 0;
		at1 = map_attr [COORDS (cx1, caux)];
	}
	cy1 = pry >> 4;
	if (cy1 <= 12) {
		caux = cy1 ? cy1 - 1 : 0;
		at2 = map_attr [COORDS (cx1, caux)];
	}
	cy1 = (pry + 15) >> 4;
	if (cy1 <= 12) {
		caux = cy1 ? cy1 - 1 : 0;
		at3 = map_attr [COORDS (cx1, caux)];
	}
}
#endif

unsigned char collide_in (x0, y0, x1, y1) {
	return (x0 >= x1 && x0 <= x1 + 15 && y0 >= y1 && y0 <= y1 + 15);	
}

unsigned char collide (unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2) {
#ifdef SMALL_COLLISION
	return (x1 + 5 >= x2 && x1 <= x2 + 13 && y1 + 13 >= y2 && y1 <= y2 + 13);
#elif defined (TALL_COLLISION)
	return (x1 + 3 >= x2 && x1 <= x2 + 11 && y1 + 23 >= y2 && y1 <= y2 + 13);
#else
	return (x1 + 3 >= x2 && x1 <= x2 + 11 && y1 + 11 >= y2 && y1 <= y2 + 11);
#endif
}

signed int add_sign (signed int sign, signed int value) {
	return sign == 0 ? 0 : sign < 0 ? -value : value;
}

#ifdef ACTIVATE_SCRIPTING
void run_fire_script (void) {
	run_script (2 * MAP_SIZE + 2);
	run_script (n_pant + n_pant + 1);
}
#endif

signed int saturate (signed int v, signed int max) {
	return v >= 0 ? (v > max ? max : v) : (v < -max ? -max : v);
}

void pad_read (void) {
	// Thanks for this, Nicole & nesdev!
	// https://forums.nesdev.com/viewtopic.php?p=179315#p179315
	pad_this_frame = i;
	i = pad_poll (0);			// Read pads here.
	pad_this_frame = (pad_this_frame ^ i) & i;
}

#if defined (ENABLE_HOMING_FANTY) || defined (ENABLE_COCOS)
	// Lame but fast and tiny
	// Before the call: copy fanty's coordinates into rdx, rdy
	unsigned char distance (void) {
		rda = DELTA (prx, rdx); // dx
		rdb = DELTA (pry, rdy); // dy
		rdc = MIN (rda, rdb);
		return (rda + rdb - (rdc >> 1) - (rdc >> 2) + (rdc >> 4));
	}
#endif
