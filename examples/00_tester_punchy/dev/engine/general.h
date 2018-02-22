// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

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

#if PLAYER_COLLISION_TOP_BG > 0
void cm_three_points (void) {
	// Always vertical, upon pry and pre-calculated cx1.
	cy1 = (pry - PLAYER_COLLISION_TOP_BG) >> 4;
	if (cy1 <= 12) {
		caux = cy1 ? cy1 - 1 : 0;
		at1 = map_attr [COORDS (cx1, caux)];
	}
	cy2 = pry >> 4;
	if (cy2 <= 12) {
		caux = cy2 ? cy2 - 1 : 0;
		at2 = map_attr [COORDS (cx1, caux)];
	}
	cy3 = (pry + 15) >> 4;
	if (cy3 <= 12) {
		caux = cy3 ? cy3 - 1 : 0;
		at3 = map_attr [COORDS (cx1, caux)];
	}
}
#endif

unsigned char collide_in (x0, y0, x1, y1) {
	return (x0 >= x1 && x0 <= x1 + 15 && y0 >= y1 && y0 <= y1 + 15);	
}

unsigned char collide (void) {

	// Player bounding box is:
	//     prx to prx + 7
	//     pry - PLAYER_COLLISION_TOP_FG to pry + 13
	// Enemy bounding box is:
	//     _en_x + 4 to _en_x + 11
	//     _en_y - ENEMS_COLLISION_TOP_FG to _en_y + 13

	return (
		prx + 3 >= _en_x && 
		prx <= _en_x + 11 && 
		pry + 13 + ENEMS_COLLISION_TOP_FG <= _en_y &&
		pry <= _en_y + 13 + PLAYER_COLLISION_TOP_FG
	);

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
