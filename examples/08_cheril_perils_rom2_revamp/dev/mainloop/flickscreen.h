// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Change screen
// Very basic. Extend when needed.

if (prx == 4 && 
	#ifdef PLAYER_TOP_DOWN
		pvx < 0
	#else
		(cfx + pvx) < 0
	#endif
) {
	n_pant --;
	px = 244 << FIXBITS;
} else if (prx == 244 && 
	#ifdef PLAYER_TOP_DOWN
		pvx > 0
	#else
		(cfx + pvx) > 0
	#endif
) {
	n_pant ++;
	px = 4 << FIXBITS;
#ifdef PLAYER_TOP_DOWN				

	} else if (pry <= 16 && pvy < 0) {
		n_pant -= c_map_w;
		py = 192 << FIXBITS;
	} else if (pry >= 192 && pvy > 0) {
		n_pant += c_map_w;
		py = 16 << FIXBITS;
		
#else
	} else if (pry == 0 && pvy < 0 && n_pant >= c_map_w) {
		n_pant -= c_map_w;
		py = 192 << FIXBITS;
		if (pvy > -PLAYER_VY_JUMP_MAX) pvy = -PLAYER_VY_JUMP_MAX;
	} else if (pry >= 192 && pvy > 0) {
		n_pant += c_map_w;
		py = 0;
#endif				
}
