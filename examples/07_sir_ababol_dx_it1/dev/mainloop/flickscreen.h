// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// Change screen
// Very basic. Extend when needed.

/*
if (prx == 4 && pvx < 0) {
	n_pant --;
	px = 244 << FIXBITS;
} else if (prx == 244 && pvx > 0) {
	n_pant ++;
	px = 4 << FIXBITS;
#ifdef PLAYER_TOP_DOWN				

	} else if (pry <= 16 && pvy < 0) {
		n_pant -= MAP_W;
		py = 192 << FIXBITS;
	} else if (pry >= 192 && pvy > 0) {
		n_pant += MAP_W;
		py = 16 << FIXBITS;
		
#else
	} else if (pry == 0 && pvy < 0 && n_pant >= MAP_W) {
		n_pant -= MAP_W;
		py = 192 << FIXBITS;
		if (pvy > -PLAYER_VY_JUMP_MAX) pvy = -PLAYER_VY_JUMP_MAX;
	} else if (pry >= 192 && pvy > 0) {
		n_pant += MAP_W;
		py = 0;
#endif				
}
*/

// Custom screen / level switcher for Sir Ababol DX

if (prx == 4 && pvx < 0) {
	n_pant --;
	px = 244 << FIXBITS;
} else if (prx == 244 && pvx > 0) {
	n_pant ++;
	px = 4 << FIXBITS;
} else if (pry == 0 && pvy < 0) {
	if (level == 1 && n_pant < MAP_W) {
		level_switching = 1; break;
	} else {
		n_pant -= MAP_W;
		py = 192 << FIXBITS;
		if (pvy > -PLAYER_VY_JUMP_MAX) pvy = -PLAYER_VY_JUMP_MAX;		
	}
} else if (pry >= 192 && pvy > 0) {
	if (level == 0 && n_pant >= MAP_W) {
		level_switching = 1; break;
	} else {
		n_pant += MAP_W;
		py = 0;
	}
}
