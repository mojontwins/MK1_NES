// NES MK1 v0.7
// Copyleft Mojon Twins 2013, 2015, 2016

// general.h
// General functions, vars & buffers

#define COORDS(x,y) (x+(y<<4))

unsigned char attr (signed char x, signed char y) {
	if (x < 0 || x > 15 || y < 0 || y > 11) return 0;
	return map_attr [x + (y << 4)];
}

unsigned char qtile (signed char x, signed char y) {
	return map_buff [x + (y << 4)];
}

unsigned char collide_in (x0, y0, x1, y1) {
	return (x0 >= x1 && x0 <= x1 + 15 && y0 >= y1 && y0 <= y1 + 15);	
}

unsigned char collide (unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2) {
#ifdef SMALL_COLLISION
	return (x1 + 8 >= x2 && x1 <= x2 + 8 && y1 + 8 >= y2 && y1 <= y2 + 8);
#else
	return (x1 + 13 >= x2 && x1 <= x2 + 13 && y1 + 12 >= y2 && y1 <= y2 + 12);
#endif
}

signed int add_sign (signed int sign, signed int value) {
	return sign == 0 ? 0 : sign < 0 ? -value : value;
}

#ifdef ACTIVATE_SCRIPTING
void run_fire_script (void) {
	run_script (2 * MAP_W * MAP_H + 2);
	run_script (n_pant + n_pant + 1);
}
#endif

signed int saturate (signed int v, signed int max) {
	return v >= 0 ? (v > max ? max : v) : (v < -max ? -max : v);
}

#if defined (PLAYER_KILLS_ENEMIES) || defined (PLAYER_CAN_FIRE) || defined (FANTY_KILLED_BY_TILE)
void kill_enemy (unsigned char gpit) {
	en_t [gpit] = 0;
#ifdef PERSISTENT_DEATHS
	ep_flags [n_pant + n_pant + n_pant + gpit] &= 0xFE;
#endif
#ifdef ACTIVATE_SCRIPTING
	run_script (2 * MAP_W * MAP_H + 5);
#endif
	pkilled ++;
#ifdef COUNT_KILLED_IN_FLAG
	flags [COUNT_KILLED_IN_FLAG] ++;
#endif
}
#endif
