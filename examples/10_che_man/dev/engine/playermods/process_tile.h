// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

void player_process_tile (at, x0, y0, x1, y1) {
	if (y0) y0 --;
	// Boxes

	#ifdef PLAYER_PUSH_BOXES
		if (y1) y1 --;
		if (
			at == 11 
			#ifdef FIRE_TO_PUSH
				&& b_button
			#endif
		) {
			b_button = 0;
			
			if (
				x0 > 0 && x0 < 15 && y0 > 0 && y0 < 11 &&
				ATTR (x1, y1) == 0
			) {
				#ifdef ENABLE_PUSHED_SCRIPT
					flags [PUSHED_TO_X_FLAG] = x1;
					flags [PUSHED_TO_Y_FLAG] = y1;
					flags [PUSHED_TILE_FLAG] = QTILE (x1, y1);
				#endif
				
				sfx_play (SFX_TILE, 1);
				_x = x0; _y = y0; _t = 0;  map_set ();
				_x = x1; _y = y1; _t = 14; map_set ();
				
				#ifdef ENABLE_PUSHED_SCRIPT
					just_pushed = 1;
					run_fire_script ();
				#endif
			}	
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
			_x = x0; _y = y0; _t = 0; map_set ();
			clear_cerrojo (COORDS(x0, y0));
			pkeys --;
			sfx_play (SFX_TILE, 1);
		} else {
			#ifdef ENABLE_NO
				no_ct = 100;
			#endif
		}
	#endif

}
