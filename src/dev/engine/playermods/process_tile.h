// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

void player_process_tile (at, x0, y0, x1, y1) {
	if (y0) y0 --;
	// Boxes

	#ifdef PLAYER_PUSH_BOXES
		if (y1) y1 --;
		if (
			at == 11 &&
			x0 > 0 && x0 < 15 && y0 > 0 && y0 < 11
			#ifdef FIRE_TO_PUSH
				&& (i & PAD_B)
			#endif
		) {
			pfiring = pushed_any = 1;
			map_set (x0, y0, 0);
			map_set (x1, y1, 14);
			sfx_play (1, 1);
		}
	#else
		y1=x1;//Shutup, compiler!
	#endif

	// Bolts

	#ifndef DEACTIVATE_KEYS
		if (
			at == 10 &&
			pkeys
		) {
			map_set (x0, y0, 0);
			clear_cerrojo (COORDS(x0, y0));
			pkeys --;
			sfx_play (1, 1);
		} else {
			no_ct = 100;
		}
	#endif

}
