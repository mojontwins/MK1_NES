// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Change screen
// Very basic. Extend when needed.

void flickscreen_do_horizontal (void) {
	if (prx == 4 && 
		#if defined (PLAYER_TOP_DOWN) || !defined (ENABLE_CONVEYORS)
			pvx < 0
		#else
			(cfx + pvx) < 0
		#endif
	) {
		#ifdef DOUBLE_WIDTH
			n_pant -= 2;
		#else
			-- n_pant;
		#endif
		px = MAX_PRX << FIXBITS;
	} else if (prx == MAX_PRX && 
		#if defined (PLAYER_TOP_DOWN) || !defined (ENABLE_CONVEYORS)
			pvx > 0
		#else
			(cfx + pvx) > 0
		#endif
	) {
		#ifdef DOUBLE_WIDTH
			n_pant += 2;
		#else
			++ n_pant;
		#endif
		px = 4 << FIXBITS;
	}
}

void flickscreen_do_vertical (void) {
	#ifdef PLAYER_TOP_DOWN				

		if (pry <= 16 && pvy < 0) {
			n_pant -= c_map_w;
			py = 192 << FIXBITS;
		} else if (pry >= 192 && pvy > 0) {
			n_pant += c_map_w;
			py = 16 << FIXBITS;
		}
			
	#else
		
		if (pry == 0 && pvy < 0 && n_pant >= c_map_w) {
			n_pant -= c_map_w;
			py = 192 << FIXBITS;
			if (pvy > -PLAYER_VY_JUMP_MAX) pvy = -PLAYER_VY_JUMP_MAX;
		} else if (pry >= 192 && pvy > 0 && n_pant < c_map_size - c_map_w) {
			n_pant += c_map_w;
			py = 0;
		}
		
	#endif				
}
